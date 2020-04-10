#pragma once

typedef unsigned int dxGameID;
typedef unsigned int dxGameGroupID;
typedef unsigned int dxGameSideID;
typedef unsigned int dxSignUpPlayerID;

enum ESIGNUP_RESULT
{
	SIGNUP_RESULT_OK,
	SIGNUP_RESULT_GAMEGROUP_NOT_EXISTED,
	SIGNUP_RESULT_GAME_NOT_EXISTED,
	SIGNUP_RESULT_PLAYER_SIGNUP_ALREADY,
	SIGNUP_RESULT_SIDE_NOT_EXISTED,
	SIGNUP_RESULT_PLAYER_FULL,
	SIGNUP_RESULT_SIGN_UP_INSTANCE_FAILED,
};

enum EGAMEPLAYER_STATUS
{
	GAMEPLAYER_STATUS_NULL,
	GAMEPLAYER_STATUS_WAITING,
	GAMEPLAYER_STATUS_PLAYING,
};

struct dxSignUpPlayerGameStatus
{
	dxGameGroupID groupID;
	dxGameID gameID;
	dxGameSideID sideID;
	int status;
	unsigned int signUpTime;
};