//
//  SSNSet.cpp
//  Homework1
//
//  Created by Megan on 4/14/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "SSNSet.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet()
:m_set()
{
}

bool SSNSet::add(unsigned long ssn)
{
    m_set.insert(ssn);
    return false;
}

int SSNSet::size() const
{
    return m_set.size();
}

void SSNSet::print() const
{
    ItemType number;
    for(int i = 0; i < m_set.size(); i++)
    {
        m_set.get(i, number);
        cout << number << endl;
    }
}