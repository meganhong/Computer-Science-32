//
//  testnewSet.cpp
//  Homework1
//
//  Created by Megan on 4/15/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "newSet.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Set a(1000);   // a can hold at most 1000 distinct items
    Set b(5);      // b can hold at most 5 distinct items
    Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    ItemType v[6] = { 1, 2, 3, 4, 5, 6 };
    
    // No failures inserting 5 distinct items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v[k]));
    
    // Failure if we try to insert a sixth distinct item into b
    assert(!b.insert(v[5]));
    
    // When two Sets' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(v[5])  &&  b.insert(v[5]));
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}