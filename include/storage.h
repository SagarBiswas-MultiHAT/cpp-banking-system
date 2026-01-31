#pragma once

#include "model.h"
#include <optional>
#include <string>
#include <vector>

class Storage {
public:
    explicit Storage(std::string accounts_path, std::string transactions_path, std::string config_path);

    // Account persistence
    std::vector<Account> load_accounts();
    void save_accounts(const std::vector<Account>& accounts);

    // Transactions persistence
    void append_transaction(const Transaction& tx);

    // Config persistence
    AppConfig load_config();
    void save_config(const AppConfig& cfg);

private:
    std::string accounts_path_;
    std::string transactions_path_;
    std::string config_path_;

    void ensure_files_exist();
};
