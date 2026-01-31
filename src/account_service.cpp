#include "account_service.h"

#include "util.h"

#include <stdexcept>
#include <ctime>

AccountService::AccountService(Storage& storage) : storage_(storage) {
    accounts_ = storage_.load_accounts();
}

Account AccountService::create_account(const std::string& name,
    const std::string& father_name,
    const std::string& national_id,
    const std::string& phone,
    const std::string& email,
    const std::string& password,
    double initial_deposit) {
    if (!is_valid_amount(initial_deposit)) {
        throw std::invalid_argument("Initial deposit must be positive and reasonable");
    }

    Account acc;
    acc.id = generate_uuid();
    acc.name = name;
    acc.father_name = father_name;
    acc.national_id = national_id;
    acc.phone = phone;
    acc.email = email;
    acc.salt = crypto::generate_salt();
    acc.password_hash = crypto::hash_password(password, acc.salt);
    acc.balance = initial_deposit;
    acc.created_at = std::time(nullptr);
    acc.updated_at = acc.created_at;

    accounts_.push_back(acc);
    persist();

    Transaction tx{ std::time(nullptr), acc.id, "Deposit", initial_deposit, "Initial deposit" };
    storage_.append_transaction(tx);

    return acc;
}

std::optional<Account> AccountService::authenticate(const std::string& account_id, const std::string& password) {
    accounts_ = storage_.load_accounts();
    if (Account* acc = find_account_mutable(account_id)) {
        if (crypto::verify_password(password, acc->salt, acc->password_hash)) {
            return *acc;
        }
    }
    return std::nullopt;
}

std::optional<Account> AccountService::get_account(const std::string& account_id) {
    accounts_ = storage_.load_accounts();
    if (Account* acc = find_account_mutable(account_id)) {
        return *acc;
    }
    return std::nullopt;
}

bool AccountService::deposit(const std::string& account_id, double amount) {
    accounts_ = storage_.load_accounts();
    if (!is_valid_amount(amount)) return false;
    if (Account* acc = find_account_mutable(account_id)) {
        acc->balance += amount;
        acc->updated_at = std::time(nullptr);
        persist();
        storage_.append_transaction({ std::time(nullptr), account_id, "Deposit", amount, "User deposit" });
        return true;
    }
    return false;
}

bool AccountService::withdraw(const std::string& account_id, double amount) {
    accounts_ = storage_.load_accounts();
    if (!is_valid_amount(amount)) return false;
    if (Account* acc = find_account_mutable(account_id)) {
        if (acc->balance < amount) return false;
        acc->balance -= amount;
        acc->updated_at = std::time(nullptr);
        persist();
        storage_.append_transaction({ std::time(nullptr), account_id, "Withdraw", amount, "User withdrawal" });
        return true;
    }
    return false;
}

bool AccountService::update_contact(const std::string& account_id, const std::string& phone, const std::string& email) {
    accounts_ = storage_.load_accounts();
    if (Account* acc = find_account_mutable(account_id)) {
        if (!phone.empty()) acc->phone = phone;
        if (!email.empty()) acc->email = email;
        acc->updated_at = std::time(nullptr);
        persist();
        storage_.append_transaction({ std::time(nullptr), account_id, "UpdateContact", 0.0, "Contact update" });
        return true;
    }
    return false;
}

void AccountService::persist() {
    storage_.save_accounts(accounts_);
}

Account* AccountService::find_account_mutable(const std::string& account_id) {
    for (auto& acc : accounts_) {
        if (acc.id == account_id) return &acc;
    }
    return nullptr;
}
