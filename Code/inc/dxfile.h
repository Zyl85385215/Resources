#ifndef DXFILE_H
#define DXFILE_H

class CDxFile
{
public:
	void *	LoadResource(const char * filename);
	void	FreeResource(void * p);
};

#endif