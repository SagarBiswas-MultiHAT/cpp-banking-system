#pragma once

#include <string>

namespace crypto {
    std::string generate_salt(std::size_t length = 16);
    std::string hash_password(const std::string& password, const std::string& salt);
    bool verify_password(const std::string& password, const std::string& salt, const std::string& expected_hash);
}
