#ifndef _SPECIAL_EFFECT_
#define _SPECIAL_EFFECT_
enum
{
	EFFECT_ENEMY,
	EFFECT_PLAYER,
	EFFECT_POINT,
	EFFECT_CHUNK,
	EFFECT_SLAVE,
};

struct SPECIALEFFECT_EFFECT_PLAYER_Data {
	DWORD	uin;
	DWORD	tagUin;
	WORD	effectID;
};

struct SPECIALEFFECT_EFFECT_ENEMY_Data {
	DWORD	uin;
	DWORD	enemyIndex;
	WORD	effectID;
};
struct SPECIALEFFECT_EFFECT_SLAVE_Data {
	DWORD	uin;
	DWORD	slaveIndex;
	WORD	effectID;
};
struct SPECIALEFFECT_EFFECT_CHUNK_Data {
	DWORD	uin;
	DWORD	chunkIndex;
	WORD	effectID;
};

struct SPECIALEFFECT_EFFECT_POINT_Data {
	DWORD	uin;
	WORD	x, y;
	WORD	effectID;
};
#endif