#include <iostream>
#include <ctime> 
#include <string>
#include <cstdio> // For snprintf
#include "Transaction.h"

// Helper Function to get status string from enum 
string getStatusString(Status status) {
    switch (status) {
    case Successful:
        return "Successful";
    case Failed:
        return "Failed";
    case Pending:
        return "Pending";
    default:
        return "Unknown";
    }
}

string getCurrentDate() {
    time_t now = time(0);           // Get current time
    tm localTime;                   // Declare a tm structure for the local time
    localtime_s(&localTime, &now);  // Use localtime_s for safety

    // Format the date as YYYY-MM-DD
    char date[11];
    snprintf(date, sizeof(date), "%04d-%02d-%02d",
        1900 + localTime.tm_year,
        1 + localTime.tm_mon,
        localTime.tm_mday);

    return string(date);
}

double readAmount() {
    double amount;
    cout << "Enter the amount to send: ";
    cin >> amount;

    while (amount <= 0) {
        cout << "Invalid amount! Enter an amount greater than 0: ";
        cin >> amount;
        // if received a char
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid amount! Enter an amount greater than 0: ";
            continue;
        }
    }
    return amount;
}

// Choose which account to use for the transaction
int chooseAccount(const User& sender) {
    viewLinkedAccounts(sender);
    cout << "Choose the Account No. you want to send money from: ";
    int accountNumInput;
    cin >> accountNumInput;

    while (accountNumInput <= 0 || accountNumInput > sender.accountCount) {
        cout << "Invalid Input! Choose a number between 1 and " << sender.accountCount << ": ";
        cin >> accountNumInput;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid Input! Choose a number between 1 and " << sender.accountCount << ": ";
        }
    }

    return accountNumInput;
}

bool accountToWallet(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {

    User& sender = users[senderId];
    int accountNumInput, senderAccountId;
    double senderBalance;
    string receiverPhoneNumber;

    if (sender.accountCount == 0) {
        cout << "You have no linked bank accounts. Please link a bank account first." << endl;
        return false;
    }

    accountNumInput = chooseAccount(sender);
    senderBalance = sender.linkedBankAccounts[accountNumInput - 1].balance;
    senderAccountId = sender.linkedBankAccounts[accountNumInput - 1].id;

    cout << "Enter recipient's phone number: ";
    cin >> receiverPhoneNumber;


    double amount = readAmount();

    if (amount > senderBalance) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        -1,
        "",
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Insufficient funds." << endl;

        return false;
    }

    bool receiverFound = false;
    for (int i = 0; i < userCount; i++) {
        if (users[i].contactInformation == receiverPhoneNumber) {
            if (executionType == 1) {
                users[i].phoneWalletBalance += amount;
                sender.linkedBankAccounts[accountNumInput - 1].balance -= amount;
                users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " sent from Account " + to_string(senderAccountId);
                users[i].notificationCount += 1;
            }
            else {
                users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " pending from Account " + to_string(senderAccountId);
                users[i].notificationCount += 1;
            }
            receiverFound = true;
            break;
        }

    }

    if (!receiverFound) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        -1,
        "",
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Recipient not found." << endl;

        return false;

    }

    // Record transaction
    Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        -1,
        "",
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        executionType == 1 ? Successful : Pending,
        time(nullptr),
        delayInSeconds
    };

    transactions[transactionCount] = newTransaction;
    transactionCount++;

    cout << "Transaction successful! Amount $" << amount << " sent to Phone number"
        << receiverPhoneNumber << endl;
    return true;

}

