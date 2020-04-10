/*
 *	copy hge 2008.12
 */

#ifndef DXRANDOM_H
#define DXRANDOM_H

class CDxRandom
{
public:
	void	Random_Seed(int seed);
	int		Random_Int(int min, int max);
	float	Random_Float(float min, float max);
};

#endif