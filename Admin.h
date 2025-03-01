/****************************************************************
 * File: Admin.h
 * ---------------
 * This file contains the definitions for the administrative
 * functionality of the Mini-InstaPay application. It includes
 * structures and function declarations for admin management,
 * user monitoring, and system reporting.
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

#pragma once    // Admin.h

#include <string>
#include "User.h"
#include "Transaction.h"

using namespace std;

// Admin permission levels
enum AdminPermission {
    VIEW_ONLY,      // Can only view users and transactions
    FULL_ACCESS     // Can manage users, transactions, and create other admins
};

// Structure to store admin information
struct Admin {
    int id;                 // Unique identifier for admin
    string name;           // Admin's full name
    string email;          // Admin's email address (used for login)
    string password;       // Admin's password
    AdminPermission permission;  // Admin's permission level
};

// Admin authentication functions
int loginAdmin(Admin admins[], const int adminCount);
void createAdmin(Admin admins[], int& adminCount, const int maxAdmins, const Admin& currentAdmin);

// User management functions
void viewAllUsers(const User users[], const int userCount);
void viewUserProfile(const User users[], const int userCount);
void suspendUser(User users[], const int userCount);
void unsuspendUser(User users[], const int userCount);

// Transaction management functions
void viewAllTransactions(const Transaction transactions[], const int transactionCount);
void generateTransactionReport(const Transaction transactions[], const int transactionCount);

// User activity analysis functions
void viewUserActivity(const Transaction transactions[], const int transactionCount,
    const User users[], const int userCount);
void viewUserActivityByPeriod(const Transaction transactions[], const int transactionCount,
    const User users[], const int userCount);