bool accountToAccount(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {

    User& sender = users[senderId];
    int accountNumInput, senderAccountId, receiverAccountId;
    double senderBalance;

    if (sender.accountCount == 0) {
        cout << "You have no linked bank accounts. Please link a bank account first." << endl;
        return false;
    }

    accountNumInput = chooseAccount(sender);
    senderBalance = sender.linkedBankAccounts[accountNumInput - 1].balance;
    senderAccountId = sender.linkedBankAccounts[accountNumInput - 1].id;

    cout << "Enter recipient's account number: ";
    cin >> receiverAccountId;

    if (receiverAccountId == senderAccountId) {
        cout << "You can't send money to the same account" << endl;
        return false;
    }


    double amount = readAmount();

    if (amount > senderBalance) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        receiverAccountId,
        "",
        "",
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Insufficient funds." << endl;

        return false;
    }

    bool receiverFound = false;
    for (int i = 0; i < userCount && !receiverFound; i++) {
        for (int j = 0; j < users[i].accountCount; j++) {
            if (users[i].linkedBankAccounts[j].id == receiverAccountId) {
                if (executionType == 1) {
                    users[i].linkedBankAccounts[j].balance += amount;
                    sender.linkedBankAccounts[accountNumInput - 1].balance -= amount;
                    users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " sent from Account " + to_string(senderAccountId);
                    users[i].notificationCount += 1;
                }
                else {
                    users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " pending from Account " + to_string(senderAccountId);
                    users[i].notificationCount += 1;
                }
                receiverFound = true;
                break;
            }
        }

    }

    if (!receiverFound) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        receiverAccountId,
        "",
        "",
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Recipient not found." << endl;

        return false;

    }

    // Record transaction
    Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        senderAccountId,
        receiverAccountId,
        "",
        "",
        amount,
        getCurrentDate(),
        executionType == 1 ? Successful : Pending,
        time(nullptr),
        delayInSeconds
    };

    transactions[transactionCount] = newTransaction;
    transactionCount++;

    cout << "Transaction successful! Amount $" << amount << " sent to Account Number "
        << receiverAccountId << endl;
    return true;

}

bool sendMoneyByBankAccount(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {

    short sendMethod;

    do {
        cout << "Send to:\n";
        cout << "1. Phone Wallet\n";
        cout << "2. Bank Account\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> sendMethod;

        if (sendMethod != 1 && sendMethod != 2) {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }
        // if received a char
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid amount! Enter an amount greater than 0: ";
            continue;
        }
    } while (sendMethod != 1 && sendMethod != 2);


    if (sendMethod == 1) {
        return accountToWallet(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);

    }
    else {
        return accountToAccount(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);
    }
}

bool walletToAccount(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {

    User& sender = users[senderId];
    int receiverAccountId;
    double senderBalance = sender.phoneWalletBalance;
    string senderPhoneNumber = sender.contactInformation;


    cout << "Enter recipient's account number: ";
    cin >> receiverAccountId;


    double amount = readAmount();

    if (amount > senderBalance) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1,
        receiverAccountId,
        senderPhoneNumber,
        "",
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Insufficient funds." << endl;

        return false;
    }

    bool receiverFound = false;
    for (int i = 0; i < userCount && !receiverFound; i++) {
        for (int j = 0; j < users[i].accountCount; j++) {
            if (users[i].linkedBankAccounts[j].id == receiverAccountId) {
                if (executionType == 1) {
                    users[i].linkedBankAccounts[j].balance += amount;
                    sender.phoneWalletBalance -= amount;
                    users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " sent from Phone Number " + senderPhoneNumber;
                    users[i].notificationCount += 1;
                }
                else {
                    users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " pending from Account " + senderPhoneNumber;
                    users[i].notificationCount += 1;
                }

                receiverFound = true;
                break;
            }
        }

    }

    if (!receiverFound) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1,
        receiverAccountId,
        senderPhoneNumber,
        "",
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr),
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Recipient not found." << endl;

        return false;

    }

    // Record transaction
    Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1,
        receiverAccountId,
        senderPhoneNumber,
        "",
        amount,
        getCurrentDate(),
        executionType == 1 ? Successful : Pending,
        time(nullptr),
        delayInSeconds
    };

    transactions[transactionCount] = newTransaction;
    transactionCount++;

    cout << "Transaction successful! Amount $" << amount << " sent to account number"
        << receiverAccountId << endl;
    return true;

}

