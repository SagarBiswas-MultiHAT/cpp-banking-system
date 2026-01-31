#include "util.h"

#include <array>
#include <chrono>
#include <random>
#include <sstream>

namespace {
    char random_hex(std::mt19937_64& rng) {
        static constexpr std::array<char, 16> hex_digits = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        std::uniform_int_distribution<int> dist(0, 15);
        return hex_digits[dist(rng)];
    }
}

std::string generate_uuid() {
    // Simple random-based UUID v4 compatible string
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::ostringstream oss;
    int sections[] = { 8, 4, 4, 4, 12 };
    for (int i = 0; i < 5; ++i) {
        if (i != 0) oss << '-';
        for (int j = 0; j < sections[i]; ++j) {
            oss << random_hex(rng);
        }
    }
    return oss.str();
}

bool is_valid_amount(double value) {
    return value > 0.0 && value < 1e12; // basic sanity bounds
}
