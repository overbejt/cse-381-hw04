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
 * 
 * @param argList The command that the user entered, as an array of strings.
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
 * This is a helper method for parsing the command that the user entered.
 * 
 * @param input The command that the user entered.
 */
void parseCmd(string input) {    
    cout << "Running: " << input << endl;

    // Split the command up into a StrVec
    istringstream cmdStream(input);  
    StrVec cmd;
    for (string splitString; cmdStream >> splitString;) {
        // Handle string inputs
        if (splitString[0] == '"') { 
            // Concatenate the entire string as one line
            string ending;
            getline(cmdStream, ending);
            splitString += ending;
            // Removing quotes
            splitString.erase(remove(splitString.begin(), 
                    splitString.end(), '\"'), splitString.end());
        }
        cmd.push_back(splitString);
    }
    // Execute the command
    myExec(cmd);
}  // End of the 'parseCmd' method

/**
 * This is a helper method for running the process.
 * 
 * @param inCmd The command that the user entered.
 */
void initProcess(string inCmd) {
    // Fork the process
    const int pid = fork();
    int exitCode;
    if (pid == 0) {
        try {
            // Process the user input
            parseCmd(inCmd);
        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    } else {
        waitpid(pid, &exitCode, 0); 
        cout << "Exit code: " << exitCode << endl;
    }
}  // End of the 'initProcess' method

/**
 * This is a helper method for determining if the user wants to exit 
 * the program.
 * 
 * @param input The command that the user entered.
 * @return True if the user typed 'exit' and false if not.
 */
bool exit(string input) {
    // This might cause problems when exiting an ssh session
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
            initProcess(line);
        }
    }
    return 0;
}  // End of 'main'
