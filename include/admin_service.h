#pragma once

#include "model.h"
#include "storage.h"
#include "crypto.h"
#include <vector>

class AdminService {
public:
    explicit AdminService(Storage& storage);

    bool authenticate(const std::string& username, const std::string& password);
    std::vector<Account> list_accounts();
    bool reset_password(const std::string& account_id, const std::string& new_password);
    bool delete_account(const std::string& account_id);

private:
    Storage& storage_;
    AppConfig config_;
    std::vector<Account> accounts_;

    void persist();
    Account* find_account_mutable(const std::string& account_id);
};
