#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " the human must place 5 ships." << endl;
    b.display(false);
    for (int n = 0; n < game().nShips(); n++)
    {
        Direction dir;
        while(true) // loops until human player enters either h or v
        {
            cout << "Enter h or v for direction of " << game().shipName(n) << " (length " << game().shipLength(n) <<"): " ;
            char d;
            cin >> d;
            if(d == 'h')
            {
                dir = HORIZONTAL;
                break;
            }
            else if(d == 'v')
            {
                dir = VERTICAL;
                break;
            }
            else
            {
                cout << "Direction must be h or v." << endl; // prints this and goes back to the beginning of loop
            }
        }
        
        while(true) // loops until human player enters a valid row and column
        {
            int row;
            int col;
            cout << "Enter row and column of leftmost cell (e.g. 3 5): " ;
            if(getLineWithTwoIntegers(row, col))
            {
                if(b.placeShip(Point(row, col), n, dir)) // checks if ship fits in the spot
                {
                    break; // if fits, break
                }
                else
                {
                    cout << "The ship can not be placed there." << endl; // if not, goes back to beginning
                }
            }
            else
            {
                cout << "You must enter two integers." << endl; // if anything other 2 ints are entered, goes back
            }
        }
        b.display(false);
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    while(true)
    {
        int row;
        int col;
        cout << "Enter the row and column to attack (e.g, 3 5): ";
        if(getLineWithTwoIntegers(row, col))
        {
            m_lastCellAttacked.r = row;
            m_lastCellAttacked.c = col;
            
            break;
        }
        else
        {
            cout << "You must enter two integers." << endl;
        }
    }
    
    return m_lastCellAttacked;
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // none
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    // none
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
    Point m_lastCellAttacked;
    bool placeRecursively(Board& b, int shipId);
    int state;
    
    vector<Point> unused; // unused points to choose from
    vector<Point> unused2; // unused points only 4 points away
    
    // compares two points (see if row and col match)
    struct isEqual
    {
        isEqual(const Point& w):wanted(w) { }
        Point wanted;
        bool operator () (const Point& p)
        {
            return { p.r == wanted.r && p.c == wanted.c };
        }
    };

};


MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{
    // pushes all points on board into unused
    for(int r = 0; r < game().rows(); r++) {
        for(int c = 0; c < game().cols(); c++) {
            Point newP;
            newP.r = r;
            newP.c = c;
            unused.push_back(newP);
        }
    }
    state = 1;
}

bool MediocrePlayer::placeShips(Board& b)
{
    int shipsToBePlaced = 0; // starts from position 0 of the vector of ships
    
    int counter = 1; // counts up to 50 tries
    while(counter != 50)
    {
        b.block();
        if(!placeRecursively(b, shipsToBePlaced)) // calls helper function
            counter++; // increments counter if helper function returns false
        else
        {
            b.unblock(); // if helper functions returns true, unblock and return true
            return true;
        }
        b.unblock();
    }
    return false;
}

