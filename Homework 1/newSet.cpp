//
//  newSet.cpp
//  Homework1
//
//  Created by Megan on 4/15/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "newSet.h"
#include <iostream>
#include <cstdlib>

Set::Set()
{
    m_size = 0;
    m_capacity = DEFAULT_MAX_ITEMS;
    m_set = new ItemType[m_capacity];
}

Set::Set(int size)
{
    if(size < 0)
        exit(1);
    m_size = 0;
    m_capacity = size;
    m_set = new ItemType[m_capacity];
}

Set::Set(const Set& other)
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_set = new ItemType[m_capacity];
    for(int i = 0; i < m_size; i++)
        m_set[i] = other.m_set[i];
}

Set& Set::operator= (const Set &s)
{
    
    if (this != &s)
    {
        Set tempset(s);
        swap(tempset);
    }
    return *this;
}

Set::~Set()
{
    delete [] m_set;
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
    if(m_size >= m_capacity)
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
    ItemType* temp;
    
    temp = m_set;
    m_set = other.m_set;
    other.m_set = temp;

    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    int tempCapacity;
    tempCapacity = m_capacity;
    m_capacity = other.m_capacity;
    other.m_capacity = tempCapacity;
}
