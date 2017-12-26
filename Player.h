//
//  Player.hpp
//  Project1
//
//  Created by Megan on 4/6/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#ifndef Player_h
#define Player_h
#include <string>

class Arena;
class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    std::string dropPoisonPellet();
    std::string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif /* Player_h */