bool MediocrePlayer::placeRecursively(Board& b, int shipId)
{
    if(shipId == game().nShips()) // finished placing all ships
        return true;
    
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i, j), shipId, HORIZONTAL)) // recursively tries to place all ships
            {
                if(placeRecursively(b, shipId + 1)) // if ships can be placed return true
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(Point(i, j), shipId, HORIZONTAL); // unplaces previous ships
                }
            }
            else if(b.placeShip(Point(i, j), shipId, VERTICAL)) // same but for vertical
            {
                if(placeRecursively(b, shipId + 1))
                {
                        return true;
                }
                else
                {
                    b.unplaceShip(Point(i, j), shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    if(state == 2) // when the player hits a ship
    {
        Point p;
        
        if(unused2.size() == 0) // if all points in the selected set of points are used
        {
            // push all points 4 units away from it into a new vector
            
            for(int i = -4; i <= 4; i++)
            {
                p.r = m_lastCellAttacked.r + i;
                p.c = m_lastCellAttacked.c;
                if(p.r < 0)
                    continue;
                for(int index = 0; index < unused.size(); index++) // checks to see if point has already been used
                {
                    if(p.r == unused[index].r && p.c == unused[index].c)
                        unused2.push_back(p);
                }
            }
        
            for(int i = -4; i <= 4; i++)
            {
                p.r = m_lastCellAttacked.r;
                p.c = m_lastCellAttacked.c + i;
                if(p.c < 0)
                    continue;
                for(int index = 0; index < unused.size(); index++)
                {
                    if(p.r == unused[index].r && p.c == unused[index].c)
                        unused2.push_back(p);
                }
            }
        }
        
        // if there are still any surrounding points
        
        if(!unused2.empty())
        {
            int index = rand() % unused2.size(); // chooses a random point from that vector
            m_lastCellAttacked.r = unused2[index].r;
            m_lastCellAttacked.c = unused2[index].c;
            
            // find index in unused vector
            
            auto i = find_if(unused.begin(), unused.end(), isEqual(m_lastCellAttacked));
            int index1 = distance(unused.begin(), i);
            
            // erase point from both vectors
            
            if(index != unused2.size())
                unused2.erase(unused2.begin() + index);
            if(index1 != unused.size())
                unused.erase(unused.begin() + index1);
        }
        
        // if there are no more surrounding points (in case of ship length > 5)
        
        else
        {
            state = 1;
        }
    }
    
    if(state == 1) // chooses a random point
    {
        int index = rand() % unused.size();
        m_lastCellAttacked.r = unused[index].r;
        m_lastCellAttacked.c = unused[index].c;
        unused.erase(unused.begin() + index); //erases chosen point from the vector
    }
    
    return m_lastCellAttacked;
}


void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId)
{
    if(validShot == true)
    {
        if(state == 1 && shotHit) // if player hits something
        {
            state = 2;
        }
        if(shotHit && shipDestroyed) // if player destroys a ship
        {
            state = 1;
            unused2.clear();
        }
    }
}
void MediocrePlayer::recordAttackByOpponent(Point p)
{
    // none
}


//*********************************************************************
//  GoodPlayer
//*********************************************************************


class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
    Point firstHitCell; // stores point of ship that is first hit to check points around it

    int state;
    
    bool standardGame; // checks if the game is a standard battleship game
    bool switchSides; // checks both sides of the ship
    bool skipPoint; // skips point if ship is still not destroyed
    bool goBack; // goes back to destroy any missed ships
    
    int counter; // counts how many times sides are switched
    int hitID; // stores shipID of the ship that was first hit
    
    vector<Point> unused; // unused points to choose from
    vector<Point> unused2; // unused points only 1 point away
    vector<Point> unusedDiagonal; // points at a diagonal
    
    vector<Point> missedPoints; // ships that have not been destroyed but have been attacked at a point
    vector<int> missedShipIDs; // the corresponding IDs of missedPoints
    
    bool placeRecursively(Board& b, int shipId);
    
    // compares two points (checks if row and col match)
    struct isEqual
    {
        isEqual(const Point& w):wanted(w) { }
        Point wanted;
        bool operator () (const Point& p)
        {
            return { p.r == wanted.r && p.c == wanted.c };
        }
    };
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{
    // pushes all points on the board into unused
    
    for(int r = 0; r < game().rows(); r++) {
        for(int c = 0; c < game().cols(); c++) {
            Point newP;
            newP.r = r;
            newP.c = c;
            unused.push_back(newP);
        }
    }
    state = 1;
    switchSides = false;
    counter = 0;
    goBack = false;
    skipPoint = false;

}

bool GoodPlayer::placeShips(Board& b)
{
    // checks if there are 5 ships under the length 5
    // checks for 10 rows and columns
    
    if(game().nShips() == 5 && game().rows() == 10 && game().cols() == 10)
    {
        for(int i = 0; i < game().nShips(); i++){
            if(game().shipLength(i) <= 5){
                standardGame = true;
            }
            else
                standardGame = false;
        }
    }
    
    if(standardGame) // if it is a standard battleship game, manually place ships
    {
        b.placeShip(Point(5, 5), 0, VERTICAL); // A
        b.placeShip(Point(0, 5), 1, VERTICAL); // B
        b.placeShip(Point(4, 7), 2, HORIZONTAL); // D
        b.placeShip(Point(4, 0), 3, HORIZONTAL); // S
        b.placeShip(Point(5, 4), 4, VERTICAL); // P
        
        return true;
    }
    
    else // if not a standard battleship game, place ships the same way as Mediocre Player
    {
        int shipsToBePlaced = 0;
        
        int counter = 1;
        while(counter != 50)
        {
            b.block();
            if(!placeRecursively(b, shipsToBePlaced))
                counter++;
            else
            {
                b.unblock();
                return true;
            }
            b.unblock();
        }
        return false;
    }
}

bool GoodPlayer::placeRecursively(Board& b, int shipId)
{
    if(shipId == game().nShips())
        return true;
    
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i, j), shipId, HORIZONTAL))
            {
                if(placeRecursively(b, shipId + 1))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(Point(i, j), shipId, HORIZONTAL);
                }
            }
            else if(b.placeShip(Point(i, j), shipId, VERTICAL))
            {
                if(placeRecursively(b, shipId + 1))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(Point(i, j), shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}


Point GoodPlayer::recommendAttack()
{
    // if the player hits 2 points in a row
    
    if(state == 3)
    {
        Point p;
        
        if(firstHitCell.r == m_lastCellAttacked.r) // if direction is horizontal
        {
            if(firstHitCell.c < m_lastCellAttacked.c) // if it is on the right
            {
                p.r = m_lastCellAttacked.r; // check all right side first
                p.c = m_lastCellAttacked.c + 1;
                
                auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                if(distance(unused.begin(), i) == unused.size())
                {
                    switchSides = true;
                }
                
                if(switchSides) // reaches end of ship, switch to left side
                {
                    p.r = m_lastCellAttacked.r;
                    p.c = firstHitCell.c - 1;
                    switchSides = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        skipPoint = true; // skips point if already used
                    }
                }
                
                if(skipPoint) // checks next point
                {
                    p.r = m_lastCellAttacked.r;
                    p.c = firstHitCell.c - 2;
                    skipPoint = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        state = 2; // revert back to state 2
                    }
                }
                
                if(p.r < 0 || p.c < 0 || p.r >= game().rows() || p.c >= game().cols()) // if out of bound
                {
                    state = 1;
                }
                
            }
            if(firstHitCell.c > m_lastCellAttacked.c) // if it is one to the left
            {
                p.r = m_lastCellAttacked.r; // checks all left side first
                p.c = m_lastCellAttacked.c - 1;
                
                auto i = find_if(unused.begin(), unused.end(), isEqual(p));
                if(distance(unused.begin(), i) == unused.size())
                {
                    switchSides = true;
                }
                
                if(switchSides) // reaches end of ship, switch to right side
                {
                    p.r = m_lastCellAttacked.r;
                    p.c = firstHitCell.c + 1;
                    switchSides = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        skipPoint = true; // skips point if already used
                    }
                }
                
                if(skipPoint)
                {
                    p.r = m_lastCellAttacked.r;
                    p.c = firstHitCell.c + 2;
                    skipPoint = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        state = 2;
                    }
                }
                
                if(p.r < 0 || p.c < 0 || p.r >= game().rows() || p.c >= game().cols()) // if out of bound
                {
                    state = 1;
                }

            }
        }
        else if(firstHitCell.c == m_lastCellAttacked.c) // if direction is vertical
        {
            if(firstHitCell.r < m_lastCellAttacked.r) // if it is one up
            {
                p.r = m_lastCellAttacked.r + 1; // check all above first
                p.c = m_lastCellAttacked.c;
                
                auto i = find_if(unused.begin(), unused.end(), isEqual(p));
                if(distance(unused.begin(), i) == unused.size())
                {
                    switchSides = true;
                }

                
                if(switchSides) // finished checking above, switches to down
                {
                    p.r = firstHitCell.r - 1;
                    p.c = m_lastCellAttacked.c;
                    switchSides = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        skipPoint = true;
                    }
                }
                
                if(skipPoint)
                {
                    p.r = firstHitCell.r - 2;
                    p.c = firstHitCell.c;
                    skipPoint = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        state = 2;
                    }
                }
                
                if(p.r < 0 || p.c < 0 || p.r >= game().rows() || p.c >= game().cols())
                {
                    state = 1;
                }
            }
            if(firstHitCell.r > m_lastCellAttacked.r) // if it is one down
            {
                p.r = m_lastCellAttacked.r - 1;
                p.c = m_lastCellAttacked.c;
                
                auto i = find_if(unused.begin(), unused.end(), isEqual(p));
                if(distance(unused.begin(), i) == unused.size())
                {
                    switchSides = true;
                }
                
                if(switchSides) // finished checking down, switches to up
                {
                    p.r = firstHitCell.r + 1;
                    p.c = m_lastCellAttacked.c;
                    switchSides = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        skipPoint = true;
                    }
                }
                
                if(skipPoint)
                {
                    p.r = firstHitCell.r + 2;
                    p.c = firstHitCell.c;
                    skipPoint = false;
                    
                    auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
                    if(distance(unused.begin(), i) == unused.size())
                    {
                        state = 2;
                    }
                }
                
                if(p.r < 0 || p.c < 0 || p.r >= game().rows() || p.c >= game().cols())
                {
                    state = 1;
                }
            }
        }
        
        m_lastCellAttacked.r = p.r;
        m_lastCellAttacked.c = p.c;
        
        auto i = find_if(unused.begin(), unused.end(), isEqual(p)); // check if point is already used
        if(distance(unused.begin(), i) == unused.size())
        {
            state = 2; // reverts back to state 2 if points are all used
        }
        else
        {
            // erases chosen point from both vectors
            
            auto i = find_if(unused.begin(), unused.end(), isEqual(m_lastCellAttacked));
            int index1 = distance(unused.begin(), i);
            if(index1 != unused.size())
                unused.erase(unused.begin() + index1);
            
            auto j = find_if(unused2.begin(), unused2.end(), isEqual(m_lastCellAttacked));
            int index2 = distance(unused2.begin(), j);
            if(index2 != unused2.size())
                unused2.erase(unused2.begin() + index2);
        }
        
    }
    
    // if the player hits one point
    
    if(state == 2)
    {
        Point p;
        if(unused2.empty()) // if all points in the selected set of points are used
        {
            // pushes all points surrounding it into a new vector
            
            for(int i = -1; i <= 1; i++)
            {
                p.r = m_lastCellAttacked.r + i;
                p.c = m_lastCellAttacked.c;
                if(p.r < 0 || p.r > game().rows())
                    continue;
                
                for(int index = 0; index < unused.size(); index++)
                {
                    if(p.r == unused[index].r && p.c == unused[index].c)
                        unused2.push_back(p);
                }
            }
            
            for(int i = -1; i <= 1; i++)
            {
                p.r = m_lastCellAttacked.r;
                p.c = m_lastCellAttacked.c + i;
                if(p.c < 0 || p.c > game().cols())
                    continue;
                
                for(int index = 0; index < unused.size(); index++)
                {
                    if(p.r == unused[index].r && p.c == unused[index].c)
                        unused2.push_back(p);
                }
            }
            
            // store previous point into firstHitCell
            
            firstHitCell.r = m_lastCellAttacked.r;
            firstHitCell.c = m_lastCellAttacked.c;
        }
        
        //choose random point from unused2
        
        if(!unused2.empty())
        {
            int index = rand() % unused2.size();
            m_lastCellAttacked.r = unused2[index].r;
            m_lastCellAttacked.c = unused2[index].c;
            
            // erases chosen point from both vectors
            
            auto i = find_if(unused.begin(), unused.end(), isEqual(m_lastCellAttacked));
            int index1 = distance(unused.begin(), i);
            if(index1 != unused.size())
                unused.erase(unused.begin() +   index1);
            
            auto j = find_if(unused2.begin(), unused2.end(), isEqual(m_lastCellAttacked));
            int index2 = distance(unused2.begin(), j);
            if(index2 != unused2.size())
                unused2.erase(unused2.begin() + index2);
        }
    }
    
    if(state == 1)
    {
        // chooses points from the two diagonals across the boards
        
        if(game().rows() == game().cols())
        {
            for(int r = 0; r < game().rows(); r++) {
                for(int c = 0; c < game().cols(); c++) {
                    if(r == c || (r+c) == (r+c-2)/2)
                    {
                        Point newP;
                        newP.r = r;
                        newP.c = c;
                        
                        // only pushes point into vector if it is unused
                        
                        auto j = find_if(unused.begin(), unused.end(), isEqual(newP));
                        int index = distance(unused.begin(), j);
                        if(index != unused.size())
                        {
                            unusedDiagonal.push_back(newP);
                        }
                    }
                }
            }
        }
        
        // chooses random point from vector of points in diagonals
        
        if(!unusedDiagonal.empty())
        {
            m_lastCellAttacked.r = unusedDiagonal[0].r;
            m_lastCellAttacked.c = unusedDiagonal[0].c;
            
            // erase point from vector of points
            
            auto j = find_if(unused.begin(), unused.end(), isEqual(m_lastCellAttacked));
            int index = distance(unused.begin(), j);
            if(index != unused.size())
                unused.erase(unused.begin() + index);
            
            // clear diagonal vector
            
            unusedDiagonal.clear();
        }
        
        // if there are no more points left in diagonals
        
        else
        {
            // chooses random point with at least one point next to it
            
            while(true)
            {
                int index = rand() % unused.size();
                m_lastCellAttacked.r = unused[index].r;
                m_lastCellAttacked.c = unused[index].c;
                
                Point top;
                top.r = m_lastCellAttacked.r + 1;
                top.c = m_lastCellAttacked.c;
                
                Point bottom;
                bottom.r = m_lastCellAttacked.r - 1;
                bottom.c = m_lastCellAttacked.c;
                
                Point left;
                bottom.r = m_lastCellAttacked.r;
                bottom.c = m_lastCellAttacked.c - 1;
                
                Point right;
                bottom.r = m_lastCellAttacked.r;
                bottom.c = m_lastCellAttacked.c + 1;
                
                // if there is at a point on top, bottom, left, or right, then break
                
                auto j = find_if(unused.begin(), unused.end(), isEqual(top));
                int i = distance(unused.begin(), j);
                if(i != unused.size())
                {
                    unused.erase(unused.begin() + index);
                    break;
                }
                
                j = find_if(unused.begin(), unused.end(), isEqual(bottom));
                i = distance(unused.begin(), j);
                if(i != unused.size())
                {
                    unused.erase(unused.begin() + index);
                    break;
                }
                
                j = find_if(unused.begin(), unused.end(), isEqual(right));
                i = distance(unused.begin(), j);
                if(i != unused.size())
                {
                    unused.erase(unused.begin() + index);
                    break;
                }
                
                j = find_if(unused.begin(), unused.end(), isEqual(left));
                i = distance(unused.begin(), j);
                if(i != unused.size())
                {
                    unused.erase(unused.begin() + index);
                    break;
                }
                else // if there are no remaining points that satisfy this condition, revert to state 0
                {
                    state = 0;
                    break;
                }
            }
        }
    }
    if(state == 0)
    // chooses random point from vector of unused points
    {
        int index = rand() % unused.size();
        m_lastCellAttacked.r = unused[index].r;
        m_lastCellAttacked.c = unused[index].c;
        unused.erase(unused.begin() + index);
    }
    
    return m_lastCellAttacked;
}




