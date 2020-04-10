#ifndef _ENTIREFFECT_PACKET_H
#define _ENTIREFFECT_PACKET_H
#pragma pack (push)
#pragma pack (1)

enum
{
	ADD_ENTIREFFECT,
	DEL_ENTIREFFECT
};
enum
{
	ENTIR_TYPE_MALARIA,
	ENTIR_TYPE_YANHUA,
	ENTIR_TYPE_CENTER,
};
struct ENTIREFFECT_ADD_ENTIREFFECT_Data {
	WORD type;
};

struct ENTIREFFECT_DEL_ENTIREFFECT_Data {
	WORD type;
};

#pragma pack (pop)
#endif