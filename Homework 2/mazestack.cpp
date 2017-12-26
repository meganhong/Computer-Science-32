//
//  mazestack.cpp
//  Homework 2
//
//  Created by Megan on 4/27/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    stack <Coord> m_stack;
    Coord start(sr, sc);
    
    m_stack.push(start);
    maze[sr][sc] = '@';
    
    while(!m_stack.empty())
    {
        Coord cur = m_stack.top();
        int row = cur.r();
        int col = cur.c();
        
        m_stack.pop();
        
        if(row == er && col == ec)
            return true;
        
        if(maze[row-1][col] == '.') // north
        {
            m_stack.push(Coord(row-1, col));
            maze[row-1][col] = '@';
        }
        if(maze[row][col+1] == '.') // east
        {
            m_stack.push(Coord(row, col+1));
            maze[row][col+1] = '@';
        }
        if(maze[row+1][col] == '.') // south
        {
            m_stack.push(Coord(row+1, col));
            maze[row+1][col] = '@';
        }
        if(maze[row][col-1] == '.') // west
        {
            m_stack.push(Coord(row, col-1));
            maze[row][col-1] = '@';
        }
    }
    return false;
}

int main()
{
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X'},
        { 'X','.','.','.','.','.','.','.','.','X'},
        { 'X','X','.','X','.','X','X','X','X','X'},
        { 'X','.','.','X','.','X','.','.','.','X'},
        { 'X','.','.','X','.','.','.','X','.','X'},
        { 'X','X','X','X','.','X','X','X','.','X'},
        { 'X','.','X','.','.','.','.','X','X','X'},
        { 'X','.','X','X','X','.','X','X','.','X'},
        { 'X','.','.','.','X','.','.','.','.','X'},
        { 'X','X','X','X','X','X','X','X','X','X'}
    };
    
    if (pathExists(maze, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
