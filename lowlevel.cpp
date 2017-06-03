//
//  lowlevel.cpp
//  LowLevel
//
//  Created by Aaron Ruel on 4/7/16.
//  Copyright (c) 2016 AAR. All rights reserved.
//

#include "lowlevel.h"

#define ASCII_NUM_OFFSET_L 48
#define ASCII_NUM_OFFSET_R 57

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PARSE FUNCS ///////////////////////////////////
//###############################################################################//

/////////////////////////////////////////
// Call the tokenizer to start parsing //
/////////////////////////////////////////
void LowLevel::tokenizer(std::string input){
    if(*stringIter == 0) input += " ";
    regulateCharStream(input);
    while(input[*stringIter] != '\0'){
        std::string temp = "";
        while(input[*stringIter] != ' '){
            temp += input[*stringIter];
            ++*stringIter;
        }
        interpreter(temp, input);
        ++*stringIter;
    }
}

void LowLevel::regulateCharStream(std::string input){
    while(input[*stringIter] == ' '){ // Get away from spaces, BAD spaces
        ++*stringIter;
    }
}

std::string LowLevel::grabNum(std::string input){
    std::string temp = "";
    regulateCharStream(input);
    while(input[*stringIter] != ' ' || input[*stringIter] == '\0'){
        // Check if input is a number
        if((int)input[*stringIter] >= ASCII_NUM_OFFSET_L && (int)input[*stringIter] <= ASCII_NUM_OFFSET_R){
            temp += input[*stringIter];
        }
        ++*stringIter;
    }
    return temp;
}

// Why makes two different grabs? Reaffirmation
std::string LowLevel::grabCommand(std::string input){
    std::string temp = "";
    regulateCharStream(input);
    while(input[*stringIter] != ' ' || input[*stringIter] == '\0'){
        temp += input[*stringIter];
        ++*stringIter;
    }
    return temp;
}

