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

/**
 *This is a helper method for parsing the command that the user enterd.
 */
void parseCmd(string input) {
    // Make it do stuff
    
}  // End of the 'parseCmd' method

/**
 *This is a helper method for determining if the user wants to exit the program.
 */
bool exit(string input) {
    return input.find("exit") != string::npos;
}  // End of the 'exit' method

/*
 * 
 */
int main(int argc, char** argv) {
    // Main loop
    std::string line;
    while (std::cout << "> ", getline(cin, line)) {
	// Test if user wants to exit
	if (exit(line)) {return 0;}
        // Test if user entered a comment
        if (line[0] != '#' && !line.empty()) {
            // Strip the quotes
            istringstream ss(line);
            line = "";
            string stripped;
            ss >> quoted(stripped);
            stripped += " ";
            while (ss >> quoted(line)) {stripped += line;}
            
            // Repeat the input to the user
            cout << "Running: " << stripped << endl;
        }
    }

    return 0;
}  // End of 'main'

