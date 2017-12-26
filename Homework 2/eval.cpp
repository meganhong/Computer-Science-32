//
//  eval.cpp
//  Homework 2
//
//  Created by Megan on 4/27/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

string removeSpaces(string infix); // removes spaces in infix
int type(char c); // returns 1 if operand (T, F) or 2 if operator (&, |)
bool checkFormat(string infix); // checks if infix is a valid expression
int precedence(char c); // returns precedence of operator
void changeToPostfix(string infix, string& postfix); // changes infix to postfix

int evaluate(string infix, string& postfix, bool& result)
{
    removeSpaces(infix);
    
    if(checkFormat(infix))
    {
        changeToPostfix(infix, postfix);
        
        if(postfix.empty())
            return 1;
        
        stack<bool> boolStack;
        for(int i = 0; i < postfix.size(); i++)
        {
            if(postfix[i] == 'T')
                boolStack.push(true);
            else if(postfix[i] == 'F')
                boolStack.push(false);
            else if(postfix[i] == '!')
            {
                bool temp = boolStack.top();
                boolStack.pop();
                boolStack.push(!temp);
            }
            else
            {
                bool op2 = boolStack.top();
                boolStack.pop();
                bool op1 = boolStack.top();
                boolStack.pop();

                if(postfix[i] == '&')
                    boolStack.push(op1 && op2);
                else if(postfix[i] == '|')
                    boolStack.push(op1 || op2);
            }
        }
        result = boolStack.top();
        return 0;
    }
    return 1;
}

string removeSpaces(string infix)
{
    string s = "";
    for(int i = 0; i < infix.size(); i++)
        if(infix[i] != ' ')
            s += infix[i];
    return s;
}

int type(char c)
{
    if(c == 'T' || c == 'F')
        return 1;
    else if(c == '&' || c == '|')
        return 2;
    return 0;
}

bool checkFormat(string infix)
{
    infix = removeSpaces(infix);
    
    if(infix == "") // if empty
        return false;
    
    int last = int(infix.size()-1);
    
    if(infix[0] == ')' || infix[0] == '&' ||  infix[0] == '|') // checks first position
        return false;
    
    if(infix[last] == '!' || infix[last] == '&' || infix[last] == '|' || infix[last] == '(') // checks last position
        return false;
    
    for(int i = 0; i < infix.size(); i++)
    {
        char c = infix[i];
        char next = infix[i+1];
        char prev = infix[i-1];
        if(type(c) == 1) // if character is T or F
        {
            if(type(next) == 1 || next == '!' || next == '(') // cannot have these types after it
                return false;
        }
        else if(type(c) == 2) // if chracter is & or |
        {
            if(type(prev) == 2 || prev == '(' || prev == '|' || prev == '!') // cannot have these types preceding it
                return false;
            if(next == ')' || next == '&'|| next == '|') // cannot have these types after it
                return false;
        }
        else if(c == '!')
        {
            if(prev == 'T' || prev == 'F' || prev == ')')
                return false;
        }
    }
    int count1 = 0; // counts open parentheses
    int count2 = 0; // counts closing parentheses
    int counttf = 0; // counts T or F
    for(int i = 0; i < infix.size(); i++)
    {
        if(infix[i] == '(')
            count1++;
        if(infix[i] == ')')
            count2++;
        if(type(infix[i]) == 1)
            counttf++;
    }
    if(count1 != count2) // cannot have uneven number of parentheses
        return false;
    if(counttf == 0) // cannot have 0 T or F's
       return false;
    
    return true;
}

int precedence(char c)
{
    switch(c)
    {
        case '|':
            return 0;
            break;
        case '&':
            return 1;
            break;
        case '!':
            return 2;
            break;
    }
    return c;
}

void changeToPostfix(string infix, string& postfix)
{
    infix = removeSpaces(infix);
    postfix = "";
    stack<char> postStack;
    for(int i = 0; i < infix.size(); i++)
    {
        switch(infix[i])
        {
            case 'T':
            case 'F':
                postfix += infix[i];
                break;
            case '!':
            case '&':
            case '|':
                while(!postStack.empty() && postStack.top() != '(' && (precedence(infix[i]) <= precedence(postStack.top())))
                {
                    postfix += postStack.top();
                    postStack.pop();
                }
                postStack.push(infix[i]);
                break;
            case '(':
                postStack.push(infix[i]);
                break;
            case ')':
                while(!postStack.empty() && postStack.top() != '(')
                {
                    postfix += postStack.top();
                    postStack.pop();
                }
                postStack.pop();
                break;
            default:
                break;
        }
    }
    while(!postStack.empty())
    {
        postfix += postStack.top();
        postStack.pop();
    }
}

int main()
{
    string pf;
    bool answer;
    assert(evaluate("T| F", pf, answer) == 0  &&  pf == "TF|"  &&  answer);
    assert(evaluate("T|", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("T(F|T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("(T&(F|F)", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0
           &&  pf == "FF!TF&&|"  &&  !answer);
    assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);
    assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
    cout << "Passed all tests" << endl;
}
