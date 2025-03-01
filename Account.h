/****************************************************************
 * File: Account.h
 * --------------
 * This file defines the bank account structure used in the
 * Mini-InstaPay application. It provides the basic structure for
 * storing bank account information.
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

using namespace std;

// Basic structure for bank account information
struct Account {
    int id;                 // Unique account identifier
    string name;           // Account name/label
    double balance = 500;  // Account balance with default initial value
};