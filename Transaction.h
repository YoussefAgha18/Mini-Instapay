/****************************************************************
 * File: Transaction.h
 * -------------------
 * This file contains the definitions for transaction handling in
 * the Mini-InstaPay application. It includes structures and functions
 * for managing money transfers between accounts and wallets.
 *
 * Mini-InstaPay Application
 * Development Team:
 * - Ahmed Baher
 * - Mostafa Essam
 * - Yousef Agha
 * - Omar Osama
 * - Mostafa Ahmed
 *
 * Last modified: [26/12/2024]
 ****************************************************************/

#pragma once

#include <string>
#include <ctime>
#include "User.h"

using namespace std;

// Transaction status enumeration
enum Status {
    Successful,  // Transaction completed successfully
    Pending,     // Transaction is scheduled or waiting
    Failed       // Transaction failed to complete
};

// Structure to store transaction information
struct Transaction {
    int id;                 // Unique transaction identifier
    int senderAccount;      // Sender's bank account ID (-1 for wallet)
    int receiverAccount;    // Receiver's bank account ID (-1 for wallet)
    string senderPhone;     // Sender's phone number (for wallet transfers)
    string receiverPhone;   // Receiver's phone number (for wallet transfers)
    double amount;          // Transaction amount
    string date;           // Transaction date
    Status status;         // Current status of the transaction
    time_t startTime;      // Time when transaction was initiated
    int delayInSeconds;    // Delay before executing scheduled transaction
};

// Core transaction functions
bool sendMoney(Transaction transactions[], int& transactionCount, const int& maxTransactions,
    const int& senderId, User users[], const int& userCount);

// Transaction history and monitoring
void viewTransactionHistory(const Transaction transactions[], const int& transactionCount, const User& user);
void checkFutureTransactions(Transaction transactions[], const int& transactionCount, User users[], const int& userCount);
void printTransaction(Transaction transaction);