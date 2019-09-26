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


using namespace std;
using StrVec = vector<string>;
using PidVec = vector<int>;

// Prototypes
bool exit(string input);
void serial(string fileName);
void parallel(string fileName);
int preChecks(string input);
void initProcess(string inCmd);
void parseCmd(string input);    
void  myExec(StrVec argList);

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
    // Split the command up into a StrVec
    istringstream cmdStream(input);  
    StrVec cmd;
    string print = "";
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
        print += splitString + " ";
    }
    cout << "Running: " << print << endl;
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
 * This is a helper method that will filter out comments and empty lines from 
 * the user.
 * 
 * @param input The input from the user.
 * @return whether or not the user wants to exit the program.  If so, it returns
 *         0, otherwise it will return -1.
 */
int preChecks(string input) {
    // Test if user wants to exit
    if (exit(input)) {
        exit(0);
    }
    // Test if user entered a comment
    if (input[0] != '#' && !input.empty()) {
        if (input.substr(0, 6) == "SERIAL") {
            serial(input.substr(7));
        } else if (input.substr(0, 8) == "PARALLEL") {
            parallel(input.substr(9));
        } else {
            initProcess(input);
        }
    }
    return -1;
}  // End of the 'preChecks' method

/**
 * This is a helper method for processing commands that are meant to be ran in
 * parallel.
 * 
 * @param commands the commands that the user wants to run.
 */
void parallel(string fileName) {
    StrVec commands;
    PidVec pids;
    // Scrape the file
    ifstream contents(fileName, ifstream::in);
    for (string line; getline(contents, line);) {
        // Push each line onto a vector
        commands.push_back(line);
    }    
    contents.close();
    // Create a fork for each line
    for (auto const cmd : commands) {
        const int pid = fork();
        pids.push_back(pid);
    }        
    // Execute each line
    for (auto const cmd : commands) {
        // Test if user wants to exit
        if (exit(cmd)) {
            exit(0);
        }
        // Test if user entered a comment
        if (cmd[0] != '#' && !cmd.empty()) {
            initProcess(cmd);
        }
    }
    
    // Todo: why is it not exiting when exit command is passed after running 
    // parallel?
      
    // Wait for each pid to return
    for (const auto pid : pids) {
        int exitCode;
        waitpid(pid, &exitCode, 0);
    }
    cout << "Parallel is finished running." << endl;
}  // End of the 'parallel' method

/**
 * This is a helper method for when the user wants to run a batch of bash 
 * commands in serial.
 * 
 * @param fileName The name of the file. 
 */
void serial(string fileName) {
    ifstream contents(fileName, ifstream::in);
    for (string line; getline(contents, line);) {
        preChecks(line);
    }
    contents.close();
}  // End of the 'serial' method

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
    while (getline(cin, line)) {
        // Run some pre checks
        int exitReady =  preChecks(line);
        if (exitReady == 0) {return 0;}
    }
    return 0;
}  // End of 'main'
