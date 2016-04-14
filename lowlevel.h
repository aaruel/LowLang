//
//  lowlevel.h
//  LowLevel
//
//  Created by Aaron Ruel on 4/7/16.
//  Copyright (c) 2016 AAR. All rights reserved.
//

#ifndef __LowLevel__lowlevel__
#define __LowLevel__lowlevel__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <array>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

enum command_table{
    ErrorCom = -1,
    PrintStack,
    PrintTop,
    Load,
    Math,
    Stack
};

enum math_table{
    ErrorMth = 1,
    plus,
    minus,
    multiply,
    divide,
    squareroot
};

enum stack_table{
    ErrorStk = -1,
    duplicate,
    drop,
    swap,
    overwrite,
    rotate
};

class LowLevel {
private:
    std::stack<signed long long> stack;
    // updating to a parse tree in the future (!!)
    // Keeps track of the file pointer position
    //int *stringIter = new int(0);
    std::unique_ptr<int> stringIter{new int(0)}; // Pointers are scary after all
    
    const std::array<std::string, 5> operatorArray = {"+", "-", "*", "/", "sqrt"}; // Allows for equation chaining
    
public:
    // Parsing Functions
    void tokenizer(std::string input);
    void load(std::string input);
    void interpreter(std::string input, std::string fileInputStream);
    void regulateCharStream(std::string input);
    // Print Functions
    void stackPrint();
    void topPrint();
    // Main::Math Function
    void math(std::string input);
    void pls(std::string input);
    void mns(std::string input);
    void mlt(std::string input);
    void div(std::string input);
    void srt();
    bool opCheck(std::string input);
    // Main::Stack Functions
    void stackF(std::string input);
    void dup();
    void drp();
    void swp();
    void ovr();
    void rot();
    // Main::Variable Functions
    void var();
    // Grab next thing functions
    std::string grabNum(std::string input);
    std::string grabCommand(std::string input);
    // Enum Functions
    command_table hashTableCom(std::string input);
    math_table hashTableMth(std::string input);
    stack_table hashTableStk(std::string input);
};

#endif /* defined(__LowLevel__lowlevel__) */
