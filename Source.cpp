#include <iostream>
#include "User.h"
#include "Transaction.h"
#include "Admin.h"

using namespace std;

int main() {

    const int maxUsers = 100;
    User users[maxUsers];
    int userCount = 0;

    const int maxTransactions = 1000;
    Transaction transactions[maxTransactions];
    int transactionCount = 0;

    const int maxAdmins = 10;
    Admin admins[maxAdmins];
    int adminCount = 1;

    // Populate system with test users and transactions
// Add test users
    users[0] = { 0, "John Smith", "john@email.com", "pass123", "1234567890" };
    users[0].phoneWalletBalance = 1000.0;  // Set wallet balance separately
    users[0].linkedBankAccounts[0] = { 1001, "Savings", 5000.0 };
    users[0].linkedBankAccounts[1] = { 1002, "Checking", 3000.0 };
    users[0].accountCount = 2;

    users[1] = { 1, "Sarah Johnson", "sarah@email.com", "pass456", "2345678901" };
    users[1].phoneWalletBalance = 1500.0;
    users[1].linkedBankAccounts[0] = { 2001, "Main Account", 7000.0 };
    users[1].accountCount = 1;

    users[2] = { 2, "Mike Brown", "mike@email.com", "pass789", "3456789012" };
    users[2].phoneWalletBalance = 2000.0;
    users[2].linkedBankAccounts[0] = { 3001, "Personal", 10000.0 };
    users[2].linkedBankAccounts[1] = { 3002, "Business", 15000.0 };
    users[2].linkedBankAccounts[2] = { 3003, "Savings", 8000.0 };
    users[2].accountCount = 3;

    users[3] = { 3, "Emma Davis", "emma@email.com", "pass321", "4567890123" };
    users[3].phoneWalletBalance = 800.0;
    users[3].linkedBankAccounts[0] = { 4001, "Main", 6000.0 };
    users[3].linkedBankAccounts[1] = { 4002, "Savings", 4000.0 };
    users[3].accountCount = 2;

    users[4] = { 4, "Alex Wilson", "alex@email.com", "pass654", "5678901234" };
    users[4].phoneWalletBalance = 2500.0;
    users[4].linkedBankAccounts[0] = { 5001, "Personal", 9000.0 };
    users[4].accountCount = 1;

    userCount = 5;

    // Add test transactions
    // Format: id, senderAccount, receiverAccount, senderPhone, receiverPhone, amount, date, status

    // Bank account to bank account transactions
    transactions[0] = { 1, 1001, 2001, "", "", 500.0, "2023-10-15", Successful };
    transactions[1] = { 2, 2001, 3001, "", "", 750.0, "2023-10-16", Successful };
    transactions[2] = { 3, 3002, 4001, "", "", 1000.0, "2023-10-17", Successful };
    transactions[3] = { 4, 4002, 5001, "", "", 300.0, "2023-10-18", Failed };
    transactions[4] = { 5, 1002, 3003, "", "", 600.0, "2023-10-19", Successful };

    // Phone wallet to bank account transactions
    transactions[5] = { 6, -1, 2001, "1234567890", "", 200.0, "2023-11-01", Successful };
    transactions[6] = { 7, -1, 3001, "2345678901", "", 300.0, "2023-11-02", Failed };
    transactions[7] = { 8, -1, 4001, "3456789012", "", 400.0, "2023-11-03", Successful };

    // Bank account to phone wallet transactions
    transactions[8] = { 9, 1001, -1, "", "4567890123", 250.0, "2023-11-15", Successful };
    transactions[9] = { 10, 2001, -1, "", "5678901234", 350.0, "2023-11-16", Successful };
    transactions[10] = { 11, 3001, -1, "", "1234567890", 450.0, "2023-11-17", Failed };

    // Phone wallet to phone wallet transactions
    transactions[11] = { 12, -1, -1, "1234567890", "2345678901", 100.0, "2023-12-01", Successful };
    transactions[12] = { 13, -1, -1, "2345678901", "3456789012", 150.0, "2023-12-02", Successful };
    transactions[13] = { 14, -1, -1, "3456789012", "4567890123", 200.0, "2023-12-03", Successful };
    transactions[14] = { 15, -1, -1, "4567890123", "5678901234", 250.0, "2023-12-04", Failed };

    // More recent transactions for testing period filtering
    transactions[15] = { 16, 1001, 2001, "", "", 800.0, "2024-01-05", Successful };
    transactions[16] = { 17, 3001, 4001, "", "", 900.0, "2024-01-10", Successful };
    transactions[17] = { 18, -1, -1, "1234567890", "3456789012", 300.0, "2024-01-15", Successful };
    transactions[18] = { 19, 5001, -1, "", "2345678901", 400.0, "2024-01-20", Pending };
    transactions[19] = { 20, -1, 3002, "4567890123", "", 550.0, "2024-01-25", Successful };

    // Transactions from different months for monthly analysis
    transactions[20] = { 21, 1002, 3001, "", "", 650.0, "2023-09-15", Successful };
    transactions[21] = { 22, 2001, 4002, "", "", 725.0, "2023-08-20", Successful };
    transactions[22] = { 23, 3003, 5001, "", "", 825.0, "2023-07-25", Successful };
    transactions[23] = { 24, 4001, 1001, "", "", 925.0, "2023-06-30", Successful };
    transactions[24] = { 25, 5001, 2001, "", "", 1100.0, "2023-05-05", Failed };

    transactionCount = 25;
     //Initialize default admin
    admins[0] = { 0, "Main Admin", "admin@bank.com", "admin123", FULL_ACCESS };

    int currentUserId = -1; // current user that is using the system  (logged user)

    while (true) {
        cout << "--------------------------------------------------------------------------------"<<endl ;
        cout << "\nWelcome to Online Banking System\n";
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "1. Register\n2. Login\n3. Login as Admin\n4. Exit\nChoose an option: ";
        int choice;
        cin >> choice;

        // if received a char
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid input! Please enter a number between 1 and 4." << endl;
            continue;
        }

        if (choice == 1) {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "-----------------Register Page-------------------" << endl;
            registerUser(users, userCount, maxUsers);
        }
        else if (choice == 2) {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "-----------------Login Page-------------------" << endl;
            currentUserId = loginUser(users, userCount);
            if (currentUserId != -1) {
                User& currentUser = users[currentUserId];

                // Check if user is suspended
                if (currentUser.isSuspended) {
                    cout << "Your account is suspended. Please contact admin for support." << endl;
                    continue;
                }

                while (true) {
                    cout << "--------------------------------------------------------------------------------" << endl;
                    showNotifications(currentUser);
                    cout << "--------------------------------------------------------------------------------" << endl;
                    cout << "-----------------User Page-------------------" << endl;
                    cout << "----------------- welcome "<<currentUser.name<<" -------------------" << endl;
                    cout << "\nUser Menu:\n"
                        << "1. Add Bank Account\n"
                        << "2. Remove Bank Account\n"
                        << "3. View Linked Accounts\n"
                        << "4. Update Personal Info\n"
                        << "5. Send Money\n"
                        << "6. View Transaction History\n"
                        << "7. show Phone Balance\n"
                        << "8. Logout\n"
                        << "Choose an option: ";
                    cin >> choice;
                    checkFutureTransactions(transactions, transactionCount, users, userCount);
                    // if received a char
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(); // Ignore the invalid input
                        cout << "Invalid input! Please enter a number between 1 and 7." << endl;
                        continue;
                    }

                    if (choice == 1) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------adding Bank Account-------------------" << endl;
                        addBankAccount(currentUser, users, userCount);
                    }
                    else if (choice == 2) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Removing Bank Account-------------------" << endl;
                        removeBankAccount(currentUser);
                    }
                    else if (choice == 3) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------view Accounts-------------------" << endl;
                        viewLinkedAccounts(currentUser);
                    }
                    else if (choice == 4) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------personal info update-------------------" << endl;
                        updatePersonalInformation(currentUser);
                    }
                    else if (choice == 5) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Sending Money-------------------" << endl;
                        sendMoney(transactions, transactionCount, maxTransactions, currentUserId, users, userCount);
                    }
                    else if (choice == 6) {
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------View Transactions-------------------" << endl;
                        viewTransactionHistory(transactions, transactionCount, currentUser);
                    } 
                    else if (choice == 7){
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Wallet Balance-------------------" << endl;
                        showWalletBalance(currentUser);
                    }
                    else if (choice == 8) {
                        cout << "Logged out successfully!" << endl;
                        currentUserId = -1;
                        break;
                    }
                    else {
                        cout << "Invalid input! Please enter a number between 1 and 7." << endl;
                    }
                }
            }
        }
        else if (choice == 3) {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "----------------- Login as Admin -------------------" << endl;
            int currentAdmin = loginAdmin(admins, adminCount);
            if (currentAdmin != -1) {
                while (true) {
                    cout << "--------------------------------------------------------------------------------" << endl;
                    cout << "-----------------Welcome Admin "<<admins[currentAdmin].name<<" -------------------" << endl;
                    cout << "\nAdmin Dashboard:\n"
                        << "1. View All Users\n"
                        << "2. View All Transactions\n"
                        << "3. Create New Admin\n"
                        << "4. Suspend User\n"
                        << "5. Unsuspend User\n"
                        << "6. Generate Transaction Report\n"
                        << "7. View User Profile\n"
                        << "8. View User Activity\n"
                        << "9. View User Activity by Period\n"
                        << "10. Logout\n"
                        << "Choose an option: ";

                    int adminChoice;
                    cin >> adminChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid input! Please enter a number." << endl;
                        continue;
                    }

                    switch (adminChoice) {
                    case 1:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------All Users -------------------" << endl;
                        viewAllUsers(users, userCount);
                        break;
                    case 2:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "----------------- All Transactions -------------------" << endl;
                        viewAllTransactions(transactions, transactionCount);
                        break;
                    case 3:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Admin Creation-------------------" << endl;
                        createAdmin(admins, adminCount, maxAdmins, admins[currentAdmin]);
                        break;
                    case 4:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Suspending Users-------------------" << endl;
                        suspendUser(users, userCount);
                        break;
                    case 5:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Unsuspending Users-------------------" << endl;
                        unsuspendUser(users, userCount);
                        break;
                    case 6:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Transaction Report-------------------" << endl;
                        generateTransactionReport(transactions, transactionCount);
                        break;
                    case 7:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------View User Porfile-------------------" << endl;
                        viewUserProfile(users, userCount);
                        break;
                    case 8:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------User Activity Analysis-------------------" << endl;
                        viewUserActivity(transactions, transactionCount, users, userCount);
                        break;
                    case 9:
                        cout << "--------------------------------------------------------------------------------" << endl;
                        cout << "-----------------Periodic User Activity Analysis-------------------" << endl;
                        viewUserActivityByPeriod(transactions, transactionCount, users, userCount);
                        break;
                    case 10:
                        cout << "Logged out from admin dashboard." << endl;
                        goto adminLogout;
                    default:
                        cout << "Invalid choice!" << endl;
                    }
                }
            adminLogout:;
            }
        }
        else if (choice == 4) {
            cout << "Thank you for using our Online Banking System. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid input! Please enter a number between 1 and 4." << endl;
        }
    }

    return 0;
}