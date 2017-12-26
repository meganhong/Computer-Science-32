//
//  Set.cpp
//  Project2
//
//  Created by Megan on 4/18/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "Set.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

Set::Set()
{
    m_size = 0; // creates an empty set
    head = nullptr; // points both head and tail to null
    tail = nullptr;
}

Set::~Set()
{
    Node *n;
    n = head;
    while(!empty() && n != nullptr) // deletes node when set is not empty
    {
        Node *deletethis = n; // temporary pointer that will be deleted
        n = n->next; // n points to the next value so it will not be deleted
        delete deletethis; // delete the pointer that points to n
        m_size--; // decrement size
    }
}

Set::Set(const Set &other)
{
    m_size = 0;
    
    head = nullptr; // creates a new empty linked list
    tail = nullptr;
    
    Node *n = other.head; // creates new pointer that points to other's head
    for(int i = 0; i < other.m_size; i++) // copies all items into the new list
    {
        insert(n->item); // inserts all the items into the new set
        n = n->next; // goes to next item
    }
}

Set & Set::operator= (const Set &other)
{
    if(this != &other) // avoid self copy
    {
        Set temp(other); // calls swap function to copy and swap the sets
        swap(temp);
    }
    return *this;
}

bool Set::empty() const
{
    return size() == 0; // checks to see if set is empty
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    if(!contains(value)) // if set does not contain value
    {
        Node *p = new Node; // create new pointer
        p->item = value; // assign value to p's item
    
        if(empty() && p != nullptr) // if set is empty
        {
            p->prev = nullptr; // set prev to nullptr
            p->next = nullptr; // set head to nullptr
            head = p; // assign the new node to head and tail since there is only 1 item
            tail = p;
            m_size++;
        
            return true;
        }
        else if(p != nullptr) // inserts value to end of a non-empty set
        {
            p->next = nullptr; // creates a new nullptr at the end
            tail->next = p; // places value in the new position created
            p->prev = tail; // sets prev value to tail because p is in the new tail
            tail = p; // p occupies the last position
            m_size++;

            return true;
        }
    }
    return false;
}

bool Set::erase(const ItemType& value)
{
    if(empty()) // if set is empty
        return false;
    
    if(head != nullptr && head->item == value) // deletes value if it is in the 1st position
    {
        Node *n = head; // create a new node pointer
        head = head->next; // point head to next before deleting
        if(head != nullptr)
            head->prev = nullptr;
        delete n;
        
        m_size--; // decrement size
        return true;
    }
    
    // deletes value if it is in another position
    {
        Node* n = head;
        while(n != nullptr) // while current node is not at the end
        {
            if(n->next != nullptr && n->next->item == value) // if its next item is equal to value
            {
                Node* target = n->next; // set target equal to the item
                if(target->next != nullptr) // if it is not the last position
                    target->next->prev = target->prev; // rearrange to close the gap
                target->prev->next = target->next;
                delete target;
                
                m_size--;
                return true;
            }
            else
                n = n->next; // search next value if the values do not match
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    for(Node *n = head; n != nullptr; n = n->next) // loops through linked list
        if(n->item == value) // if the item in the node is equal to value
            return true;
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i >= 0 && i < m_size) // check to see if i is possible
    {
        Node *n1 = head; // create a new node pointer
        while(n1 != NULL)
        {
            int count = 0; // count variable to count how many are greater
            Node *n2 = n1; // create a new node pointer to search nodes after n1
            while(n2 != NULL)
            {
                if(n1->item > n2->item) // if the item in n1 is greater than an item after it
                    count++; // increment count variable
                n2 = n2->next; // go to next value until it reaches the end
            }
            
            n2 = n1; // sets n2 back to n1 to search for nodes before n1
            
            while(n2 != NULL)
            {
                if(n1->item > n2->item) // if the item in n1 is greater than an item before it
                    count++; // increment count variable
                n2 = n2->prev; // go to previus value until it reaches the beginning
            }
            
            if(count == i) // if count is equal to i
            {
                value = n1->item; // set value equal to item
                return true;
            }
            n1 = n1->next; // search next node if count is not equal to i
        }
    }
    return false;
}


void Set::swap(Set& other)
{
    Node *temp = head; // swap head pointers
    head = other.head;
    other.head = temp;
    
    Node *temptail = tail; // swap tail pointers
    tail = other.tail;
    other.tail = temptail;
    
    int tempsize = m_size; // swap sizes
    m_size = other.m_size;
    other.m_size = tempsize;
    
}

void unite(const Set& s1, const Set& s2, Set& result)
{
    Set temp; // creates a new empty set
    
    for(int i = 0; i < s1.size(); i++) // inserts items from s1 into temp set
    {
        ItemType j;
        s1.get(i, j); // to extract all items from s1
        temp.insert(j);
    }
    
    for(int i = 0; i < s2.size(); i++) // inserts items from s2 into temp set
    {
        ItemType k;
        s2.get(i, k); // to extract all items from s2
        temp.insert(k);
    }
    
    result = temp; // result is equal to the temp set created
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
    Set temp; // creates a new empty set
    
    if(s1.empty()) // if set 1 is empty, then result will be empty
        result = temp;

    else
    {
        for(int i = 0; i < s1.size(); i++) // inserts items from s1 into temp set
        {
            ItemType j;
            s1.get(i, j); // to extract all items from s1
            temp.insert(j);
        }
        
        for(int i = 0; i < s2.size(); i++) // takes out items from s2
        {
            ItemType j;
            s2.get(i, j); // to extract all items from s2
            ItemType deletethis = j;
            if(temp.contains(deletethis)) // if item from s2 is in s1, then delete
            {
                temp.erase(deletethis);
                i--;
            }
        
            result = temp; // result is equal to the temp set created
        }
    }
}

void Set::dump() const
{
    for(Node* n = head; n != NULL; n = n->next)
    {
        cerr << n->item << " ";
    }
}
