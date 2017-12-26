//
//  Game.hpp
//  Project1
//
//  Created by Megan on 4/6/17.
//  Copyright © 2017 Megan. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <string>

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */
