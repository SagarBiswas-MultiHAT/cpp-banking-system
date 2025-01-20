#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class BankAccount {
    int accNo;
    string name, Fname, NID, P_no, email, password;
    float amount;
    fstream file;

public:
    void createAccount();
    bool login(int accNo, const string& enteredPassword);
    void depositAmount(int searchAccNo);
    void withdrawAmount(int searchAccNo);
    void checkInfo(int searchAccNo);
    void updateAccountInfo(int searchAccNo);

private:
    void updateFile(int searchAccNo, float changeAmount);
    bool loadAccount(int searchAccNo);
    void saveAccount();
};
class Admin {
    fstream file;
    int accNo;
    string name, Fname, NID, P_no, email, password;
    float amount;

public:
    void viewAllAccounts();
    void resetPassword(int accNo, const string& newPassword);
    bool login(const string& enteredUsername, const string& enteredPassword);
    void deleteAccount(int accNo);
};

int main() {
    BankAccount obj;
    Admin admin;
    char choice;

    cout << "\n\n\n\t\t\t......::: Sagar's Secure Bank :::......";
    cout << "\n\t=======================================================================\n";
    while (true) {
        cout << "\n\n==> Main Menu \n";
        cout << "\n\t\t\t    :: ::::::::::::::::::::::::::  ::";
        cout << "\n\t\t\t    :: ------- Secure Bank ------- ::";
        cout << "\n\t\t\t    ::         -----------         ::";
        cout << "\n\t\t\t    :: press 1 to Login Account    ::";
        cout << "\n\t\t\t    :: press 2 to Create Account   ::";
        cout << "\n\t\t\t    :: press 3 for Admin Panel     ::";
        cout << "\n\t\t\t    :: press 0 to Exit             ::";
        cout << "\n\t\t\t    :: ........................... ::\n\t\t\t\t\t    ";
        cin >> choice;

        cout << "\n\n";

        switch (choice) {
        case '1': {
            int searchAccNo;
            string enteredPassword;
            cout << "..:: Enter your account no\t: ";
            cin >> searchAccNo;
            cout << "..:: Enter your password  \t: ";
            cin >> enteredPassword;
            if (obj.login(searchAccNo, enteredPassword)) {
                while (true) {
                    cout << "\n\n\t\t\t:: :::::::::::::::::::::::::::::: ::";
                    cout << "\n\t\t\t:: --------- USER PANEL --------- ::";
                    cout << "\n\t\t\t::           ----------           ::";
                    cout << "\n\t\t\t:: press 1 to Deposit Amount      ::";
                    cout << "\n\t\t\t:: press 2 to Withdraw Amount     ::";
                    cout << "\n\t\t\t:: press 3 to Check Info          ::";
                    cout << "\n\t\t\t:: press 5 to Update Account Info ::";
                    cout << "\n\t\t\t:: press 0 to Exit Menu           ::";
                    cout << "\n\t\t\t:: .............................. ::\n\t\t\t\t\t  ";
                    cin >> choice;

                    switch (choice) {
                    case '1':
                        obj.depositAmount(searchAccNo);
                        break;
                    case '2':
                        obj.withdrawAmount(searchAccNo);
                        break;
                    case '3':
                        obj.checkInfo(searchAccNo);
                        break;
                    case '5':
                        obj.updateAccountInfo(searchAccNo);
                        break;
                    case '0':
                        // Break out of the inner while loop to return to the main menu
                        break;
                    default:
                        cout << "Invalid Choice...!";
                        break;
                    }
                    if (choice == '0') break; // Exit the inner while loop
                    cout << "\n\n";
                    system("pause"); // used to pause the program
                    system("cls"); // used to clear the screen
                }
            }
            else {
                cout << "Invalid account number or password!\n";
            }
            break;
        }
        case '2':
            obj.createAccount();
            break;
        case '3': {
            string adminUsername, adminPassword;
            cout << "..:: Enter admin username\t: ";
            cin >> adminUsername;
            cout << "..:: Enter admin password\t: ";
            cin >> adminPassword;
            if (admin.login(adminUsername, adminPassword)) {
                while (true) {

                    char adminChoice; // Change from int to char
                    cout << "\n\n\t\t\t:: ::::::::::::::::::::::::::::: ::";
                    cout << "\n\t\t\t:: -------- ADMIN PANEL -------- ::";
                    cout << "\n\t\t\t::          -----------          ::";
                    cout << "\n\t\t\t:: press 1 to View All Accounts  ::";
                    cout << "\n\t\t\t:: press 2 to Reset Password     ::";
                    cout << "\n\t\t\t:: press 4 to Delete Account     ::";
                    cout << "\n\t\t\t:: press 0 to Exit Admin Panel   ::";
                    cout << "\n\t\t\t:: ............................. ::\n\t\t\t\t\t  ";
                    cin >> adminChoice;
                    cout << "\n\n";
                    switch (adminChoice) {
                    case '1':
                        admin.viewAllAccounts();
                        break;
                    case '2': {
                        int accNo;
                        string newPassword;
                        cout << "..:: Enter account number to reset password \t: ";
                        cin >> accNo;
                        cout << "..:: Enter new password \t\t\t\t\t: ";
                        cin >> newPassword;
                        admin.resetPassword(accNo, newPassword);
                        cout << "\n\t\t ===> Password reset successfully. <===\n";
                        break;
                    }
                    case '4': {
                        int accNo;
                        cout << "..:: Enter account number to delete :: ";
                        cin >> accNo;
                        admin.deleteAccount(accNo);
                        break;
                    }
                    case '0':
                        break;
                    default:
                        cout << "\t\t===> Invalid Choice...!";
                        break;
                    }
                    if (adminChoice == '0') break; // Exit the inner while loop
                    cout << "\n\n";
                    system("pause"); // used to pause the program
                    system("cls"); // used to clear the screen
                }
            }
            else {
                cout << "\n\n ===> Invalid admin username or password!\n";
            }
            break;
        }
        case '0':
            return 0;
        default:
            cout << " Invalid choice...! ";
            break;
        }
    }
    return 0;
}

