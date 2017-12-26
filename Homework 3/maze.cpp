//
//  maze.cpp
//  Homework3
//
//  Created by Megan on 5/5/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

    maze[sr][sc] = '@';
    if (sr == er  &&  sc == ec)
        return true;
    else
    {
        if(maze[sr-1][sc] == '.')
        {
            if(pathExists(maze, sr-1, sc, er, ec))
                return true;
        }
        if(maze[sr+1][sc] == '.')
        {
            if(pathExists(maze, sr+1, sc, er, ec))
                return true;
        }
        if(maze[sr][sc+1] == '.')
        {
            if(pathExists(maze, sr, sc+1, er, ec))
                return true;
        }
        if(maze[sr][sc-1] == '.')
        {
            if(pathExists(maze, sr, sc-1, er, ec))
                return true;
        }
    }
    return false;
}
