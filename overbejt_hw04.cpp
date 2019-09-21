/* 
 * File:   overbejt_hw04.cpp
 * Author: Josh Overbeck
 * Description: This is for the homework 04 assignment in CSE 381.
 * Created on September 21, 2019, 12:55 PM
 * 
 * Copyright (c) 2019 overbejt@miamioh.edu
 */

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    // Main loop
    std::string line;
    while (std::cout << "> ", getline(cin, line)) {
        // Process the input line here.
        istringstream ss(line);
        line = "";
        string stripped;
        ss >> quoted(stripped);
        stripped += " ";
        while (ss >> quoted(line)) {stripped += line;}
        cout << "Running: " << stripped << endl;
    }

    return 0;
}  // End of 'main'

