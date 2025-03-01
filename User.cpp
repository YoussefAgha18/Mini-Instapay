#include <iostream>
#include <iomanip> 
#include "User.h"
#include <thread> // For sleep functionality
#include <chrono>  // For sleep duration
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()

bool registerUser(User users[], int& userCount, const int& maxUsers) {
    string contactInformation, email;
    User newUser;

    if (userCount >= maxUsers) {
        cout << "User registration limit reached!" << endl;
        return false;
    }

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, newUser.name);

    cout << "Enter your email: ";
    cin >> email;

    // Check if the email is already registered
    for (int i = 0; i < userCount; i++) {
        if (users[i].email == email) {
            cout << "Registration failed! Email used by another user." << endl;
            return false;
        }
    }

    // Generate a dummy OTP
    srand(time(0)); // Seed the random number generator
    int generatedOTP = rand() % 9000 + 1000; // Generate a random 4-digit number
    cout << "A one-time password (OTP) has been sent to your email: " << generatedOTP << endl;

    int enteredOTP;
    cout << "Enter the OTP to verify your email: ";
    cin >> enteredOTP;

    // Verify the entered OTP
    if (enteredOTP != generatedOTP) {
        cout << "Verification failed! Incorrect OTP." << endl;
        return false;
    }

    newUser.email = email;

    cout << "Enter your password: ";
    cin >> newUser.password;

    cout << "Enter your phone number: ";
    cin >> contactInformation;

    // Check if the phone number is already registered
    for (int i = 0; i < userCount; i++) {
        if (users[i].contactInformation == contactInformation) {
            cout << "Registration failed! Phone number used by another user." << endl;
            return false;
        }
    }

    newUser.contactInformation = contactInformation;
    newUser.id = userCount;
    users[userCount] = newUser;
    userCount++;

    cout << "Registration successful! Your User ID is " << newUser.id << endl;
    return true;
}

int loginUser(User users[], const int userCount) {
    string email, password;
    while (true) {
        cout << "Enter your email: ";
        cin >> email;
        cout << "Enter your password: ";
        cin >> password;

        for (int i = 0; i < userCount; i++) {
            if (users[i].email == email && users[i].password == password) {
                cout << "Login successful! Welcome, " << users[i].name << "!" << endl;
                return users[i].id; // Return the User ID for further actions
            }
        }

        cout << "Invalid email or password!" << endl;
    }
    return -1; // Return -1 for failed login
}


bool addBankAccount(User& user, User users[], const int& userCount) {
    int accountNumber;
    Account newAccount;

    if (user.accountCount >= 10) {
        cout << "You cannot link more than 10 bank accounts!" << endl;
        return false;
    }

    cout << "Enter the bank account number to link: ";
    cin >> accountNumber;
    cin.ignore();


    for (int i = 0; i < userCount; i++) {
        // Check for uniqueness
        for (int j = 0; j < users[i].accountCount; j++) {
            if (users[i].linkedBankAccounts[j].id == accountNumber) {
                cout << "This bank account number is already linked!" << endl;
                return false;
            }
        }
    }

    newAccount = { accountNumber };

    cout << "Enter new name: ";
    getline(cin, newAccount.name);

    user.linkedBankAccounts[user.accountCount] = newAccount;
    user.accountCount++;

    // Simulate validation with a 3-second sleep
    cout << "Validating with the bank..." << endl;
    this_thread::sleep_for(chrono::seconds(3));


    cout << "Bank account linked successfully!" << endl;
    return true;
}



bool removeBankAccount(User& user) {
    int accountNumber, accountNumInput;

    if (user.accountCount == 0) {
        cout << "No linked bank accounts to remove!" << endl;
        return false;
    }

    viewLinkedAccounts(user);

    cout << "Choose the Account No. you want to delete: ";

    cin >> accountNumInput;

    while (accountNumInput <= 0 || accountNumInput > user.accountCount) {
        cout << "Invalid Input!" << endl;
        cout << "choose a number between 1 and " << user.accountCount << endl;
        cout << "Choose the Account you want to send money from: ";
        cin >> accountNumInput;

        // if received a char
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "choose a number between 1 and " << user.accountCount << endl;
            continue;
        }
    }

    user.linkedBankAccounts[accountNumInput - 1];

    for (int i = accountNumInput - 1; i < user.accountCount - 1; i++) {
        user.linkedBankAccounts[i] = user.linkedBankAccounts[i + 1];
    }
    user.accountCount--;
    cout << "Bank account removed successfully!" << endl;
    return true;
}


void viewLinkedAccounts(const User& user) {
    if (user.accountCount == 0) {
        cout << "No linked bank accounts!" << endl;
        return;
    }

    cout << left << setw(5) << "No."
        << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(15) << "Balance"
        << endl;

    cout << string(50, '-') << endl; // Separator line

    for (int i = 0; i < user.accountCount; i++) {
        cout << left << setw(5) << i + 1
            << setw(10) << user.linkedBankAccounts[i].id
            << setw(20) << user.linkedBankAccounts[i].name
            << setw(15) << fixed << setprecision(2) << user.linkedBankAccounts[i].balance
            << endl;
    }
}


void updatePersonalInformation(User& user) {
    int choice;

    cout << "\nUpdate Personal Information:" << endl;
    cout << "1. Name" << endl;
    cout << "2. Contact Information" << endl;
    cout << "3. Bank Accounts" << endl;

    cout << "Enter your choice: ";
    cin >> choice;

    cin.ignore(); // Clear newline from input buffer

    if (choice == 1) {
        cout << "Enter new name: ";
        getline(cin, user.name);
        cout << "Name updated successfully!" << endl;
    }
    else if (choice == 2) {
        cout << "Enter new contact information: ";
        getline(cin, user.contactInformation);
        cout << "Contact information updated successfully!" << endl;
    }
    else if (choice == 3) {
        if (user.accountCount == 0) {
            cout << "No linked bank accounts to update!" << endl;
            return;
        }

        cout << "\nLinked Bank Accounts:" << endl;
        viewLinkedAccounts(user);

        int accountChoice;
        cout << "Choose the account number you want to update (1-" << user.accountCount << "): ";
        cin >> accountChoice;

        // Validate input
        while (accountChoice <= 0 || accountChoice > user.accountCount || cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(); // Ignore invalid input
            cout << "Invalid input! Choose a number between 1 and " << user.accountCount << ": ";
            cin >> accountChoice;
        }

        cin.ignore(); // Clear newline from input buffer

        cout << "Enter new name for the account: ";
        getline(cin, user.linkedBankAccounts[accountChoice - 1].name);
        cout << "Account name updated successfully!" << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
    }
}


void showNotifications(User& user) {
    cout << "Notifications: " << endl;
    if (user.notificationCount == 0) {
        cout << "No Notifications!" << endl;
        return;
    }
    for (int i = 0; i < user.notificationCount; i++) {
        cout << i + 1 << " - " << user.notifications[i] << endl;
    }
    user.notificationCount = 0;
}

void showWalletBalance(const User& currentUser) {
    cout << "Wallet Balance: " << currentUser.phoneWalletBalance << endl;
};