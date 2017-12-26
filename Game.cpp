#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <algorithm>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
  private:
    int m_rows;
    int m_cols;
    
    struct Ship
    {
        int length;
        char symbol;
        string name;
        string dir;
    };
    
    vector<Ship> s;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship newShip;
    newShip.length = length;
    newShip.symbol = symbol;
    newShip.name = name;
    s.push_back(newShip);
    return true;
}

int GameImpl::nShips() const
{
    return s.size();
}

int GameImpl::shipLength(int shipId) const
{
    return s[shipId].length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return s[shipId].symbol;
}

string GameImpl::shipName(int shipId) const
{
    return s[shipId].name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1) || !p2->placeShips(b2))
        return nullptr;
    
    // determines whether to play game or stop
    bool endGame = !(b1.allShipsDestroyed() || b2.allShipsDestroyed());
    
    while(endGame)
    {
        bool shipDestroyed1 = false;
        bool shotHit1 = false;
        int shipId1 = -1;
        
        bool shipDestroyed2 = false;
        bool shotHit2 = false;
        int shipId2 = -1;
        
        
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        if(p1->isHuman())
            b2.display(true);
        else
            b2.display(false);
        
        Point p = p1->recommendAttack();
        
        bool validShot1 = b2.attack(p, shotHit1, shipDestroyed1, shipId1);
        
        p1->recordAttackResult(p, validShot1, shotHit1, shipDestroyed1, shipId1);
        
        cout << p1->name() << " attacked (" << p.r << ", " << p.c << ") and ";
        if(!validShot1)
            cout << "and wasted a shot at (" << p.r  << ", " << p.c << ")." << endl;
        else if(shotHit1 == true)
            cout << "and hit something, resulting in:" << endl;
        else
            cout << "missed, resulting in:" << endl;
        
        if(p1->isHuman())
            b2.display(true);
        else
            b2.display(false);
        
        if(b2.allShipsDestroyed() == true)
            break;

        if(shouldPause == true)
        {
            waitForEnter();
        }
        
        
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        if(p2->isHuman())
            b1.display(true);
        else
            b1.display(false);
        
        p = p2->recommendAttack();
        
        bool validShot2 = b1.attack(p, shotHit2, shipDestroyed2, shipId2);
        p2->recordAttackResult(p, validShot2, shotHit2, shipDestroyed2, shipId2);
        
        cout << p2->name() << " attacked (" << p.r << ", " << p.c << ") and ";
        if(!validShot2)
            cout << "and wasted a shot at (" << p.r  << ", " << p.c << ")." << endl;
        else if(shotHit2 == true)
            cout << "and hit something, resulting in:" << endl;
        else
            cout << "missed, resulting in:" << endl;
        
        if(p2->isHuman())
            b1.display(true);
        else
            b1.display(false);
        
        if(shouldPause == true)
        {
            waitForEnter();
        }
      
        endGame = !(b1.allShipsDestroyed() || b2.allShipsDestroyed()); // updates endGame
    }
    
    // determines which player won
    if(b1.allShipsDestroyed() == true)
    {
        if(p1->isHuman())
            b2.display(false);
        cout << p2->name() << " wins!" << endl;
        return p2;
    }
    if(b2.allShipsDestroyed() == true)
    {
        if(p2->isHuman())
            b1.display(false);
        cout << p1->name() << " wins!" << endl;
        return p1;
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