bool LowLevel::opCheck(std::string input){
    bool flag = false;
    regulateCharStream(input);
    for(int i = 0; i < operatorArray.size(); ++i){
        if(input == operatorArray[i]){
            flag = true;
        }
    }
    
    return flag;
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MAIN ROUT ////////////////////////////////////
//###############################################################################//

command_table LowLevel::hashTableCom(std::string input){
    if(input == "PrintStack") return PrintStack;
    if(input == "PrintTop") return PrintTop;
    if(input == "Load") return Load;
    if(input == "Math") return Math;
    if(input == "Stack") return Stack;
    if(input == "Var") return Var;
    else return ErrorCom;
}

void LowLevel::interpreter(std::string input, std::string fileInputStream){
    switch(hashTableCom(input)){
        case PrintStack:{
            stackPrint();
            break;
        }
        case PrintTop:{
            topPrint();
            break;
        }
        case Load:{
            load(fileInputStream);
            break;
        }
        case Math:{
            math(fileInputStream);
            break;
        }
        case Stack:{
            stackF(fileInputStream);
            break;
        }
        case Var:{
            varF(input, fileInputStream);
            break;
        }
        default:{
            // Unrecognized command handler
            printf("Unrecognized Command at Char %lu: %s\n", (*stringIter-input.length()+1), input.c_str());
            exit(EXIT_FAILURE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MATH FUNCS ///////////////////////////////////
//###############################################################################//

math_table LowLevel::hashTableMth(std::string input){
    if(input == "+") return plus;
    if(input == "-") return minus;
    if(input == "*") return multiply;
    if(input == "/") return divide;
    if(input == "sqrt") return squareroot;
    else return ErrorMth;
}

void LowLevel::math(std::string input){
    std::string temp = "";
    if(!grabvar(input)){ // excepts for varibles
        load(input); // Load first num on stack
    }
    temp = grabCommand(input); // get operator
    while(opCheck(temp)){ // check operator
        switch(hashTableMth(temp)){
            case plus:{
                pls(input);
                break;
            }
            case minus:{
                mns(input);
                break;
            }
            case multiply:{
                mlt(input);
                break;
            }
            case divide:{
                div(input);
                break;
            }
            case squareroot:{
                srt();
                break;
            }
            default:{
                // Unrecognized command handler
                printf("Unrecognized MATH Command at Char %lu: %s\n", (*stringIter-input.length()+1), input.c_str());
                exit(EXIT_FAILURE);
            }
        }
        temp = grabCommand(input);
    }
    *stringIter -= temp.length()+1;
}

void LowLevel::pls(std::string input){
    long long tempNum = 0;
    
    if(!grabvar(input)){ // excepts for varibles
        load(input); // Load first num on stack
    }
    tempNum = stack.top(); // place into temp
    stack.pop(); // pop top
    tempNum += stack.top(); // add
    stack.pop(); // so only the resultant remains
    stack.push(tempNum); // push added value
}

void LowLevel::mns(std::string input){
    long long tempNum = 0;
    if(!grabvar(input)){
        load(input);
    }
    long long tempNumTwo = 0;
    tempNumTwo = stack.top();
    stack.pop();
    tempNum = stack.top();
    stack.pop();
    tempNum = tempNum - tempNumTwo;
    stack.push(tempNum);
}

void LowLevel::mlt(std::string input){
    long long tempNum = 0;
    if(!grabvar(input)){ // excepts for varibles
        load(input); // Load first num on stack
    }
    tempNum = stack.top();
    stack.pop();
    tempNum *= stack.top();
    stack.pop();
    stack.push(tempNum);
}

void LowLevel::div(std::string input){
    long long tempNum = 0;
    if(!grabvar(input)){ // excepts for varibles
        load(input); // Load first num on stack
    }
    tempNum = stack.top();
    stack.pop();
    tempNum /= stack.top();
    stack.pop();
    stack.push(tempNum);
}

void LowLevel::srt(){
    double tempNumFloat = 0.f;
    tempNumFloat = sqrt(stack.top());
    stack.pop();
    stack.push(tempNumFloat);
}

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// STACK FUNCS ///////////////////////////////////
//###############################################################################//

stack_table LowLevel::hashTableStk(std::string input){
    if(input == "Duplicate") return duplicate;
    if(input == "Drop") return drop;
    if(input == "Swap") return swap;
    if(input == "Overwrite") return overwrite;
    if(input == "Rotate") return rotate;
    else return ErrorStk;
}

void LowLevel::stackF(std::string input){
    std::string command = grabCommand(input);
    if(stack.size() > 0){
        switch(hashTableStk(command)){
            case duplicate:{
                dup();
                break;
            }
            case drop:{
                drp();
                break;
            }
            case swap:{
                swp();
                break;
            }
            case overwrite:{
                ovr();
                break;
            }
            case rotate:{
                rot();
                break;
            }
            default:{
                // Unrecognized command handler
                printf("Unrecognized STK Command at Char %lu: %s\n", (*stringIter-input.length()+1), input.c_str());
                exit(EXIT_FAILURE);
            }
        }
    }
    else{
        printf("Invalid Stack Access: Stack Empty");
        exit(EXIT_FAILURE);
    }
}

// Duplicate top of stack, there exists two of the same number
// in the top two stack positions
void LowLevel::dup(){
    stack.push(stack.top());
}

void LowLevel::drp(){ // Literally just popping the stack
    stack.pop();
}

void LowLevel::swp(){ // Swap first two elements in stack
    long long pos1 = 0, pos2 = 0;
    pos1 = stack.top();
    stack.pop();
    pos2 = stack.top();
    stack.pop();
    stack.push(pos1);
    stack.push(pos2); // pos 2 ends up on top
}

void LowLevel::ovr(){ // Duplicate second item overwritting the top
    stack.pop();
    stack.push(stack.top());
}

void LowLevel::rot(){ // Rotate the position of first three stack elements
    long long pos1 = 0, pos2 = 0, pos3 = 0;
    pos1 = stack.top();
    stack.pop();
    pos2 = stack.top();
    stack.pop();
    pos3 = stack.top();
    stack.pop();
    stack.push(pos1);
    stack.push(pos2);
    stack.push(pos3);
}

void LowLevel::stackPrint(){
    // Prints contents of stack
    unsigned char iter = 0x0;
    while(!stack.empty()){
        printf("0x%x : %lli\n", iter, stack.top());
        iter += 0x4;
        stack.pop();
    }
}

void LowLevel::topPrint(){
    // Print and remove top of stack
    if(!stack.empty()){
        printf("0x0 : %lli\n", stack.top());
        stack.pop();
    }
    else {
        printf("Stack Empty!");
    }
}

void LowLevel::load(std::string input){
    std::string temp = "";
    temp = grabNum(input);
    if(temp.length() > 0){
        signed long long strtoi;
        try {
            strtoi = boost::lexical_cast<signed long long>(temp);
        }
        catch(boost::bad_lexical_cast const&) {
            printf(" -Warning: Bad Syntax- ");
        }
        stack.push(strtoi);
    }
}

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// VARIABLE FUNCS /////////////////////////////////
//###############################################################################//

variable_table LowLevel::hashTableVar(std::string input){
    if(input == "Var") return vari;
    else return ErrorVar;
}

void LowLevel::varF(std::string input, std::string inputGlob){
    switch(hashTableVar(input)){
        case vari:{
            var(inputGlob);
            break;
        }
        default:{
            // Unrecognized command handler
            printf("Unrecognized VAR Command at Char %lu: %s\n", (*stringIter-input.length()+1), input.c_str());
            exit(EXIT_FAILURE);
        }
    }
}

void LowLevel::var(std::string input){
    variable<signed long long> v;
    v.name = grabCommand(input);
    std::string e = grabCommand(input);
    if(e == "="){
        load(input); // Loading onto stack *temporarily* to handle lexigraphical casting
        v.value = stack.top();
        stack.pop();
        varstk.push_back(v);
    }
    else{
        printf("Syntax Error %s operator not recognized\n", e.c_str());
        exit(EXIT_FAILURE);
    }
    sortvar();
}

void LowLevel::sortvar(){ // Bubble sort used as vector treated as "nearly sorted"
    bool flag = true;
    for(int i = 1; (i <= varstk.size()) && flag; ++i){
        flag = false;
        for(int j = 0; j < (varstk.size() - 1); ++j){
            if(varstk[j+1].name < varstk[j].name){
                std::swap(varstk[j], varstk[j+1]);
                flag = true;
            }
        }
    }
}

bool LowLevel::grabvar(std::string input){
    std::string pulledString = grabCommand(input);
    unsigned int lowbound = 0, upperbound = (unsigned int)varstk.size(), searchpoint = upperbound/2;
    while(varstk[searchpoint].name != pulledString && lowbound < upperbound){
        if(varstk[searchpoint].name > pulledString){
            upperbound = searchpoint - 1;
        }
        else{
            lowbound = searchpoint + 1;
        }
        searchpoint = (lowbound + upperbound) / 2;
    }
    if(lowbound <= upperbound){
        // searchpoint now holds the variable index
        stack.push(varstk[searchpoint].value);
        return true;
    }
    return false;
}