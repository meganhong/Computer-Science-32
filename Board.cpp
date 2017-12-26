#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <vector>
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    char board[MAXROWS][MAXCOLS];
    const Game& m_game;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    // creates an empty board
    for(int i = 0; i < m_game.rows(); i++)
        for(int j = 0; j < m_game.cols(); j++)
            board[i][j] = '.';
}

void BoardImpl::clear()
{
    for(int i = 1; i < m_game.rows(); i++)
        for(int j = 1; j < m_game.cols(); j++)
            board[i][j] = '.'; // puts dots in the entire board
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                board[r][c] = '@'; // puts a character to block
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(board[r][c] == '@')
                board[r][c] = '.'; //replaces blocks with dots
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    // checks to see if shipId is invalid
    if(shipId < 0 || shipId >= m_game.nShips())
        return false;
    
    // checks to see if point is out of bounds
    if(topOrLeft.r < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c < 0 || topOrLeft.c >= m_game.cols())
        return false;
    
    if(dir == VERTICAL)
    {
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()) // if entire ship fits
            return false;
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            if(board[i][topOrLeft.c] != '.') // if ship is overlapping another one
                return false;
        }
    }
    if(dir == HORIZONTAL)
    {
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) // if entire ship fits
            return false;
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
        {
            if(board[topOrLeft.r][i] != '.') // if ship is overlapping another one
                return false;
        }
    }
    
    // if all tests passed, place ship on the board
    
    if(dir == VERTICAL)
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
            board[i][topOrLeft.c] = m_game.shipSymbol(shipId);
    
    if(dir == HORIZONTAL)
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
            board[topOrLeft.r][i] = m_game.shipSymbol(shipId);
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    // checks to see if shipId is invalid
    if(shipId < 0 || shipId >= m_game.nShips())
        return false;
    
    // checks to see if point is out of bounds
    if(topOrLeft.r < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c < 0 || topOrLeft.c >= m_game.cols())
        return false;
    
    // checks if the entire ship is there
    if(dir == VERTICAL)
    {
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            if(board[i][topOrLeft.c] != m_game.shipSymbol(shipId))
                return false;
        }
    }

    if(dir == HORIZONTAL)
    {
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
            return false;
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
        {
            if(board[topOrLeft.r][i] != m_game.shipSymbol(shipId))
                return false;
        }
    }
    
    // if all tests passed, remove ship
    
    if(dir == VERTICAL)
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
            board[i][topOrLeft.c] = '.';
    
    if(dir == HORIZONTAL)
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
            board[topOrLeft.r][i] = '.';
    
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    if(shotsOnly == false)
    {
        cout << "  ";
        for(int i = 0; i < m_game.cols(); i++)
            cout << i;
        cout << endl;
        for(int i = 0; i < m_game.rows(); i++)
        {
            for(int j = 0; j < m_game.cols(); j++)
            {
                if(j == 0)
                {
                    cout << i;
                    cout << " ";
                }
                cout << board[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }
    else
    {
        cout << "  ";
        for(int i = 0; i < m_game.cols(); i++)
            cout << i;
        cout << endl;
        for(int i = 0; i < m_game.rows(); i++)
        {
            for(int j = 0; j < m_game.cols(); j++)
            {
                if(j == 0)
                {
                    cout << i;
                    cout << " ";
                }
                if(board[i][j] == 'X' || board[i][j] == '.' || board[i][j] == 'o')
                   cout << board[i][j];
                else
                    cout << '.';
            }
            cout << endl;
        }
        cout << endl;
    }

}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if(p.r >= m_game.rows() || p.c >= m_game.cols() || p.r < 0 || p.c < 0)
        return false;
    
    if(board[p.r][p.c] == 'o') // already shot at this point
        return false;
    if(board[p.r][p.c] == 'X')
        return false;
    
    if(board[p.r][p.c] == '.') // missed
    {
        board[p.r][p.c] = 'o';
        shotHit = false;
        return true;
    }
    else
    {
        shotHit = true;

        char shipHit = board[p.r][p.c]; // store in temp variable
        board[p.r][p.c] = 'X'; // change boat's symbol to X for damaged on board
        bool shipExists = false;
        for(int i = 0; i < m_game.rows(); i++)
        {
           for(int j = 0; j < m_game.cols(); j++)
           {
               if(board[i][j] == shipHit) // finds a matching letter
               {
                   shipExists = true; 
                   shipDestroyed = false;
                   break;
               }
           }
        }
        
        if (!shipExists) // if no matching letters are found, ship is destroyed
        {
            shipDestroyed = true;
        }
        for(int i = 0; i < m_game.nShips(); i++) // set shipId to the ship hit/destroyed
            if(shipHit == m_game.shipSymbol(i))
                shipId = i;
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    // checks to see if there are no characters other than o, X, or .
    for(int i = 0; i < m_game.rows(); i++)
        for(int j = 0; j < m_game.cols(); j++)
            if(board[i][j] != '.' && board[i][j] != 'o' && board[i][j] != 'X')
                return false;
  
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}