# Testing

## Running

```
cmake -S . -B build -DENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Coverage

- Account lifecycle: create, authenticate, deposit, withdraw (with overdraft prevention).
- Admin lifecycle: default admin auth, password reset, account deletion.
- Persistence: operations rely on JSON files in `build_test_artifacts/` to avoid touching production data.

## Adding Tests

- Prefer fast, deterministic assertions with no external dependencies.
- If adding integration-style tests, isolate artifacts under `build_test_artifacts/` or a temp directory.
- Keep tests independent; each should set up and tear down its own data files.
