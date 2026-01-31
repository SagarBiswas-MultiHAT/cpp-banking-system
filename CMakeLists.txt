cmake_minimum_required(VERSION 3.20)
project(cpp_banking_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(_DEFAULT_SANITIZERS OFF)
else()
    set(_DEFAULT_SANITIZERS ON)
endif()
option(ENABLE_SANITIZERS "Enable Address/Undefined sanitizers" ${_DEFAULT_SANITIZERS})

if (WIN32 AND ENABLE_SANITIZERS)
    message(WARNING "Address/undefined sanitizers currently require runtime libraries that are not bundled with the default MSYS2/MinGW toolchain. Disabling sanitizers for this configuration.")
    set(ENABLE_SANITIZERS OFF CACHE BOOL "Enable Address/Undefined sanitizers" FORCE)
endif()

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if (ENABLE_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(banking_core
    src/account_service.cpp
    src/admin_service.cpp
    src/storage.cpp
    src/crypto.cpp
    src/util.cpp
)
target_include_directories(banking_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(banking_core PUBLIC nlohmann_json::nlohmann_json)

if (WIN32)
    target_link_libraries(banking_core PUBLIC bcrypt)
else()
    find_package(OpenSSL REQUIRED)
    target_link_libraries(banking_core PUBLIC OpenSSL::SSL OpenSSL::Crypto)
endif()

add_executable(banking_app src/main.cpp)
target_link_libraries(banking_app PRIVATE banking_core)

add_executable(banking_tests tests/bank_tests.cpp)
target_link_libraries(banking_tests PRIVATE banking_core)

enable_testing()
add_test(NAME banking_tests COMMAND banking_tests)
