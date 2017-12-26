//
//  animal.cpp
//  Homework3
//
//  Created by Megan on 5/2/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

class Animal
{
    public:
    Animal(string n)
    {
        m_name = n;
    }
    string name() const
    {
        return m_name;
    }
    virtual ~Animal()
    {
    }
    virtual string moveAction() const
    {
        return "walk";
    }
    virtual void speak() const = 0;
    
    private:
    string m_name;
};

class Cat : public Animal
{
    public:
    Cat(string n)
    : Animal(n)
    {
    }
    virtual ~Cat()
    {
        cout << "Destroying " << this->name() << " the cat" << endl;
    }
    virtual void speak() const
    {
        cout << "Meow";
    }
};

class Pig : public Animal
{
    public:
    Pig(string n, double w)
    : Animal(n)
    {
        m_weight = w;
    }
    virtual ~Pig()
    {
        cout << "Destroying " << this->name() << " the pig" << endl;
    }
    virtual void speak() const
    {
        if(m_weight >= 170)
            cout << "Grunt";
        else
            cout << "Oink";
    }
    
    private:
    double m_weight;
};

class Duck : public Animal
{
    public:
    Duck(string n)
    : Animal(n)
    {
    }
    virtual ~Duck()
    {
        cout << "Destroying " << this->name() << " the duck" << endl;
    }
    virtual void speak() const
    {
        cout << "Quack";
    }
    virtual string moveAction() const
    {
        return "swim";
    }
};