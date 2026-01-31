# Security Notes

- **Passwords**: Salted SHA-256 via OpenSSL; salts are random per account. Admin credentials live in `config/app.json` and are created during the first-run setup.
- **No plaintext secrets**: Passwords are stored only as `salt` + `password_hash`.
- **Authentication**: Constant-time comparison is approximated by re-hashing and equality check; swap in a dedicated timing-safe compare if available in your crypto backend.
- **Input validation**: Amounts must be positive and below 1e12. Withdrawals fail if balance is insufficient.
- **Atomic writes**: Account updates are written to a temp file then renamed to reduce corruption risk.
- **Logs**: Every balance-impacting action is appended to `data/transactions.log` (JSONL) for auditability.
- **Defaults**: No hard-coded admin password is shipped. The first run prompts for admin credentials.