bool walletToWallet(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {
    User& sender = users[senderId];
    double senderBalance = sender.phoneWalletBalance;
    string receiverPhoneNumber;

    cout << "Enter recipient's phone number: ";
    cin >> receiverPhoneNumber;

    if (receiverPhoneNumber == sender.contactInformation) {
        cout << "You can't send money to same phone number";
        return false;
    }


    double amount = readAmount();

    if (amount > senderBalance) {
        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1, // Sender's account
        -1,  // receiver's account
        sender.contactInformation,
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr), // Current time in seconds,
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Insufficient funds." << endl;
        return false;
    }

    bool receiverFound = false;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == senderId) continue;

        if (users[i].contactInformation == receiverPhoneNumber) {
            if (executionType == 1) {
                users[i].phoneWalletBalance += amount;
                sender.phoneWalletBalance -= amount;
                users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " sent from Phone Number " + sender.contactInformation;
                users[i].notificationCount += 1;
            }
            else {
                users[i].notifications[users[i].notificationCount] = "Amount $" + to_string(int(amount)) + " pending from Account " + sender.contactInformation;
                users[i].notificationCount += 1;
            }
            receiverFound = true;
            break;
        }

    }

    if (!receiverFound) {

        Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1, // Sender's account
        -1,  // receiver's account
        sender.contactInformation,
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        Failed,
        time(nullptr), // Current time in seconds,
        delayInSeconds
        };

        transactions[transactionCount] = newTransaction;
        transactionCount++;

        cout << "Transaction failed! Recipient not found." << endl;
        return false;
    }

    Transaction newTransaction = {
        transactionCount + 1, // Unique Transaction ID
        -1, // Sender's account
        -1, // Receiver's account
        sender.contactInformation,
        receiverPhoneNumber,
        amount,
        getCurrentDate(),
        executionType == 1 ? Successful : Pending,
        time(nullptr),
        delayInSeconds

    };

    transactions[transactionCount] = newTransaction;
    transactionCount++;

    cout << "Transaction successful! Amount $" << amount << " sent to phone number "
        << receiverPhoneNumber << endl;

    return true;

}

bool sendMoneyByWallet(Transaction transactions[], int& transactionCount,
    const int& senderId, User users[], const int& userCount, const short& executionType, const int& delayInSeconds) {
    User& sender = users[senderId];
    double senderBalance = sender.phoneWalletBalance;

    cout << "Current wallet balance is: " << senderBalance << endl;

    short sendMethod;

    do {
        cout << "Send to:\n";
        cout << "1. Phone Wallet\n";
        cout << "2. Bank Account\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> sendMethod;

        if (sendMethod != 1 && sendMethod != 2) {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }
        // if received a char
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid choice. Please enter 1 or 2." << endl;
            continue;
        }
    } while (sendMethod != 1 && sendMethod != 2);


    if (sendMethod == 1) {
        return walletToWallet(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);

    }
    else {
        return walletToAccount(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);
    }
}

bool sendMoney(Transaction transactions[], int& transactionCount, const int& maxTransactions,
    const int& senderId, User users[], const int& userCount) {
    if (transactionCount >= maxTransactions) {
        cout << "Transaction limit reached!" << endl;
        return false;
    }

    short transferMethod;

    // Prompt the user for the transfer method
    do {
        cout << "How do you want to send money?\n";
        cout << "1. Phone Wallet\n";
        cout << "2. Bank Account\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> transferMethod;

        if (transferMethod != 1 && transferMethod != 2) {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }

        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }
    } while (transferMethod != 1 && transferMethod != 2);

    // Ask if the transaction is to be executed now or scheduled for later
    short executionType;
    int delayInSeconds = 0;

    do {
        cout << "When do you want this transaction to be executed?\n";
        cout << "1. Right now\n";
        cout << "2. Schedule for later\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> executionType;

        if (executionType != 1 && executionType != 2) {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }

        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(); // Ignore the invalid input
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }
    } while (executionType != 1 && executionType != 2);

    // If scheduled, ask for the delay in seconds
    if (executionType == 2) {
        do {
            cout << "Enter the number of seconds to wait before executing the transaction: ";
            cin >> delayInSeconds;

            if (delayInSeconds <= 0) {
                cout << "Invalid input! The delay should be a positive number." << endl;
            }

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                cout << "Invalid input! Please enter a valid number." << endl;
            }
        } while (delayInSeconds <= 0 || cin.fail());
    }

    // Store the schedule details into variables
    cout << "Transaction will ";
    if (executionType == 1) {
        cout << "execute immediately." << endl;
    }
    else {
        cout << "be scheduled to execute after " << delayInSeconds << " seconds." << endl;
    }

    // Delegate to the specific transfer method function
    if (transferMethod == 1) {
        return sendMoneyByWallet(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);
    }
    else {
        return sendMoneyByBankAccount(transactions, transactionCount, senderId, users, userCount, executionType, delayInSeconds);
    }
}

