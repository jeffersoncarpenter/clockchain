#ifndef MINE_H
#define MINE_H

#include "block.h"

double computeDifficulty(double prevDefficulty, double latestBlockTime);
void mine();

#endif
