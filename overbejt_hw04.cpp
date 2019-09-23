/* 
 * File:   overbejt_hw04.cpp
 * Author: Josh Overbeck
 * Description: This is for the homework 04 assignment in CSE 381.
 * Created on September 21, 2019, 12:55 PM
 * 
 * Copyright (c) 2019 overbejt@miamioh.edu
 */

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>


using namespace std;
using StrVec = vector<string>;

/**
 * This is a helper method for executing system calls supplied by the user.  
 * It is borrowed from the supplemental pdf from lab 4.
 */
void  myExec(StrVec argList) {
    std::vector<char*> args;
    for (size_t i = 0; (i < argList.size()); i++) {
        args.push_back(&argList[i][0]);
    }
    // nullptr is very important
    args.push_back(nullptr);
    execvp(args[0], &args[0]);
}  // End of the 'myExec' method

/**
 * This is a helper method for parsing the command that the user enterd.
 */
void parseCmd(string input) {
//    // Strip the quotes
//    istringstream ss(input);
//    input = "";
//    string stripped;
//    ss >> quoted(stripped);
//    stripped += " ";
//    cout << ss.str() << endl;
//    while (ss >> quoted(input)) {stripped += input;}
//            
//    // Repeat the input to the user
//    cout << "Running: " << stripped << endl;
    
    cout << "Running: " << input << endl;

    // Split the command up into a StrVec
    istringstream cmdStream(input);  // 
    StrVec cmd;
    for (string splitString; cmdStream >> splitString;) {
        if (splitString[0] == '"') { 
            string ending;
            getline(cmdStream, ending);
            splitString += ending;
            cout << splitString << endl;
        }
        cmd.push_back(splitString);
    }

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
            // Fork the process
            const int pid = fork();
            int exitCode;
            if (pid == 0) {
                try {
                    // Process the user input
                    parseCmd(line);
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
            } else {
                waitpid(pid, &exitCode, 0); 
                cout << "Exit code: " << exitCode << endl;
            }
        }
    }
    return 0;
}  // End of 'main'
