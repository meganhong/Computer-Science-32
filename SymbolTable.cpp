//
//  SymbolTable.cpp
//  Project4
//
//  Created by Megan on 6/2/17.
//  Copyright © 2017 Megan. All rights reserved.
//

// SymbolTable.cpp

#include "SymbolTable.h"
#include <string>
#include <stack>
#include <functional>
#include <list>

using namespace std;

const int TABLE_SIZE = 19997; // hash table size

class SymbolTableImpl
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    unsigned int hashFunc(const string & id) const;
    int currentScope = 0; // keeps track of scope in enter/exit scope
    
    struct Entry // entry in list
    {
        Entry(int l, int s, string ID)
        : line(l), scope(s), id(ID)
        { }
        int line;
        int scope;
        string id;
    };
    
    struct Bucket // bucket in hash table
    {
        list<Entry> bucketList;
    };
    
    stack<Entry> scopeStack; // keeps track of scopes
    Bucket hashTable [TABLE_SIZE]; // hash table to look up strings quickly
};

void SymbolTableImpl::enterScope()
{
    currentScope++; // increment scope
    Entry temp(0, currentScope, "");
    scopeStack.push(temp);
}

bool SymbolTableImpl::exitScope()
{
    if(currentScope <= 0) // if there are any extra }
        return false;
    
    while(!scopeStack.empty() && scopeStack.top().id != "")
    {
        int bucket = hashFunc(scopeStack.top().id);

        // looks for matching id in current scope
        for(list<Entry>::iterator it = hashTable[bucket].bucketList.begin(); it != hashTable[bucket].bucketList.end();)
        {
            if(it->id == scopeStack.top().id && it->scope == currentScope)
            {
                it = hashTable[bucket].bucketList.erase(it); // delete all items in scope
            }
            else
                it++;
        }
        scopeStack.pop();
    }
    currentScope--;
    scopeStack.pop(); // pop off empty string
    return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
    int bucket = hashFunc(id);
    
    if(hashTable[bucket].bucketList.empty())
    {
        Entry e(lineNum, currentScope, id); // create a new entry
        hashTable[bucket].bucketList.push_front(e);
        scopeStack.push(e);
        return true;
    }
    
    for(list<Entry>::iterator it = hashTable[bucket].bucketList.begin(); it != hashTable[bucket].bucketList.end(); it++)
    {
        if(it->id == id && it->scope == currentScope) // if already in current scope
        {
            return false;
        }
    }
    
    // push onto list and stack
    Entry e(lineNum, currentScope, id);
    hashTable[bucket].bucketList.push_front(e);
    scopeStack.push(e);
    return true;
}

int SymbolTableImpl::find(const string& id) const
{
    int bucket = hashFunc(id);
    
    if(hashTable[bucket].bucketList.empty())
        return -1;
    
    // search through list in bucket
    
    for(list<Entry>::const_iterator it = hashTable[bucket].bucketList.begin(); it != hashTable[bucket].bucketList.end(); it++)
    {
        if(it->id == id && it->scope <= currentScope) // if there is a match
            return it->line;
    }
    return -1;
}

unsigned int SymbolTableImpl::hashFunc(const string &id) const
{
    hash<string> str_hash;
    unsigned int hashVal = str_hash(id);
    unsigned int bucket = hashVal % TABLE_SIZE;
    return bucket;
}


//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}

