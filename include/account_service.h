#pragma once

#include "model.h"
#include "storage.h"
#include "crypto.h"
#include <optional>

class AccountService {
public:
    explicit AccountService(Storage& storage);

    Account create_account(const std::string& name,
        const std::string& father_name,
        const std::string& national_id,
        const std::string& phone,
        const std::string& email,
        const std::string& password,
        double initial_deposit);

    std::optional<Account> authenticate(const std::string& account_id, const std::string& password);
    std::optional<Account> get_account(const std::string& account_id);

    bool deposit(const std::string& account_id, double amount);
    bool withdraw(const std::string& account_id, double amount);
    bool update_contact(const std::string& account_id, const std::string& phone, const std::string& email);

private:
    Storage& storage_;
    std::vector<Account> accounts_;

    void persist();
    Account* find_account_mutable(const std::string& account_id);
};
