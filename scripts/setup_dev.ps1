Write-Host "Installing CMake and Ninja..."
winget install -e --id Kitware.CMake
winget install -e --id Ninja-build.Ninja
Write-Host "Done. Reopen your terminal if needed."
