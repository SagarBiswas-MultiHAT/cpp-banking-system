# Architecture

## Overview

- **CLI**: `src/main.cpp` handles user/admin menus and input validation.
- **Services**: `AccountService` and `AdminService` in `src/account_service.cpp` and `src/admin_service.cpp` encapsulate business rules.
- **Persistence**: `Storage` in `src/storage.cpp` manages JSON files with atomic writes and transaction append-only logging.
- **Security**: `crypto` in `src/crypto.cpp` provides salted SHA-256 hashing via OpenSSL; salts are per-user.
- **Utilities**: `src/util.cpp` supplies UUID generation and amount validation.

## Data Flow

1. UI collects user input and calls service methods.
2. Services mutate in-memory models and request persistence.
3. Storage serializes to `data/accounts.json` and writes transaction logs.
4. Admin operations reuse the same storage and add audit entries.

## Files

- Accounts: `data/accounts.json` (array of account objects)
- Transactions: `data/transactions.log` (one JSON object per line)
- Config: `config/app.json` (admin credentials salted+hashed). Created automatically if missing.

## Extensibility

- Replace the storage backend by swapping `Storage` implementation while keeping the service API.
- Add more transaction types by extending `Transaction.kind` and logging in services.
- Introduce other frontends (REST/GUI) by reusing the service layer.
