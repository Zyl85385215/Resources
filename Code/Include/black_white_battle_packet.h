#pragma once


enum BLACKWHITE_PACKET_CMD
{
	BLACK_FLAG_HURT,
	WHITE_FLAG_HURT,
	SEND_BLACK_FLAG_HURT,
	SEND_WHITE_FLAG_HURT,
	SEND_SET_BLACK_WEAK,
	SEND_SET_WHITE_WEAK,
};

struct BLACK_WHITE_BATTLE_BLACK_FLAG_HURT_Data 
{

};

struct BLACK_WHITE_BATTLE_WHITE_FLAG_HURT_Data 
{

};

struct BLACK_WHITE_BATTLE_SEND_BLACK_FLAG_HURT_Data 
{

};

struct BLACK_WHITE_BATTLE_SEND_WHITE_FLAG_HURT_Data 
{

};

struct BLACK_WHITE_BATTLE_SEND_SET_WHITE_WEAK_Data
{
	bool bIsWeak;
};

struct BLACK_WHITE_BATTLE_SEND_SET_BLACK_WEAK_Data
{
	bool bIsWeak;
};