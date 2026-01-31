# Contributing

Thanks for your interest in contributing!

## Quickstart

1. Install dependencies (CMake + Ninja recommended).
2. Build:
    - `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
    - `cmake --build build`
3. Test:
    - `ctest --test-dir build --output-on-failure`

## Code Style

- Format C++ with clang-format: `.clang-format`
- Static analysis with clang-tidy: `.clang-tidy`
- Prefer small, focused functions and clear naming

## Tests

- Add tests for new behavior.
- Prefer deterministic tests; use `build_test_artifacts/` for test data.

## Pull Requests

- Keep PRs focused and small.
- Include a short rationale and test evidence.
- Update README or docs when behavior changes.

## Security

Do not file public issues for security vulnerabilities. See [SECURITY.md](SECURITY.md).
