
# C++ Banking System

![C++ Banking System](https://scontent.fdac138-1.fna.fbcdn.net/v/t39.30808-6/474491472_122136891218552158_8494252222310176576_n.jpg?stp=dst-jpg_p526x296_tt6&_nc_cat=110&ccb=1-7&_nc_sid=127cfc&_nc_ohc=YRULWqoqy_EQ7kNvgHcNHqF&_nc_zt=23&_nc_ht=scontent.fdac138-1.fna&_nc_gid=ATqL_SQxTXKtXxGQj15W1hQ&oh=00_AYAJNv4b8twoGZRw35MXE4AQdLJgwxZO1d08-7jyBHVIWw&oe=67946426)

A simple banking system implemented in C++ for managing customer accounts and performing administrative operations. This project simulates basic banking functionalities, including account creation, deposit, withdrawal, balance inquiry, and admin-level controls like viewing all accounts, resetting passwords, and deleting accounts.

## Features

### User Functions
- **Create Account**: Create a new bank account with user details.
- **Login**: Secure login to user accounts using account number and password.
- **Deposit Amount**: Add funds to the account.
- **Withdraw Amount**: Withdraw funds from the account.
- **Check Info**: View account details and current balance.
- **Update Account Info**: Modify contact details such as phone number and email.

### Admin Functions
- **View All Accounts**: List all bank accounts and their details.
- **Reset Password**: Reset the password for any account.
- **Generate Report**: Generate a comprehensive report of all accounts.
- **Delete Account**: Remove an account from the system.
- **Admin Login**: Secure login for administrators with default credentials.

##### Enter admin username :: #admin#
##### Enter admin password :: #admin#

## Getting Started

### Prerequisites
- C++ compiler (e.g., g++)
- Standard C++ Libraries

### How to Compile
```bash
g++ banking_system.cpp -o banking_system
```

### How to Run
```bash
./banking_system
```

## Usage
1. Follow the prompts to either login, create an account, or access the admin panel.
2. Admin credentials are predefined as `#admin#` for both username and password.

## File Structure
- `data.txt`: Stores account information.
- `transactions.txt`: Logs deposit and withdrawal transactions.

## Contributing
Feel free to fork this repository and submit pull requests to improve the system or add new features.


## Author
[SagarBiswas-MultiHAT](https://github.com/SagarBiswas-MultiHAT)
