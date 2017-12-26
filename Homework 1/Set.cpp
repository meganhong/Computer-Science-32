//
//  Set.cpp
//  Homework1
//
//  Created by Megan on 4/12/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "Set.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

Set::Set()
{
    m_size = 0;
}

bool Set::empty() const
{
    if(m_size == 0)
    {
        return true;
    }
    return false;
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    for(int i = 0; i < m_size; i++)
        if(m_set[i] == value)
            return false;
    if(m_size >= DEFAULT_MAX_ITEMS)
        return false;
    else
    {
        m_set[m_size] = value;
        m_size++;
        return true;
    }
}

bool Set::erase(const ItemType& value)
{
    if(this->contains(value))
    {
        for(int i = 0; i < m_size; i++)
        {
            if(m_set[i] == value)
            {
                for(int j = i; j < m_size; j++)
                {
                    if(j == m_size - 1)
                        m_set[j] = ' ';
                    else
                        m_set[j] = m_set[j+1];
                }
                m_size--;
            }
        }
        return true;
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    for(int i = 0; i < m_size; i++)
        if(m_set[i] == value)
            return true;
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i >= 0 && i < m_size)
    {
        for(int k = 0; k < m_size; k++)
        {
            int count = 0;
            for(int j = 0; j < m_size; j++)
            {
                // if((m_set[k]).compare(m_set[j]) > 0)
                if(m_set[k] > m_set[j])
                    count++;
            }
            if(count == i)
            {
                value = m_set[k];
                return true;
            }
        }
    }
    return false;
}

void Set::swap(Set& other)
{
    Set temp = *this;
    *this = other;
    other = temp;
}

void Set::print()
{
    for(int i = 0; i < m_size; i++)
        cout << m_set[i] << endl;
}
