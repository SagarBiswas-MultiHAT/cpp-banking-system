#include "account_service.h"
#include "admin_service.h"
#include "crypto.h"
#include "storage.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <filesystem>
#include <string>

namespace {
    std::string prompt_line(const std::string& label) {
        std::cout << label;
        std::string value;
        std::getline(std::cin, value);
        return value;
    }

    std::string prompt_non_empty(const std::string& label) {
        while (true) {
            std::string value = prompt_line(label);
            if (!value.empty()) return value;
            std::cout << "Value cannot be empty.\n";
        }
    }

    double prompt_amount(const std::string& label) {
        while (true) {
            std::cout << label;
            std::string line;
            std::getline(std::cin, line);
            try {
                double value = std::stod(line);
                if (value > 0) return value;
            }
            catch (...) {
            }
            std::cout << "Invalid amount. Please enter a positive number.\n";
        }
    }

    void render_account(const Account& acc) {
        std::cout << "\nAccount ID: " << acc.id
            << "\nName      : " << acc.name
            << "\nFather    : " << acc.father_name
            << "\nNID       : " << acc.national_id
            << "\nPhone     : " << acc.phone
            << "\nEmail     : " << acc.email
            << "\nBalance   : " << std::fixed << std::setprecision(2) << acc.balance
            << "\n";
    }
}

int main() {
    const std::string accounts_path = "data/accounts.json";
    const std::string transactions_path = "data/transactions.log";
    const std::string config_path = "config/app.json";

    Storage storage(accounts_path, transactions_path, config_path);
    AccountService account_service(storage);

    if (!std::filesystem::exists(config_path)) {
        std::cout << "\n=== First run admin setup ===\n";
        AppConfig cfg;
        cfg.admin.username = prompt_non_empty("Admin username: ");
        std::string password = prompt_non_empty("Admin password: ");
        cfg.admin.salt = crypto::generate_salt();
        cfg.admin.password_hash = crypto::hash_password(password, cfg.admin.salt);
        storage.save_config(cfg);
        std::cout << "Admin account created.\n";
    }

    AdminService admin_service(storage);

    while (true) {
        std::cout << "\n=== Secure Bank ===\n";
        std::cout << "1) Login to account\n";
        std::cout << "2) Create account\n";
        std::cout << "3) Admin panel\n";
        std::cout << "0) Exit\n";
        std::cout << "Choice: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "0" || std::cin.eof()) break;
        if (choice == "2") {
            std::string name = prompt_line("Name: ");
            std::string father = prompt_line("Father name: ");
            std::string nid = prompt_line("National ID: ");
            std::string phone = prompt_line("Phone: ");
            std::string email = prompt_line("Email: ");
            std::string password = prompt_line("Set password: ");
            double initial = prompt_amount("Initial deposit: ");

            Account acc = account_service.create_account(name, father, nid, phone, email, password, initial);
            std::cout << "\nAccount created. Save your Account ID: " << acc.id << "\n";
            continue;
        }

        if (choice == "1") {
            std::string account_id = prompt_line("Account ID: ");
            std::string password = prompt_line("Password: ");
            auto account = account_service.authenticate(account_id, password);
            if (!account) {
                std::cout << "Invalid credentials.\n";
                continue;
            }
            while (true) {
                std::cout << "\n--- User Panel ---\n";
                std::cout << "1) Deposit\n";
                std::cout << "2) Withdraw\n";
                std::cout << "3) Check info\n";
                std::cout << "4) Update contact\n";
                std::cout << "0) Back\n";
                std::cout << "Choice: ";
                std::string user_choice;
                std::getline(std::cin, user_choice);
                if (user_choice == "0") break;
                if (user_choice == "1") {
                    double amt = prompt_amount("Amount to deposit: ");
                    if (account_service.deposit(account_id, amt)) {
                        std::cout << "Deposit successful.\n";
                    }
                    else {
                        std::cout << "Deposit failed.\n";
                    }
                }
                else if (user_choice == "2") {
                    double amt = prompt_amount("Amount to withdraw: ");
                    if (account_service.withdraw(account_id, amt)) {
                        std::cout << "Withdraw successful.\n";
                    }
                    else {
                        std::cout << "Withdraw failed (insufficient funds or invalid).\n";
                    }
                }
                else if (user_choice == "3") {
                    auto info = account_service.get_account(account_id);
                    if (info) render_account(*info);
                }
                else if (user_choice == "4") {
                    std::string new_phone = prompt_line("New phone (leave blank to keep): ");
                    std::string new_email = prompt_line("New email (leave blank to keep): ");
                    if (account_service.update_contact(account_id, new_phone, new_email)) {
                        std::cout << "Contact updated.\n";
                    }
                    else {
                        std::cout << "Update failed.\n";
                    }
                }
                else {
                    std::cout << "Unknown choice.\n";
                }
            }
            continue;
        }

        if (choice == "3") {
            std::string username = prompt_line("Admin username: ");
            std::string password = prompt_line("Admin password: ");
            if (!admin_service.authenticate(username, password)) {
                std::cout << "Invalid admin credentials.\n";
                continue;
            }
            while (true) {
                std::cout << "\n--- Admin Panel ---\n";
                std::cout << "1) View all accounts\n";
                std::cout << "2) Reset password\n";
                std::cout << "3) Delete account\n";
                std::cout << "0) Back\n";
                std::cout << "Choice: ";
                std::string admin_choice;
                std::getline(std::cin, admin_choice);
                if (admin_choice == "0") break;
                if (admin_choice == "1") {
                    auto accounts = admin_service.list_accounts();
                    std::cout << "Total accounts: " << accounts.size() << "\n";
                    for (const auto& acc : accounts) {
                        render_account(acc);
                    }
                }
                else if (admin_choice == "2") {
                    std::string id = prompt_line("Account ID to reset: ");
                    std::string new_pw = prompt_line("New password: ");
                    if (admin_service.reset_password(id, new_pw)) {
                        std::cout << "Password reset.\n";
                    }
                    else {
                        std::cout << "Account not found.\n";
                    }
                }
                else if (admin_choice == "3") {
                    std::string id = prompt_line("Account ID to delete: ");
                    if (admin_service.delete_account(id)) {
                        std::cout << "Account deleted.\n";
                    }
                    else {
                        std::cout << "Account not found.\n";
                    }
                }
                else {
                    std::cout << "Unknown choice.\n";
                }
            }
            continue;
        }

        std::cout << "Unknown choice.\n";
    }

    std::cout << "Goodbye.\n";
    return 0;
}
