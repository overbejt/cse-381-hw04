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
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <unordered_map>

using namespace std;
using StrVec = vector<string>;
using CmdVec = vector<string>;
using ExitVec = vector<int>;
using Pid_CmdMap = unordered_map<int, string>;

// Prototypes
bool exit(string input);
void serial(string fileName);
int preChecks(string input);
void initProcess(string inCmd);
void parseCmd(string input);    
void myExec(StrVec argList);
void parallel(string fileName);
void initProcessParallel(CmdVec commands);

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
 * It will run it in series.
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
 * This is a helper method for running the process.
 * It will run it in parallel.
 * 
 * @param commands The vector of commands that the user entered.
 */
void initProcessParallel(CmdVec commands) {
    Pid_CmdMap pids;
    // Loop and fork the process
    for(const auto cmd : commands) {
            const int pid = fork();
            pids.insert({pid, cmd});
        if (pid == 0) {
            try {
                // Process the user input
                preChecks(cmd);
	        cout << "Processing: " << cmd << endl;
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
        } else {
            int exitCode;
    	    waitpid(pid, &exitCode, 0);
	    cout << "Exit code: " << exitCode << endl;
        }
    }
}  // End of the 'initProcessParallel' method

int preChecks(string input) {
    // Test if user wants to exit
    if (exit(input)) {
        return 0;
    }
    // Test if user entered a comment
    if (input[0] != '#' && !input.empty()) {
        if (input.substr(0, 6) == "SERIAL") {
            serial(input.substr(7));
        } else if (input.substr(0, 8) == "PARALLEL") {
            cout << "you entered parallel" << endl;
	    cout << "substr: " << input.substr(9) << endl;
	    parallel(input.substr(9));
        } else {
            initProcess(input);
        }
    }
    return 888;
}  // End of the 'preChecks' method

/**
 * This is a helper method for when the user wants to run a batch of bash 
 * commands in serial.
 * @param fileName The name of the file. 
 */
void serial(string fileName) {
    ifstream contents(fileName, ifstream::in);
    for (string line; getline(contents, line);) {
        // Run the pre checks before executing
        preChecks(line);  // Or just send it to initProcess
    }
    contents.close();
}  // End of the 'serial' method

/**
 * This is a helper method for when the user wants to run a batch of bash 
 * commands in parallel.
 * @param fileName The name of the file. 
 */
void parallel(string fileName) {
    CmdVec commands;
    ifstream contents(fileName, ifstream::in);
    for (string line; getline(contents, line);) {
        // Push the command onto the CmdVec
        commands.push_back(line);
    }
    contents.close();
    // Send the commands to be executed

}  // End of the 'parallel' method

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
        // Run some pre checks
        int exitReady =  preChecks(line);
	if (exitReady == 0) {return 0;}
    }
    return 0;
}  // End of 'main'
