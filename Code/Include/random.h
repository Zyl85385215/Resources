#ifndef _RANDOM_H_
#define _RANDOM_H_

extern unsigned int g_seed;

void  Random_Seed(int seed);
int   Random_Int(/*int min, int max*/);
float Random_Float(float min, float max);

#endif