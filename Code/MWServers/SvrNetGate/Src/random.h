#ifndef _RANDOM_H_
#define _RANDOM_H_

int Random_Int();
float Random_Float(float min, float max);

class CRandom
{
public:
	CRandom();
	~CRandom();
	
	void  Random_Seed (int seed);
	int   Random_Int  (int min, int max);
	
private:
	unsigned int     m_wSeed;
};

#endif