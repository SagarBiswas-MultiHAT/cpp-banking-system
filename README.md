
# C++ Banking System

![C++ Banking System](https://imgur.com/BF4sbAi.png)

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
