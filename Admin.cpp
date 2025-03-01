#include <iostream>
#include <iomanip>
#include "Admin.h"
#include "Transaction.h"
int loginAdmin(Admin admins[], const int adminCount) {
    string email, password;
    cout << "Enter admin email: ";
    cin >> email;
    cout << "Enter admin password: ";
    cin >> password;

    for (int i = 0; i < adminCount; i++) {
        if (admins[i].email == email && admins[i].password == password) {
            cout << "Admin login successful! Welcome, " << admins[i].name << "!" << endl;
            return admins[i].id;
        }
    }
    cout << "Invalid admin credentials!" << endl;
    return -1;
}

void createAdmin(Admin admins[], int& adminCount, const int maxAdmins, const Admin& currentAdmin) {
    if (currentAdmin.permission != FULL_ACCESS) {
        cout << "You don't have permission to create new admins!" << endl;
        return;
    }

    if (adminCount >= maxAdmins) {
        cout << "Maximum number of admins reached!" << endl;
        return;
    }

    Admin newAdmin;
    cout << "Enter new admin name: ";
    cin.ignore();
    getline(cin, newAdmin.name);

    cout << "Enter new admin email: ";
    cin >> newAdmin.email;

    // Check if email already exists
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].email == newAdmin.email) {
            cout << "An admin with this email already exists!" << endl;
            return;
        }
    }

    cout << "Enter new admin password: ";
    cin >> newAdmin.password;

    newAdmin.id = adminCount + 1;
    newAdmin.permission = VIEW_ONLY;  // New admins get VIEW_ONLY permission by default

    admins[adminCount] = newAdmin;
    adminCount++;

    cout << "New admin created successfully!" << endl;
}

void viewAllUsers(const User users[], const int userCount) {
    if (userCount == 0) {
        cout << "No users registered in the system." << endl;
        return;
    }

    cout << "\nAll Users in System:" << endl;
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(30) << "Email"
        << setw(15) << "Phone" << setw(15) << "Status" << endl;
    cout << string(85, '-') << endl;

    for (int i = 0; i < userCount; i++) {
        cout << setw(5) << users[i].id
            << setw(20) << users[i].name
            << setw(30) << users[i].email
            << setw(15) << users[i].contactInformation
            << setw(15) << (users[i].isSuspended ? "Suspended" : "Active") << endl;
    }
}

void viewAllTransactions(const Transaction transactions[], const int transactionCount) {
    if (transactionCount == 0) {
        cout << "No transactions in the system." << endl;
        return;
    }

    cout << "\nAll Transactions:" << endl;
    for (int i = 0; i < transactionCount; i++) {
        printTransaction(transactions[i]);
    }
}

void suspendUser(User users[], const int userCount) {
    int userId;
    cout << "Enter user ID to suspend: ";
    cin >> userId;

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            if (users[i].isSuspended) {
                cout << "User is already suspended!" << endl;
            }
            else {
                users[i].isSuspended = true;
                cout << "User " << users[i].name << " has been suspended." << endl;
            }
            return;
        }
    }
    cout << "User not found!" << endl;
}

void unsuspendUser(User users[], const int userCount) {
    int userId;
    cout << "Enter user ID to unsuspend: ";
    cin >> userId;

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            if (!users[i].isSuspended) {
                cout << "User is not suspended!" << endl;
            }
            else {
                users[i].isSuspended = false;
                cout << "User " << users[i].name << " has been unsuspended." << endl;
            }
            return;
        }
    }
    cout << "User not found!" << endl;
}

void generateTransactionReport(const Transaction transactions[], const int transactionCount) {
    if (transactionCount == 0) {
        cout << "No transactions to report." << endl;
        return;
    }

    int successfulCount = 0;
    int failedCount = 0;
    int pendingCount = 0;
    double totalAmount = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].status == Successful) {
            successfulCount++;
            totalAmount += transactions[i].amount;
        }
        else if (transactions[i].status == Failed) {
            failedCount++;
        }
        else {
            pendingCount++;
        }
    }

    cout << "\nTransaction Report" << endl;
    cout << "----------------" << endl;
    cout << "Total Transactions: " << transactionCount << endl;
    cout << "Successful Transactions: " << successfulCount << endl;
    cout << "Failed Transactions: " << failedCount << endl;
    cout << "Pending Transactions: " << pendingCount << endl;
    cout << "Total Amount Transferred: $" << fixed << setprecision(2) << totalAmount << endl;
}

void viewUserProfile(const User users[], const int userCount) {
    int userId;
    cout << "Enter user ID to view profile: ";
    cin >> userId;

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            cout << "\nUser Profile:" << endl;
            cout << "ID: " << users[i].id << endl;
            cout << "Name: " << users[i].name << endl;
            cout << "Email: " << users[i].email << endl;
            cout << "Phone: " << users[i].contactInformation << endl;
            cout << "Status: " << (users[i].isSuspended ? "Suspended" : "Active") << endl;
            cout << "Wallet Balance: $" << users[i].phoneWalletBalance << endl;
            cout << "\nLinked Bank Accounts:" << endl;
            viewLinkedAccounts(users[i]);
            return;
        }
    }
    cout << "User not found!" << endl;
}

