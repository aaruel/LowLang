//
//  main.cpp
//  LowLevel
//
//  Created by Aaron Ruel on 4/7/16.
//  Copyright (c) 2016 AAR. All rights reserved.
//

#include <iostream>
#include "lowlevel.h"

int main(int argc, const char * argv[]) {
    LowLevel lang;
    
    lang.tokenizer("Math 1 + 1 + 444 - 4 PrintStack");
}