void checkFutureTransactions(Transaction transactions[], const int& transactionCount, User users[], const int& userCount) {
    time_t currentTime = time(nullptr);

    for (int i = 0; i < transactionCount; i++) {
        // Only check transactions with a Pending status
        if (transactions[i].status == Pending) {
            time_t elapsedTime = currentTime - transactions[i].startTime;
            bool completedTransaction = false;

            // Check if the delay time has elapsed
            if (elapsedTime >= transactions[i].delayInSeconds) {
                if (transactions[i].senderAccount == -1) {
                    if (transactions[i].receiverAccount == -1) {
                        // sender Phone, receiver Phone
                        for (int user = 0; user < userCount; user++) {
                            if (users[user].contactInformation == transactions[i].senderPhone) {
                                if (users[user].phoneWalletBalance - transactions[i].amount >= 0) {
                                    users[user].phoneWalletBalance -= transactions[i].amount;
                                    completedTransaction = true;
                                    users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                    users[user].notificationCount++;
                                }
                            }
                            else if (users[user].contactInformation == transactions[i].receiverPhone) {
                                users[user].phoneWalletBalance += transactions[i].amount;
                                users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                users[user].notificationCount++;

                            }
                        }
                    }
                    else {
                        // sender Phone, receiver Account
                        for (int user = 0; user < userCount; user++) {
                            if (users[user].contactInformation == transactions[i].senderPhone) {
                                if (users[user].phoneWalletBalance - transactions[i].amount >= 0) {
                                    users[user].phoneWalletBalance -= transactions[i].amount;
                                    completedTransaction = true;
                                    users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                    users[user].notificationCount++;

                                }
                            }

                            for (int account = 0; account < users[user].accountCount; account++) {
                                if (users[user].linkedBankAccounts[account].id == transactions[i].receiverAccount)
                                    users[user].linkedBankAccounts[account].balance += transactions[i].amount;
                                users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                users[user].notificationCount++;

                            }

                        }
                    }
                }
                else {
                    if (transactions[i].receiverAccount == -1) {
                        // sender Account, receiver Phone
                        for (int user = 0; user < userCount; user++) {
                            if (users[user].contactInformation == transactions[i].receiverPhone) {
                                users[user].phoneWalletBalance += transactions[i].amount;
                                users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                users[user].notificationCount++;
                            }

                            for (int account = 0; account < users[user].accountCount; account++) {
                                if (users[user].linkedBankAccounts[account].id == transactions[i].senderAccount)
                                    if (users[user].linkedBankAccounts[account].balance - transactions[i].amount >= 0) {
                                        users[user].linkedBankAccounts[account].balance -= transactions[i].amount;
                                        completedTransaction = true;
                                        users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                        users[user].notificationCount++;
                                    }
                            }

                        }
                    }
                    else {
                        // sender Account, receiver Account
                        for (int user = 0; user < userCount; user++) {
                            for (int account = 0; account < users[user].accountCount; account++) {
                                if (users[user].linkedBankAccounts[account].id == transactions[i].senderAccount) {
                                    if (users[user].linkedBankAccounts[account].balance - transactions[i].amount >= 0) {
                                        users[user].linkedBankAccounts[account].balance -= transactions[i].amount;
                                        completedTransaction = true;
                                        users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                        users[user].notificationCount++;
                                    }
                                }
                                else if (users[user].linkedBankAccounts[account].id == transactions[i].receiverAccount) {
                                    users[user].linkedBankAccounts[account].balance += transactions[i].amount;
                                    users[user].notifications[users[user].notificationCount] = "Transaction with id: " + to_string(transactions[i].id) + " completed successfully";
                                    users[user].notificationCount++;
                                }
                            }
                        }

                    }
                }



                if (completedTransaction) {
                    // Update transaction status to Successful
                    transactions[i].status = Successful;

                    // Notify the user about the updated status
                    cout << "Transaction ID " << transactions[i].id
                        << " has been successfully executed." << endl;
                }
                else {
                    // Update transaction status to Successful
                    transactions[i].status = Failed;

                    // Notify the user about the updated status
                    cout << "Transaction ID " << transactions[i].id
                        << " Failed! Insufficient funds." << endl;
                }
            }
        }
    }
}

