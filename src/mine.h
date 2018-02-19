#ifndef MINE_H
#define MINE_H

#include "block.h"

double computeDifficulty(double prevDefficulty, Block prevBlock, Block block);
void mine();

#endif
