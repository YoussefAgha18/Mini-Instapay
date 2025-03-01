/****************************************************************
 * File: User.h
 * ------------
 * This file contains the user management functionality for the
 * Mini-InstaPay application. It defines the user structure and
 * related functions for account management and authentication.
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
#include "Account.h"

using namespace std;

// Main user structure containing all user-related information
struct User {
    int id;                             // Unique user identifier
    string name;                        // User's full name
    string email;                       // User's email (used for login)
    string password;                    // User's password
    string contactInformation;          // User's phone number
    Account linkedBankAccounts[10];     // Array of linked bank accounts
    int accountCount = 0;               // Number of linked accounts (max 10)
    bool isSuspended = false;           // User account status
    double phoneWalletBalance = 500;    // Mobile wallet balance
    string notifications[100];          // Array of user notifications
    int notificationCount = 0;          // Number of unread notifications
};

// User authentication functions
bool registerUser(User users[], int& userCount, const int& maxUsers);
int loginUser(User users[], const int userCount);

// Account management functions
bool addBankAccount(User& user, User users[], const int& userCount);
bool removeBankAccount(User& user);
void viewLinkedAccounts(const User& user);
void updatePersonalInformation(User& user);

// Wallet and notification functions
void showNotifications(User& user);
void showWalletBalance(const User& currentUser);