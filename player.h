#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "stdio.h"
#include "common.h"
#include "board.h"
// #include "HashEntry.h"
// #include "HashMap.h"

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board gamebrd;
    int moveCounter;
    Side my_side, op_side;

    vector<int> heuristicValues(vector<Move*> legalMoves);
    int getMaxIndex(vector<int> moveValues);
    int positionHeuristic(Move move, Board *board);
    int frontierHeuristic(Move move, Board *newBoard);
    int mobilityHeuristic(Move move, Board *board);
    int pieceHeuristic(Move move, Board* newBoard);

    int minimax(Move *move, Board *myBoard, int depth, int ply);
    int minimaxAB(Move *move, Board *myBoard, int depth, int ply, int alpha, int beta);
    int iter_DDFS(int depth);
};

#endif
