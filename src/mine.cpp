#include <algorithm>

#include <crypto++/sha.h>
#include <sqlite3.h>

#include "block.h"
#include "config.h"
#include "mine.h"
#include "sql.h"

using CryptoPP::SHA256;

double computeDifficulty(double prevDifficulty, double latestBlockTime) {
    double targetDifficultyFactor = (double)targetMillisecondsPerBlock / latestBlockTime;
    double targetDifficulty = prevDifficulty * targetDifficultyFactor;
    double newDifficultyPreClamp =
        prevDifficulty * (1.0 - difficultyAdjustmentRate) +
        targetDifficulty * difficultyAdjustmentRate;
    return std::min(prevDifficulty + difficultyAdjustmentClamp,
        std::max(prevDifficulty - difficultyAdjustmentClamp,
            newDifficultyPreClamp));
}

void mine() {
    byte currentHash[SHA256::DIGESTSIZE];
    Block prevBlock;
    Block block;
    double difficulty;

    getLatestBlock(&prevBlock, &difficulty);

    memcpy(block.magicNumber, magicNumber, 4);
    hash_block(block.hashPrevBlock, prevBlock);

    set_big_block_array(block.nonce, 0);
    do {
        increment_big_block_array(block.nonce);
        hash_block(currentHash, block);
    } while (true);
}
