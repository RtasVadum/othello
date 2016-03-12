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
    moveCounter = 0;
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
        Board* newBoard = gamebrd.copy();
        move = *legalMoves[i];
        newBoard->doMove(&move, my_side);
        moveValue = 1;
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
        // include factor that handles piece count
        int countDiff = newBoard->countBlack() - newBoard->countWhite();
        if (my_side == WHITE) // if player is white, take negative of piece count
        {
            countDiff *= -1;
        }
        if (moveCounter < 15) // "evaporation" strategy in early game
        {
            moveValue -= 5 * countDiff;
        }
        else
        {
            moveValue += 5 * countDiff;
        }
        moveValues.push_back(moveValue);
        delete newBoard;
    }
    return moveValues;
}

// BELOW HEURISTIC FUNCTIONS NOT YET IMPLEMENTED/INCLUDED IN ACTUAL CALCULATION
/*
 * Part of the Heuristic calcaultion that considers objective board position.
 */
int Player::positionHeuristic(Move move)
{
    int boardPos[64] = {
         4, -3,  2,  2,  2,  2, -3,  4,
        -3, -4, -1, -1, -1, -1, -4, -3,
         2, -1,  1,  0,  0,  1, -1,  2,
         2, -1,  0,  1,  1,  0, -1,  2,
         2, -1,  0,  1,  1,  0, -1,  2,
         2, -1,  1,  0,  0,  1, -1,  2,
        -3, -4, -1, -1, -1, -1, -4, -3,
         4, -3,  2,  2,  2,  2, -3,  4,
    };
    int movePos = move.getX() + 8 * move.getY();

    return boardPos[movePos];
}

/*
 * Part of the Heuristic calcaultion that considers whether the piece
 * can be recaptured at a later time.
 */
int Player::stabilityHeuristic(Move move)
{
    return 0;
}

/*
 * Part of the Heuristic calcaultion that considers how many moves would
 * be possible in the new board position.
 */
int Player::mobilityHeuristic(Move move)
{
    return 0;
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
 * Implementation of a minimax algorithm used to determine which board
 * move to make.
 */
int Player::minimax(Move *move, Board *myBoard, int depth, int ply)
{
    vector<Move*> possMoves;
    if (ply > 0) // if my player's move
    {
        possMoves = myBoard->getLegalMoves(my_side);
    }
    else // opposing player's move
    {
        possMoves = myBoard->getLegalMoves(op_side);
    }

    if (depth <= 0 || possMoves.size() == 0) // base case
    {
        // get value of move
        vector<Move*> singleMove;
        singleMove.push_back(move);
        vector<int> moveValue = heuristicValues(singleMove);
        return moveValue[0] * ply;
    }

    int a = -1000000; // very low value, effectively negative infinity

    if (ply > 0) // if my player's move
    {
        myBoard->doMove(move, my_side); // make move to update board
    }
    else // opposing player's move
    {
        myBoard->doMove(move, op_side); // make move to update board
    }

    for (unsigned int i = 0; i < possMoves.size(); i++) // recursive step
    {
        a = max(a, -minimax(possMoves[i], myBoard, depth - 1, -ply));
    }
    return a; 
}


/*
 * Implementation of a minimax algorithm that also uses alpha-beta pruning
 * to determine which move to make.
 */
int Player::minimaxAB(Move *move, Board *myBoard, int depth, int ply, int alpha, int beta)
{
    vector<Move*> possMoves; // possible moves that could be made
    if (ply > 0) // if my player's move
    {
        possMoves = myBoard->getLegalMoves(my_side);
    }
    else // opposing player's move
    {
        possMoves = myBoard->getLegalMoves(op_side);
    }

    if (depth <= 0 || possMoves.size() == 0) // base case
    {
        // get value of move
        vector<Move*> singleMove;
        singleMove.push_back(move);
        vector<int> moveValue = heuristicValues(singleMove);
        return moveValue[0] * ply;
    }

    int score = -1000; // very low value, effectively negative infinity

    if (ply > 0) // if my player's move
    {
        myBoard->doMove(move, my_side); // make move to update board
    }
    else // opposing player's move
    {
        myBoard->doMove(move, op_side); // make move to update board
    }

    for (unsigned int i = 0; i < possMoves.size(); i++) // recursive step
    {
        score = -minimaxAB(possMoves[i], myBoard, depth - 1, -ply, -beta, -alpha);
        if (score > alpha)
        {
            alpha = score;
        }
        if (score >= beta)
        {
            break;
        }
    }
    return score; 
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
    // std::cerr << "black: " << gamebrd.countBlack() << std::endl;
    // std::cerr << "white: " << gamebrd.countWhite() << std::endl;
    
    legalMoves = gamebrd.getLegalMoves(my_side); // get possible moves
    // std::cerr << "black2: " << gamebrd.countBlack() << std::endl;
    // std::cerr << "white2: " << gamebrd.countWhite() << std::endl;
    
    if (legalMoves.size() != 0) // if there are legal moves available
    {
        Board* myBoard = gamebrd.copy();
        for (unsigned int i = 0; i < legalMoves.size(); i++)
        {
            // arbitrarily set minimax search depth to 3
            //moveValues.push_back(minimax(legalMoves[i], myBoard, 3, 1));
            moveValues.push_back(minimaxAB(legalMoves[i], myBoard, 5, 1, -1000000, 1000000));
        }
        int index = getMaxIndex(moveValues);
        myMove = legalMoves[index];

        gamebrd.doMove(myMove, my_side); // perform the move

        for (unsigned int i = 0; i < legalMoves.size(); i++)
        {
            if (legalMoves[i]->getX() != myMove->getX() || 
                legalMoves[i]->getY() != myMove->getY())
            {
                delete legalMoves[i]; // free memory for other moves
            }
        }
        delete myBoard;
        moveCounter++;
        std::cerr << moveCounter << std::endl;
        return myMove;
    }

    // std::cerr << "11111" << std::endl;;
    return NULL;
}

