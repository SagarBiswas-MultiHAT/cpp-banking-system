#include "admin_service.h"

#include <algorithm>
#include <ctime>

AdminService::AdminService(Storage& storage) : storage_(storage) {
    accounts_ = storage_.load_accounts();
    config_ = storage_.load_config();
}

bool AdminService::authenticate(const std::string& username, const std::string& password) {
    if (username != config_.admin.username) return false;
    return crypto::verify_password(password, config_.admin.salt, config_.admin.password_hash);
}

std::vector<Account> AdminService::list_accounts() {
    accounts_ = storage_.load_accounts();
    return accounts_;
}

bool AdminService::reset_password(const std::string& account_id, const std::string& new_password) {
    accounts_ = storage_.load_accounts();
    if (Account* acc = find_account_mutable(account_id)) {
        acc->salt = crypto::generate_salt();
        acc->password_hash = crypto::hash_password(new_password, acc->salt);
        acc->updated_at = std::time(nullptr);
        persist();
        storage_.append_transaction({ std::time(nullptr), account_id, "ResetPassword", 0.0, "Admin reset" });
        return true;
    }
    return false;
}

bool AdminService::delete_account(const std::string& account_id) {
    accounts_ = storage_.load_accounts();
    auto it = std::remove_if(accounts_.begin(), accounts_.end(), [&](const Account& a) { return a.id == account_id; });
    if (it == accounts_.end()) return false;
    accounts_.erase(it, accounts_.end());
    persist();
    storage_.append_transaction({ std::time(nullptr), account_id, "DeleteAccount", 0.0, "Admin delete" });
    return true;
}

void AdminService::persist() {
    storage_.save_accounts(accounts_);
}

Account* AdminService::find_account_mutable(const std::string& account_id) {
    for (auto& acc : accounts_) {
        if (acc.id == account_id) return &acc;
    }
    return nullptr;
}
