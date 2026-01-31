#include "storage.h"

#include "crypto.h"
#include "model.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

namespace {
    Account account_from_json(const nlohmann::json& j) {
        Account acc;
        acc.id = j.at("id").get<std::string>();
        acc.name = j.at("name").get<std::string>();
        acc.father_name = j.at("father_name").get<std::string>();
        acc.national_id = j.at("national_id").get<std::string>();
        acc.phone = j.at("phone").get<std::string>();
        acc.email = j.at("email").get<std::string>();
        acc.password_hash = j.at("password_hash").get<std::string>();
        acc.salt = j.at("salt").get<std::string>();
        acc.balance = j.at("balance").get<double>();
        acc.created_at = j.at("created_at").get<std::time_t>();
        acc.updated_at = j.at("updated_at").get<std::time_t>();
        return acc;
    }

    nlohmann::json account_to_json(const Account& acc) {
        return {
            {"id", acc.id},
            {"name", acc.name},
            {"father_name", acc.father_name},
            {"national_id", acc.national_id},
            {"phone", acc.phone},
            {"email", acc.email},
            {"password_hash", acc.password_hash},
            {"salt", acc.salt},
            {"balance", acc.balance},
            {"created_at", acc.created_at},
            {"updated_at", acc.updated_at}
        };
    }

    nlohmann::json admin_to_json(const AdminCredential& admin) {
        return {
            {"username", admin.username},
            {"password_hash", admin.password_hash},
            {"salt", admin.salt}
        };
    }

    AdminCredential admin_from_json(const nlohmann::json& j) {
        AdminCredential admin;
        admin.username = j.at("username").get<std::string>();
        admin.password_hash = j.at("password_hash").get<std::string>();
        admin.salt = j.at("salt").get<std::string>();
        return admin;
    }
}

Storage::Storage(std::string accounts_path, std::string transactions_path, std::string config_path)
    : accounts_path_(std::move(accounts_path)),
    transactions_path_(std::move(transactions_path)),
    config_path_(std::move(config_path)) {
    ensure_files_exist();
}

void Storage::ensure_files_exist() {
    fs::create_directories(fs::path(accounts_path_).parent_path());
    fs::create_directories(fs::path(transactions_path_).parent_path());
    fs::create_directories(fs::path(config_path_).parent_path());

    if (!fs::exists(accounts_path_)) {
        std::ofstream out(accounts_path_);
        out << "[]";
    }
    if (!fs::exists(transactions_path_)) {
        std::ofstream out(transactions_path_);
    }
    // Config is created explicitly by the application on first run.
}

std::vector<Account> Storage::load_accounts() {
    std::ifstream in(accounts_path_);
    if (!in.is_open()) {
        throw std::runtime_error("Unable to open accounts file");
    }
    nlohmann::json j;
    in >> j;
    std::vector<Account> accounts;
    for (const auto& item : j) {
        accounts.push_back(account_from_json(item));
    }
    return accounts;
}

void Storage::save_accounts(const std::vector<Account>& accounts) {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& acc : accounts) {
        j.push_back(account_to_json(acc));
    }

    fs::path target(accounts_path_);
    fs::path tmp = target;
    tmp += ".tmp";

    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Unable to open temp accounts file for writing");
        }
        out << j.dump(2);
    }

    if (fs::exists(target)) {
        fs::remove(target);
    }
    fs::rename(tmp, target);
}

void Storage::append_transaction(const Transaction& tx) {
    nlohmann::json j = {
        {"timestamp", tx.timestamp},
        {"account_id", tx.account_id},
        {"kind", tx.kind},
        {"amount", tx.amount},
        {"note", tx.note}
    };
    std::ofstream out(transactions_path_, std::ios::app);
    if (!out.is_open()) {
        throw std::runtime_error("Unable to open transactions file");
    }
    out << j.dump() << "\n";
}

AppConfig Storage::load_config() {
    std::ifstream in(config_path_);
    if (!in.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }
    nlohmann::json j;
    in >> j;
    AppConfig cfg;
    cfg.admin = admin_from_json(j.at("admin"));
    return cfg;
}

void Storage::save_config(const AppConfig& cfg) {
    nlohmann::json j;
    j["admin"] = admin_to_json(cfg.admin);

    fs::path target(config_path_);
    fs::path tmp = target;
    tmp += ".tmp";

    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Unable to open temp config file for writing");
        }
        out << j.dump(2);
    }

    if (fs::exists(target)) {
        fs::remove(target);
    }
    fs::rename(tmp, target);
}
