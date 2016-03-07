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
     
    vector<Move*> rand_moves;
    Move* myMove;

    gamebrd.doMove(opponentsMove, op_side); // make the opponent's move
    std::cerr << "black: " << gamebrd.countBlack() << std::endl;
    std::cerr << "white: " << gamebrd.countWhite() << std::endl;
    rand_moves = gamebrd.legalMoves(my_side);
    
    std::cerr << "black2: " << gamebrd.countBlack() << std::endl;
    std::cerr << "white2: " << gamebrd.countWhite() << std::endl;
    
    if (rand_moves.size() != 0) // if there are legal moves available
    {
        myMove = rand_moves[0]; // determine which move to make
        gamebrd.doMove(myMove, my_side); // perform the move
        for (unsigned int i = 0; i < rand_moves.size(); i++)
        {
            if (rand_moves[i]->x != myMove->x || rand_moves[i]->y != myMove->y)
            {
                delete rand_moves[i]; // free memory for other moves
            }
        }
        return myMove;
    }

    // std::cerr << "11111" << std::endl;;
    return NULL;
     
}