void analyzeUserTransactions(const Transaction transactions[], const int transactionCount,
    const User& user, const string& startDate = "", const string& endDate = "") {
    int totalTransactions = 0;
    int successfulCount = 0;
    int failedCount = 0;
    int pendingCount = 0;
    double totalReceived = 0;
    double totalSent = 0;

    for (int i = 0; i < transactionCount; i++) {
        // Skip if transaction is outside the date range (if dates are provided)
        if (!startDate.empty() && transactions[i].date < startDate) continue;
        if (!endDate.empty() && transactions[i].date > endDate) continue;

        bool isInvolved = false;
        bool isSender = false;

        // Check if transaction involves user's phone wallet
        if (transactions[i].senderPhone == user.contactInformation) {
            isInvolved = true;
            isSender = true;
        }
        else if (transactions[i].receiverPhone == user.contactInformation) {
            isInvolved = true;
        }

        // Check if transaction involves user's bank accounts
        for (int j = 0; j < user.accountCount; j++) {
            if (transactions[i].senderAccount == user.linkedBankAccounts[j].id) {
                isInvolved = true;
                isSender = true;
                break;
            }
            else if (transactions[i].receiverAccount == user.linkedBankAccounts[j].id) {
                isInvolved = true;
                break;
            }
        }

        if (isInvolved) {
            totalTransactions++;

            if (transactions[i].status == Successful) {
                successfulCount++;
                if (isSender) {
                    totalSent += transactions[i].amount;
                }
                else {
                    totalReceived += transactions[i].amount;
                }
            }
            else if (transactions[i].status == Failed) {
                failedCount++;
            }
            else {
                pendingCount++;
            }
        }
    }

    double accountGrowth = totalReceived - totalSent;

    cout << "\nTransaction Analysis:" << endl;
    cout << "-------------------" << endl;
    cout << "Total Transactions: " << totalTransactions << endl;
    cout << "Successful Transactions: " << successfulCount << endl;
    cout << "Failed Transactions: " << failedCount << endl;
    cout << "Pending Transactions: " << pendingCount << endl;
    cout << fixed << setprecision(2);
    cout << "Total Amount Sent: $" << totalSent << endl;
    cout << "Total Amount Received: $" << totalReceived << endl;
    cout << "Account Growth: $" << accountGrowth << endl;
}

void viewUserActivity(const Transaction transactions[], const int transactionCount,
    const User users[], const int userCount) {
    int userId;
    cout << "Enter user ID to view activity: ";
    cin >> userId;

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            cout << "\nActivity Analysis for User: " << users[i].name << endl;
            cout << "Email: " << users[i].email << endl;
            cout << "Phone: " << users[i].contactInformation << endl;

            analyzeUserTransactions(transactions, transactionCount, users[i]);
            return;
        }
    }
    cout << "User not found!" << endl;
}

void viewUserActivityByPeriod(const Transaction transactions[], const int transactionCount,
    const User users[], const int userCount) {
    int userId;
    cout << "Enter user ID to view activity: ";
    cin >> userId;

    User* selectedUser = nullptr;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            selectedUser = const_cast<User*>(&users[i]);
            break;
        }
    }

    if (!selectedUser) {
        cout << "User not found!" << endl;
        return;
    }

    cout << "\nSelect period type:\n";
    cout << "1. Monthly\n";
    cout << "2. Annual\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    if (cin.fail() || (choice != 1 && choice != 2)) {
        cin.clear();
        cin.ignore();
        cout << "Invalid choice!" << endl;
        return;
    }

    string startDate, endDate;
    if (choice == 1) {  // Monthly
        int year, month;
        cout << "Enter year (YYYY): ";
        cin >> year;
        cout << "Enter month (1-12): ";
        cin >> month;

        if (cin.fail() || month < 1 || month > 12 || year < 1900) {
            cout << "Invalid date input!" << endl;
            return;
        }

        // Format dates as strings
        char startDateStr[11], endDateStr[11];
        snprintf(startDateStr, sizeof(startDateStr), "%04d-%02d-01", year, month);

        // Calculate end date (first day of next month)
        if (month == 12) {
            snprintf(endDateStr, sizeof(endDateStr), "%04d-01-01", year + 1);
        }
        else {
            snprintf(endDateStr, sizeof(endDateStr), "%04d-%02d-01", year, month + 1);
        }

        startDate = string(startDateStr);
        endDate = string(endDateStr);

        cout << "\nActivity Analysis for " << selectedUser->name << endl;
        cout << "Period: " << year << "-" << month << endl;
    }
    else {  // Annual
        int year;
        cout << "Enter year (YYYY): ";
        cin >> year;

        if (cin.fail() || year < 1900) {
            cout << "Invalid year input!" << endl;
            return;
        }

        char startDateStr[11], endDateStr[11];
        snprintf(startDateStr, sizeof(startDateStr), "%04d-01-01", year);
        snprintf(endDateStr, sizeof(endDateStr), "%04d-01-01", year + 1);

        startDate = string(startDateStr);
        endDate = string(endDateStr);

        cout << "\nActivity Analysis for " << selectedUser->name << endl;
        cout << "Period: " << year << endl;
    }

    analyzeUserTransactions(transactions, transactionCount, *selectedUser, startDate, endDate);
}