void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId)
{
    if(!validShot) // if it is of the board or already attacked that side
    {
        if(p.r >= game().rows() || p.c >= game().cols() || p.r < 0 || p.c < 0) // out of bounds
            switchSides = true;
        else // if not out of bounds, skip the already shot point
            skipPoint = true;
    }
    
    else if(validShot)
    {
        if((state == 1 && shotHit) || (state == 0 && shotHit)) // first hit shot
        {
            hitID = shipId; // store shipId
            state = 2;
        }
        else if (state == 2 && shotHit) // hits 2 in a row
        {
            state = 3;
            if(game().shipSymbol(shipId) != game().shipSymbol(hitID)) // if they are different symbols
            {
                goBack = true;
                state = 2;
                
                //store point and shipID into vectors
                
                missedPoints.push_back(m_lastCellAttacked);
                missedShipIDs.push_back(shipId);
            }
        }

        else if (state == 3 && !shotHit) // misses a shot when in state 3
        {
            switchSides = true; // switches to other side
            counter++;
        }
        else if (state == 3 && shotHit)
        {
            if(game().shipSymbol(shipId) != game().shipSymbol(hitID))
            {
                missedPoints.push_back(m_lastCellAttacked);
                missedShipIDs.push_back(shipId);
                
                goBack = true;
                state = 2;
            }
        }
        
        if(state == 3 && counter == 2) // if it searched both sides
        {
            counter = 0;
            switchSides = false;
            
        }
        
        if(shotHit && shipDestroyed) // if ship is destroyed
        {
            if(!missedPoints.empty()) // if there are missed ships, goes back to state 2 for that point stored
            {
                unused2.clear();
                m_lastCellAttacked = missedPoints[0];
                firstHitCell = missedPoints[0];
                hitID = missedShipIDs[0];
                state = 2;
                
                missedPoints.erase(missedPoints.begin());
                missedShipIDs.erase(missedShipIDs.begin());
            }
            else // if there are no missed ships, go back to state 1
            {
                state = 1;
                switchSides = false;
                counter = 0;
                unused2.clear();
                skipPoint = false;
            }
        }
    }
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    return;
}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
