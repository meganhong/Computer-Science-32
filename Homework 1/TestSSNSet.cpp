//
//  TestSSNSet.cpp
//  Homework1
//
//  Created by Megan on 4/18/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "SSNSet.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    SSNSet test;
    ItemType v[3] = { 10, 20, 30 };
    
    for (int k = 0; k < 3; k++)
        test.add(v[k]);
    
    assert(test.size() == 3);
    
    test.add(40);
    test.add(50);
    assert(test.size() == 5);
    
    test.print(); //should bring 10 20 30 40 50 in one column
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