void BankAccount::createAccount() {
    srand(time(0));
    accNo = rand() * rand() + rand() * rand();

    cout << "..:: Enter Your name           : ";
    cin >> ws;
    getline(cin, name);

    cout << "..:: Enter Your Father name    : ";
    getline(cin, Fname);

    cout << "..:: Enter Your NID            : ";
    getline(cin, NID);

    cout << "..:: Enter Your phone no       : ";
    getline(cin, P_no);

    cout << "..::Enter Your email           : ";
    getline(cin, email);

    cout << "..:: Set your account password : ";
    cin >> password;

    cout << "..:: Enter Your amount         : ";
    cin >> amount;

    cout << endl << accNo << " This is your account number... \n";
    cout << "\n\t ==> Please, save it \n\n";

    saveAccount();
}

bool BankAccount::login(int searchAccNo, const string& enteredPassword) {
    if (loadAccount(searchAccNo)) {
        return password == enteredPassword;
    }
    return false;
}

void BankAccount::depositAmount(int searchAccNo) {
    float newAmount;
    cout << "..:: Enter amount to deposit : ";
    cin >> newAmount;
    cout << "\n";

    updateFile(searchAccNo, newAmount);

    // Log the transaction
    fstream transactionFile;
    transactionFile.open("transactions.txt", ios::out | ios::app);
    if (transactionFile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        transactionFile << dt << "\tDeposit\t" << newAmount << endl;
        transactionFile.close();
    }
    else {
        cout << "Transaction file opening error!\n";
    }
}

void BankAccount::withdrawAmount(int searchAccNo) {
    float newAmount;
    cout << "..:: Enter amount to withdraw :: ";
    cin >> newAmount;
    cout << "\n";

    updateFile(searchAccNo, -newAmount);

    // Log the transaction
    fstream transactionFile;
    transactionFile.open("transactions.txt", ios::out | ios::app);
    if (transactionFile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        transactionFile << dt << "\tWithdraw\t" << newAmount << endl;
        transactionFile.close();
    }
    else {
        cout << "Transaction file opening error!\n";
    }
}

void BankAccount::checkInfo(int searchAccNo) {
    if (loadAccount(searchAccNo)) {
        cout << "\n----------------------\n";
        cout << "\t::: Account Number ::: " << accNo << endl;
        cout << "\t::: User Name      ::: " << name << endl;
        cout << "\t::: Father Name    ::: " << Fname << endl;
        cout << "\t::: NID  number    ::: " << NID << endl;
        cout << "\t::: Phone Number   ::: " << P_no << endl;
        cout << "\t::: Email          ::: " << email << endl;
        cout << "\t::: Current amount ::: " << amount << endl;
        cout << "------------------------\n\n";
    }
    else {
        cout << "Account not found!\n";
    }
}

void BankAccount::updateFile(int searchAccNo, float changeAmount) {
    fstream file1;
    bool accountUpdated = false;

    file.open("data.txt", ios::in);
    file1.open("data_temp.txt", ios::out);

    if (file.is_open() && file1.is_open()) {
        while (file >> accNo >> ws && getline(file, name, '\t') && getline(file, Fname, '\t') && getline(file, NID, '\t') && getline(file, P_no, '\t') && getline(file, email, '\t') && file >> password >> amount) {
            if (accNo == searchAccNo) {
                amount += changeAmount;
                cout << "\n\t\t===> Updated amount :: " << amount << endl;
                accountUpdated = true;
            }
            file1 << accNo << "\t" << name << "\t" << Fname << "\t" << NID << "\t"
                << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
        }
        file.close();
        file1.close();

        if (accountUpdated) {
            remove("data.txt");
            rename("data_temp.txt", "data.txt");
        }
        else {
            remove("data_temp.txt");
            cout << "Account not found!\n";
        }
    }
    else {
        cout << "File opening error!\n";
    }
}

