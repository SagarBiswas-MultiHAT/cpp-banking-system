#include "crypto.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#if defined(_MSC_VER)
#pragma comment(lib, "bcrypt.lib")
#endif
#else
#include <openssl/evp.h>
#include <openssl/rand.h>
#endif

namespace {
    std::string to_hex(const unsigned char* data, unsigned int length) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (unsigned int i = 0; i < length; ++i) {
            oss << std::setw(2) << static_cast<unsigned int>(data[i]);
        }
        return oss.str();
    }
}

namespace crypto {
    std::string generate_salt(std::size_t length) {
        std::vector<unsigned char> buffer(length);
#if defined(_WIN32)
        if (BCryptGenRandom(nullptr, buffer.data(), static_cast<ULONG>(buffer.size()), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) {
            throw std::runtime_error("Failed to generate secure random salt");
        }
#else
        if (RAND_bytes(buffer.data(), static_cast<int>(buffer.size())) != 1) {
            throw std::runtime_error("Failed to generate secure random salt");
        }
#endif
        return to_hex(buffer.data(), static_cast<unsigned int>(buffer.size()));
    }

    std::string hash_password(const std::string& password, const std::string& salt) {
        std::string combined = salt + password;
#if defined(_WIN32)
        BCRYPT_ALG_HANDLE alg = nullptr;
        BCRYPT_HASH_HANDLE hash_handle = nullptr;

        if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, nullptr, 0) != 0) {
            throw std::runtime_error("Failed to open SHA256 provider");
        }

        DWORD object_length = 0;
        DWORD data_length = 0;
        if (BCryptGetProperty(alg, BCRYPT_OBJECT_LENGTH, reinterpret_cast<PUCHAR>(&object_length), sizeof(object_length), &data_length, 0) !=
            0) {
            BCryptCloseAlgorithmProvider(alg, 0);
            throw std::runtime_error("Failed to read SHA256 object length");
        }

        DWORD hash_length = 0;
        if (BCryptGetProperty(alg, BCRYPT_HASH_LENGTH, reinterpret_cast<PUCHAR>(&hash_length), sizeof(hash_length), &data_length, 0) != 0) {
            BCryptCloseAlgorithmProvider(alg, 0);
            throw std::runtime_error("Failed to read SHA256 hash length");
        }

        std::vector<unsigned char> hash_object(object_length);
        std::vector<unsigned char> hash(hash_length);

        if (BCryptCreateHash(alg, &hash_handle, hash_object.data(), static_cast<ULONG>(hash_object.size()), nullptr, 0, 0) != 0) {
            BCryptCloseAlgorithmProvider(alg, 0);
            throw std::runtime_error("Failed to create SHA256 hash");
        }

        if (BCryptHashData(hash_handle, reinterpret_cast<PUCHAR>(combined.data()), static_cast<ULONG>(combined.size()), 0) != 0) {
            BCryptDestroyHash(hash_handle);
            BCryptCloseAlgorithmProvider(alg, 0);
            throw std::runtime_error("Failed to hash password");
        }

        if (BCryptFinishHash(hash_handle, hash.data(), static_cast<ULONG>(hash.size()), 0) != 0) {
            BCryptDestroyHash(hash_handle);
            BCryptCloseAlgorithmProvider(alg, 0);
            throw std::runtime_error("Failed to finalize password hash");
        }

        BCryptDestroyHash(hash_handle);
        BCryptCloseAlgorithmProvider(alg, 0);

        return to_hex(hash.data(), static_cast<unsigned int>(hash.size()));
#else
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_length = 0;

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to allocate EVP_MD_CTX");
        }

        if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1 ||
            EVP_DigestUpdate(ctx, combined.data(), combined.size()) != 1 ||
            EVP_DigestFinal_ex(ctx, hash, &hash_length) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to compute password hash");
        }

        EVP_MD_CTX_free(ctx);
        return to_hex(hash, hash_length);
#endif
    }

    bool verify_password(const std::string& password, const std::string& salt, const std::string& expected_hash) {
        return hash_password(password, salt) == expected_hash;
    }
} // namespace crypto
