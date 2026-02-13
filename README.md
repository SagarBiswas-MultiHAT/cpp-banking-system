# C++ Banking System (Secure, Tested)

<div align="right">

![CI](https://github.com/SagarBiswas-MultiHAT/cpp-banking-system/actions/workflows/cmake.yml/badge.svg) 
&nbsp;
![C++17](https://img.shields.io/badge/c%2B%2B-17-blue) 
&nbsp;
![Build](https://img.shields.io/github/actions/workflow/status/SagarBiswas-MultiHAT/cpp-banking-system/cmake.yml?label=cmake) 
&nbsp;
![Tests](https://img.shields.io/badge/tests-ctest-brightgreen) 
&nbsp;
![License](https://img.shields.io/badge/license-MIT-green) 
&nbsp;
![Last commit](https://img.shields.io/github/last-commit/SagarBiswas-MultiHAT/cpp-banking-system)

</div>

Modern CLI banking simulator with secure credential handling, JSON-based storage, and automated tests/CI. It demonstrates production-grade practices (sanitizers, linting, atomic writes, and audited transactions) in a compact codebase.

## What this project is (and isn’t)

**This is a learning-focused banking simulator** built for clear architecture and safe defaults. It is **not** a real banking system and omits networked APIs, regulatory compliance, and cryptographic key management. If you need a production banking platform, this repository is a starting point for architecture and process—not a complete solution.

## Features

- Secure account creation with salted SHA-256 hashes and UUID account IDs.
- Login + deposit/withdraw with balance protection (no overdrafts).
- Contact updates and account introspection.
- Admin panel: list accounts, reset passwords, delete accounts.
- Transaction log for every balance-impacting action (JSONL).
- Atomic persistence to reduce corruption risk.

## Quickstart

### Windows (PowerShell + MSYS2 g++)

1. Install CMake (and optionally Ninja):

```powershell
winget install -e --id Kitware.CMake
winget install -e --id Ninja-build.Ninja
```

Close and reopen PowerShell after install.

2. Build and run:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/banking_app.exe
```

### Linux/macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/banking_app
```

## Example CLI walkthrough

```
=== Secure Bank ===
1) Login to account
2) Create account
3) Admin panel
0) Exit

> 2
Name: Sagar Biswas
Father name: Shyamal Biswas
National ID: 1122334455778899
Phone: 0172731077
Email: eng.sagar.aiub@example.com
Set password: ********
Initial deposit: 100

Account created. Save your Account ID: 2c6b8e7a-4aa6-4f3d-9d8d-6a1d1d0f2f3a
```

```
> 1
Account ID: 2c6b8e7a-4aa6-4f3d-9d8d-6a1d1d0f2f3a
Password: ********

--- User Panel ---
1) Deposit
2) Withdraw
3) Check info
4) Update contact
0) Back
```

```
> 3
Admin username: admin
Admin password: zxcvbnm

--- Admin Panel ---
1) View all accounts
2) Reset password
3) Delete account
0) Back
```

## Data & Config

- Accounts: `data/accounts.json`
- Transactions: `data/transactions.log` (newline-delimited JSON)
- Admin config: `config/app.json` (created during first-run setup)

### Accounts JSON schema

```json
{
    "id": "uuid",
    "name": "string",
    "father_name": "string",
    "national_id": "string",
    "phone": "string",
    "email": "string",
    "password_hash": "hex",
    "salt": "hex",
    "balance": 120.0,
    "created_at": 1738368000,
    "updated_at": 1738368000
}
```

### Transactions JSONL schema

```json
{
    "timestamp": 1738368000,
    "account_id": "uuid",
    "kind": "Deposit",
    "amount": 50.0,
    "note": "User deposit"
}
```

## Security model (summary)

- Passwords are never stored in plaintext; salted SHA-256 (Windows: bcrypt/CNG, Linux: OpenSSL).
- Input validation prevents negative/overflowed amounts and overdrafts.
- Atomic file writes reduce corruption risk; temp file then rename.
- Admin credentials are set on first run (no shipped default password).

See [docs/SECURITY.md](docs/SECURITY.md) for details.

## Architecture overview

- `src/` services (account, admin, storage, crypto, util)
- `include/` public headers
- `tests/` assertions for core flows
- `CMakeLists.txt` with warnings-as-errors and optional sanitizers

More detail in [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## API reference (service-level)

### `AccountService`

- `create_account(...)`: validates input, hashes password, persists account, logs initial deposit.
- `authenticate(id, password)`: verifies password hash and returns account if valid.
- `deposit(id, amount)`: validates amount and updates balance.
- `withdraw(id, amount)`: validates amount and prevents overdraft.
- `update_contact(id, phone, email)`: updates contact fields and logs an audit entry.

Defined in [include/account_service.h](include/account_service.h).

### `AdminService`

- `authenticate(username, password)`: validates admin credentials.
- `list_accounts()`: returns all accounts.
- `reset_password(id, new_password)`: replaces salt + hash and logs action.
- `delete_account(id)`: deletes account and logs action.

Defined in [include/admin_service.h](include/admin_service.h).

### `Storage`

- `load_accounts() / save_accounts()`: JSON persistence with atomic writes.
- `append_transaction(tx)`: JSONL append for audit trail.
- `load_config()`: loads admin credentials.

Defined in [include/storage.h](include/storage.h).

## Tests

### Windows (PowerShell + MSYS2 g++)

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target banking_tests
ctest --test-dir build --output-on-failure
```

### Linux/macOS (or any toolchain that provides `libasan`/`libubsan`)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Sanitizers are disabled automatically on Windows because the shipped MinGW toolchain on MSYS2 does not include the required runtime libraries. Use the Linux/macOS command when the toolchain can satisfy those dependencies.

More detail in [docs/TESTING.md](docs/TESTING.md).

## Docker

Build and run using Docker:

```bash
docker build -t cpp-banking-system .
docker run --rm -it cpp-banking-system
```

## Developer setup helpers

- Windows: `scripts/setup_dev.ps1`
- Linux/macOS: `scripts/setup_dev.sh`

## Performance notes

This project is optimized for small-scale CLI usage. See [docs/PERFORMANCE.md](docs/PERFORMANCE.md) for profiling tips and scalability considerations.

## Troubleshooting

- **CMake can’t find OpenSSL (Linux/macOS)**: install `libssl-dev` or the platform equivalent.
- **Sanitizers fail on Windows**: use Linux/macOS or a toolchain that ships the runtime libraries.
- **JSON parsing errors**: delete the corrupted `data/accounts.json` and retry (this resets accounts).

## FAQ

**Why JSON files instead of a database?**
This project is intentionally simple and portable; JSON keeps storage transparent.

**Is this safe for production?**
No. It is a secure simulator for learning and testing patterns.

## Future Implementation

- Include an option to "block users" in the admin panel.

## Contributing

Issues and PRs are welcome. Please include tests for new logic and run `ctest` before submission.

If you plan to contribute regularly, see CONTRIBUTING.md.

After Modifications, run a clean build and tests:

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```
