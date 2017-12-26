//
//  newSet.hpp
//  Homework1
//
//  Created by Megan on 4/15/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#ifndef newSet_h
#define newSet_h

typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 200;

class Set
{
public:
    Set();         // Create an empty set.
    
    Set(int size); // Create a set with size as an input
    
    Set(const Set& other); // Copy constructor
    
    Set &operator= (const Set &s); // Assignment operator
    
    ~Set(); // Deconstructor
    
    bool empty() const;  // Return true if the set is empty, otherwise false.
    
    int size() const;    // Return the number of items in the set.
    
    bool insert(const ItemType& value);
    // Insert value into the set if it is not already present.  Return
    // true if the value was actually inserted.  Leave the set unchanged
    // and return false if the value was not inserted (perhaps because it
    // is already in the set or because the set has a fixed capacity and
    // is full).
    
    bool erase(const ItemType& value);
    // Remove the value from the set if present.  Return true if the
    // value was removed; otherwise, leave the set unchanged and
    // return false.
    
    bool contains(const ItemType& value) const;
    // Return true if the value is in the set, otherwise false.
    
    bool get(int i, ItemType& value) const;
    // If 0 <= i < size(), copy into value the item in the set that is
    // greater than exactly i items in the set and return true.  Otherwise,
    // leave value unchanged and return false.
    
    void swap(Set& other);
    // Exchange the contents of this set with the other one.
    
private:
    ItemType *m_set;
    int m_size;
    int m_capacity;
    
};


#endif /* newSet_hpp */
