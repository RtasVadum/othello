#include "player.h"
using namespace std;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    counter = 0;
    my_side = side;
    if (side == WHITE)
    {
        op_side = BLACK;
    }
    else
    {
        op_side = WHITE;
    }
 
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}



/*
 * Destructor for the player.
 */
Player::~Player() {
}


/* 
 * Calculates the Heuristic value for each possible legal move that could be
 * made on the board.
 * Currently a very basic implementation depending only on board position.
 */
vector<int> Player::heuristicValues(vector<Move*> legalMoves)
{
    vector<int> moveValues;
    Move move = Move(-1, -1);
    int moveValue;
    int movePos;
    for (unsigned int i = 0; i < legalMoves.size(); i++)
    {
        moveValue = 1;
        move = *legalMoves[i];
        movePos = move.getX() + 8 * move.getY();
        if (movePos == 8 || movePos == 1 || movePos == 48 || movePos == 6 || 
            movePos == 57 || movePos == 15 || movePos == 62 || movePos == 55)
        {
            moveValue *= -3; // cells on edges adjacent to corners not good
        }
        else if (movePos == 0 || movePos == 56 || movePos == 7 || movePos == 63)
        {
            moveValue *= 10; // corners good
        }
        else if (move.getX() == 0 || move.getY() == 0)
        {
            moveValue *= 3; // other edges good
        }
        else if (movePos == 9 || movePos == 49 || movePos == 14 || movePos == 54)
        {
            moveValue *= -5; // cells diagonally adjacent to corners bad
        }
        moveValues.push_back(moveValue);
    }
    return moveValues;
}

/*
 * Finds the maximum element of a list and returns the index of that element.
 */
int Player::getMaxIndex(vector<int> moveValues)
{
    int index = 0;
    int max = moveValues[0];
    for (unsigned int i = 1; i < moveValues.size(); i++)
    {
        if (moveValues[i] > max)
        {
            index = i;
            max = moveValues[i];
        }
    }
    return index;
}


/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    // make a small change
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     
    vector<Move*> legalMoves;
    vector<int> moveValues;
    Move* myMove;

    gamebrd.doMove(opponentsMove, op_side); // make the opponent's move
    std::cerr << "black: " << gamebrd.countBlack() << std::endl;
    std::cerr << "white: " << gamebrd.countWhite() << std::endl;
    
    legalMoves = gamebrd.getLegalMoves(my_side);
    std::cerr << "black2: " << gamebrd.countBlack() << std::endl;
    std::cerr << "white2: " << gamebrd.countWhite() << std::endl;
    
    if (legalMoves.size() != 0) // if there are legal moves available
    {
        moveValues = heuristicValues(legalMoves);
        myMove = legalMoves[getMaxIndex(moveValues)]; // determine which move to make
        gamebrd.doMove(myMove, my_side); // perform the move
        for (unsigned int i = 0; i < legalMoves.size(); i++)
        {
            if (legalMoves[i]->getX() != myMove->getX() || 
                legalMoves[i]->getY() != myMove->getY())
            {
                delete legalMoves[i]; // free memory for other moves
            }
        }
        return myMove;
    }

    // std::cerr << "11111" << std::endl;;
    return NULL;
}