void printTransaction(Transaction transaction) {
    cout << "Transaction ID: " << transaction.id << endl;
    if (transaction.senderAccount == -1)
        cout << "Sender Phone: " << transaction.senderPhone << endl;

    else {
        cout << "Sender Account: " << transaction.senderAccount << endl;
    }
    if (transaction.receiverAccount == -1)
        cout << "Receiver Phone: " << transaction.receiverPhone << endl;
    else
        cout << "Receiver Account: " << transaction.receiverAccount << endl;
    cout << "Amount: $" << transaction.amount << endl;
    cout << "Date: " << transaction.date << endl;
    cout << "Status: " << getStatusString(transaction.status) << endl;
    cout << "Start Time: " << transaction.startTime << endl;
    cout << "Delay: " << transaction.delayInSeconds << endl;
    cout << "----------------------------------------" << endl;
}

void viewTransactionHistory(const Transaction transactions[], const int& transactionCount, const User& user) {
    if (transactionCount == 0) {
        cout << "No transactions found." << endl;
    }
    bool hasTransactions = false;


    int filterChoice;
    cout << "Do you want to filter transactions? \n1. Yes \n2. No\n";
    cin >> filterChoice;

    int filterType = 0;
    double filterAmount = 0.0;
    string filterStatus;
    string filterDate;

    if (filterChoice == 1) {
        cout << "Choose a filter type: \n1. Amount \n2. Status \n3. Date\n";
        cin >> filterType;

        switch (filterType) {
        case 1:
            cout << "Enter the amount to filter by: ";
            cin >> filterAmount;
            break;
        case 2:
            cout << "Enter the status to filter by \n1. Successful \n2. Pending\n3. Failed \n";
            cin.ignore(); // To clear newline from input buffer
            int statusInput;
            cin >> statusInput;
            switch (statusInput) {
            case 1:
                filterStatus = "Successful";
                break;
            case 2:
                filterStatus = "Pending";
                break;
            case 3:
                filterStatus = "Failed";
                break;
            default:
                filterStatus = "";
            }
            break;

        case 3:
            cout << "Enter the date to filter by (e.g., 'YYYY-MM-DD'): ";
            cin.ignore(); // To clear newline from input buffer
            getline(cin, filterDate);
            break;
        default:
            cout << "Invalid filter type. Showing all transactions." << endl;
            filterType = 0; // Reset to show all transactions
            break;
        }
    }
    cout << "\nTransaction History for User: " << user.name << endl;
    cout << "----------------------------------------" << endl;

    for (int i = 0; i < transactionCount; i++) {
        bool matchesFilter = true;

        if (filterChoice == 1) {
            switch (filterType) {
            case 1: // Amount filter
                matchesFilter = (transactions[i].amount == filterAmount);
                break;
            case 2: // Status filter
                matchesFilter = (getStatusString(transactions[i].status) == filterStatus);
                break;
            case 3: // Date filter
                matchesFilter = (transactions[i].date == filterDate);
                break;
            }
        }


        if (matchesFilter &&
            (transactions[i].senderPhone == user.contactInformation || transactions[i].receiverPhone == user.contactInformation)) {
            hasTransactions = true;
            printTransaction(transactions[i]);
            continue;
        }

        for (int j = 0; j < user.accountCount; j++) {
            if (matchesFilter &&
                (transactions[i].senderAccount == user.linkedBankAccounts[j].id ||
                    transactions[i].receiverAccount == user.linkedBankAccounts[j].id)) {
                hasTransactions = true;
                printTransaction(transactions[i]);
            }
        }
    }

    if (!hasTransactions) {
        cout << "No transactions found." << endl;
    }
}
