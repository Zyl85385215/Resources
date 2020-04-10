#ifndef _CROP_PACKET_H_
#define _CROP_PACKET_H_

#define PLANT_MAX		8
#define PLANT_SUCC_MAX	2

enum {
	CROP_OPEN,
	CROP_SELECT,
};

struct CROP_CROP_OPEN_Data {
	int		nCropType;
	DWORD	dwTimeLimit;
	int		nPlant[PLANT_MAX];
};

struct CROP_CROP_SELECT_Data {
	int		nSelect;
};


#endif