bool BankAccount::loadAccount(int searchAccNo) {
    file.open("data.txt", ios::in);
    if (file.is_open()) {
        while (file >> accNo >> ws && getline(file, name, '\t') && getline(file, Fname, '\t') && getline(file, NID, '\t') && getline(file, P_no, '\t') && getline(file, email, '\t') && file >> password >> amount) {
            if (accNo == searchAccNo) {
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

void BankAccount::saveAccount() {
    file.open("data_temp.txt", ios::out);
    if (file.is_open()) {
        file << accNo << "\t" << name << "\t" << Fname << "\t" << NID << "\t"
            << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
        file.close();
    }
    else {
        cout << "File opening error!\n";
    }
    remove("data.txt");
    rename("data_temp.txt", "data.txt");
}

void BankAccount::updateAccountInfo(int searchAccNo) {
    if (loadAccount(searchAccNo)) {
        string newPhone, newEmail;
        cout << "..:: Enter new phone number (Press enter for no change): ";
        cin.ignore(); // Ignore any leftover newline character
        getline(cin, newPhone);
        if (!newPhone.empty()) {
            P_no = newPhone;
        }
        cout << "..:: Enter new email (Press enter for no change): ";
        getline(cin, newEmail);
        if (!newEmail.empty()) {
            email = newEmail;
        }
        saveAccount();
        cout << "\n\t\t ===>Account information updated successfully.\n";
    }
    else {
        cout << "\n ===> Account not found!\n";
    }
}

void Admin::viewAllAccounts() {
    file.open("data.txt", ios::in);
    if (file.is_open()) {
        cout << "\t\t\t\t\t.......:::All Accounts:::.......\n\n";
        cout << "AccNo\t\tName\t\tFname\t\tNID\t\tPhone\t\tEmail\t\tAmount\n";
        cout << "-------------------------------------------------------------------------------------------------------\n";
        while (file >> accNo >> ws && getline(file, name, '\t') && getline(file, Fname, '\t') && getline(file, NID, '\t') && getline(file, P_no, '\t') && getline(file, email, '\t') && file >> password >> amount) {
            cout << accNo << "\t" << name << "\t" << Fname << "\t" << NID << "\t"
                << P_no << "\t" << email << "\t" << amount << "\n";
        }
        file.close();
    }
    else {
        cout << "File opening error!\n";
    }
}

void Admin::resetPassword(int accNo, const string& newPassword) {
    fstream file1;
    bool accountUpdated = false;

    file.open("data.txt", ios::in);
    file1.open("data_temp.txt", ios::out);

    if (file.is_open() && file1.is_open()) {
        while (file >> this->accNo >> ws && getline(file, name, '\t') && getline(file, Fname, '\t') && getline(file, NID, '\t') && getline(file, P_no, '\t') && getline(file, email, '\t') && file >> password >> amount) {
            if (this->accNo == accNo) {
                password = newPassword;
                accountUpdated = true;
            }
            file1 << this->accNo << "\t" << name << "\t" << Fname << "\t" << NID << "\t"
                << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
        }
        file.close();
        file1.close();

        if (accountUpdated) {
            remove("data.txt");
            rename("data_temp.txt", "data.txt");
        }
        else {
            remove("data_temp.txt");
            cout << "\n\t\t ===> Account not found!\n";
        }
    }
    else {
        cout << "\n\t\t ===> File opening error!\n";
    }
}

bool Admin::login(const string& enteredUsername, const string& enteredPassword) {
    return enteredUsername == "#admin#" && enteredPassword == "#admin#";
}

void Admin::deleteAccount(int accNo) {
    fstream file1;
    bool accountDeleted = false;

    file.open("data.txt", ios::in);
    file1.open("data_temp.txt", ios::out);

    if (file.is_open() && file1.is_open()) {
        while (file >> this->accNo >> ws && getline(file, name, '\t') && getline(file, Fname, '\t') && getline(file, NID, '\t') && getline(file, P_no, '\t') && getline(file, email, '\t') && file >> password >> amount) {
            if (this->accNo == accNo) {
                accountDeleted = true;
                cout << "\n\t\t ==> Account " << accNo << " deleted successfully.\n";
            }
            else {
                file1 << this->accNo << "\t" << name << "\t" << Fname << "\t" << NID << "\t"
                    << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
            }
        }
        file.close();
        file1.close();

        if (accountDeleted) {
            remove("data.txt");
            rename("data_temp.txt", "data.txt");
        }
        else {
            remove("data_temp.txt");
            cout << "Account not found!\n";
        }
    }
    else {
        cout << "File opening error!\n";
    }
}