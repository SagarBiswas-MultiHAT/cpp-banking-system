# Changelog

## 1.0.0 - 2026-02-01

- Rebuilt architecture with services for accounts/admin/storage.
- Added secure password hashing (salted SHA-256 via OpenSSL).
- Switched storage to JSON with atomic writes and transaction logging.
- Added CMake build with warnings-as-errors and optional sanitizers.
- Added automated tests and CI workflow.
- Documented architecture, security, and testing.
