#!/usr/bin/env bash
set -euo pipefail

if command -v apt-get >/dev/null 2>&1; then
  sudo apt-get update
  sudo apt-get install -y cmake ninja-build g++ libssl-dev
elif command -v brew >/dev/null 2>&1; then
  brew install cmake ninja openssl
else
  echo "Install CMake, Ninja, and OpenSSL with your package manager."
fi
