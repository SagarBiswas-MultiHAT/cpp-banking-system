#pragma once

#include <string>
#include <vector>
#include <optional>
#include <ctime>

struct Account {
    std::string id;
    std::string name;
    std::string father_name;
    std::string national_id;
    std::string phone;
    std::string email;
    std::string password_hash;
    std::string salt;
    double balance{ 0.0 };
    std::time_t created_at{ 0 };
    std::time_t updated_at{ 0 };
};

struct Transaction {
    std::time_t timestamp{ 0 };
    std::string account_id;
    std::string kind; // Deposit | Withdraw | ResetPassword | DeleteAccount
    double amount{ 0.0 };
    std::string note;
};

struct AdminCredential {
    std::string username;
    std::string password_hash;
    std::string salt;
};

struct AppConfig {
    AdminCredential admin;
};
