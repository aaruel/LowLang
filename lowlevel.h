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
#include <algorithm>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

enum command_table{
    ErrorCom = -1,
    PrintStack,
    PrintTop,
    Load,
    Math,
    Stack,
    Var
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

enum variable_table{
    ErrorVar = -1,
    vari
};

template<typename T>
struct variable{
    std::string name;
    T value;
};

class LowLevel {
private:
    std::vector<variable<signed long long> > varstk;
    std::stack<signed long long> stack;
    // updating to a parse tree in the future (!!)
    // Keeps track of the file pointer position
    std::unique_ptr<int> stringIter{new int(0)};
    
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
    void varF(std::string input, std::string inputGlob);
    void var(std::string input);
    void sortvar();
    bool grabvar(std::string input);
    // Grab next thing functions
    std::string grabNum(std::string input);
    std::string grabCommand(std::string input);
    // Enum Functions
    command_table hashTableCom(std::string input);
    math_table hashTableMth(std::string input);
    stack_table hashTableStk(std::string input);
    variable_table hashTableVar(std::string input);
};

#endif /* defined(__LowLevel__lowlevel__) */
