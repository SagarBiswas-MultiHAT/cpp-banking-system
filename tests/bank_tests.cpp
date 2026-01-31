#include <cassert>
#include <filesystem>
#include <iostream>

#include "account_service.h"
#include "admin_service.h"
#include "crypto.h"
#include "storage.h"

namespace fs = std::filesystem;

namespace {
    void require(bool condition, const char* message) {
        if (!condition) {
            std::cerr << "TEST FAILED: " << message << "\n";
            std::exit(1);
        }
    }
}

int main() {
    fs::create_directories("build_test_artifacts");
    const std::string accounts_path = "build_test_artifacts/accounts.json";
    const std::string tx_path = "build_test_artifacts/transactions.log";
    const std::string cfg_path = "build_test_artifacts/config.json";

    if (fs::exists(accounts_path)) fs::remove(accounts_path);
    if (fs::exists(tx_path)) fs::remove(tx_path);
    if (fs::exists(cfg_path)) fs::remove(cfg_path);

    Storage storage(accounts_path, tx_path, cfg_path);
    AppConfig cfg;
    cfg.admin.username = "admin";
    cfg.admin.salt = crypto::generate_salt();
    cfg.admin.password_hash = crypto::hash_password("admin123", cfg.admin.salt);
    storage.save_config(cfg);

    AccountService accounts(storage);
    AdminService admin(storage);

    auto acc = accounts.create_account("Alice", "Bob", "1234", "555-111", "alice@example.com", "secret", 100.0);
    require(!acc.id.empty(), "account id should not be empty");

    // Authenticate succeeds
    auto auth = accounts.authenticate(acc.id, "secret");
    require(auth.has_value(), "authentication should succeed");

    // Deposit
    require(accounts.deposit(acc.id, 50.0), "deposit should succeed");
    auto after_deposit = accounts.get_account(acc.id);
    require(after_deposit.has_value(), "account should exist after deposit");
    require(after_deposit->balance == 150.0, "balance should be 150 after deposit");

    // Withdraw within balance
    require(accounts.withdraw(acc.id, 30.0), "withdraw should succeed");
    auto after_withdraw = accounts.get_account(acc.id);
    require(after_withdraw.has_value(), "account should exist after withdraw");
    require(after_withdraw->balance == 120.0, "balance should be 120 after withdraw");

    // Prevent overdraw
    require(!accounts.withdraw(acc.id, 1000.0), "overdraft should be rejected");

    // Admin reset password
    require(admin.authenticate("admin", "admin123"), "default admin credentials should work in tests");
    require(admin.reset_password(acc.id, "newpass"), "admin password reset should succeed");
    auto post_reset = accounts.authenticate(acc.id, "newpass");
    require(post_reset.has_value(), "authentication should succeed after password reset");

    // Admin delete
    require(admin.delete_account(acc.id), "admin delete should succeed");
    auto missing = accounts.get_account(acc.id);
    require(!missing.has_value(), "account should be missing after delete");

    std::cout << "All tests passed.\n";
    return 0;
}
