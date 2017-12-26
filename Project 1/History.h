//
//  History.hpp
//  Project1
//
//  Created by Megan on 4/6/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int m_rows;
    int m_cols;
    int m_arena[MAXROWS][MAXCOLS];
};

#endif /* History_h */
