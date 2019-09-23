/* 
 * File:   overbejt_hw04.cpp
 * Author: Josh Overbeck
 * Description: This is for the homework 04 assignment in CSE 381.
 * Created on September 21, 2019, 12:55 PM
 * 
 * Copyright (c) 2019 overbejt@miamioh.edu
 */

#include <iostream>
#include <unistd.h>
//#include <sys/wait.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;
using StrVec = vector<string>;

/**
 * This is a helper method for building up a system call.  It is borrowed from
 * lab 3 supplemental pdf. 
 */
void myExec(StrVec cmd) {
    vector<char*> args;
    for (size_t i = 0; (i < cmd.size()); i++) {
        args.push_back(&cmd[i][0]);
    }
    // nullptr is very important
    args.push_back(nullptr);

    execvp(args[0], &args[0]);
}  // End of the 'myExec' method
/**
 * This is a helper method for parsing the command that the user enterd.
 */
void parseCmd(string input) {
    // Strip the quotes
    istringstream ss(input);
    input = "";
    string stripped;
    ss >> quoted(stripped);
    stripped += " ";
    while (ss >> quoted(input)) {stripped += input;}
            
    // Repeat the input to the user
    cout << "Running: " << stripped << endl;

    // Split the command up into a StrVec
    istringstream cmdStream(stripped);
    string splitString;
    StrVec cmd;
    while (ss >> splitString) {cmd.push_back(splitString);}
    // Execute the command
    myExec(cmd);
}  // End of the 'parseCmd' method

/**
 * This is a helper method for determining if the user wants to exit the program.
 */
bool exit(string input) {
    return input.find("exit") != string::npos;
}  // End of the 'exit' method

/*
 * The  main method 
 */
int main(int argc, char** argv) {
    // Main loop
    std::string line;
    while (std::cout << "> ", getline(cin, line)) {
	// Test if user wants to exit
	if (exit(line)) {return 0;}
        // Test if user entered a comment
        if (line[0] != '#' && !line.empty()) {
            // Process the user input
            parseCmd(line);
        }
    }
    return 0;
}  // End of 'main'
