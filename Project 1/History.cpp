//
//  History.cpp
//  Project1
//
//  Created by Megan on 4/6/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols)
{
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
        {
            m_arena[r][c] = 0;
        }
}

bool History::record(int r, int c)
{
    if (r < 1 || c < 1 || r > m_rows || c > m_cols)
        return false;
    
    m_arena[r-1][c-1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    char grid[MAXROWS][MAXCOLS];
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
        {
            if (m_arena[r][c] == 0)
                grid[r][c] = '.';
            else if (m_arena[r][c] >= 26)
                grid[r][c] = 'Z';
            else
                grid[r][c] = 64 + m_arena[r][c];
        }
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}
