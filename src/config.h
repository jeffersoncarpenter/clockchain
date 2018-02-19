#ifndef CONFIG_H
#define CONFIG_H

// Self-explanatory.
double targetMillisecondsPerBlock = 10000;

// Proportion of target difficulty to utilize during difficulty
// calculation.  One minus this number is proportion of previous
// difficulty to use.
double difficultyAdjustmentRate = 0.01;

// Max number of bits to add/remove to/from the difficulty requirement
// per block.
double difficultyAdjustmentClamp = 1;

#endif
