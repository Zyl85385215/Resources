#ifndef _COUNTRY_PACKET_H_
#define _COUNTRY_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define MAX_NAMELEN (20)
#define MAX_COUNTRY_POWER (100000)

#define MAX_APPOINTTIMES (6)
#define COUNTRYPOWER_FOR_APPOINTTIMES(power) (1 + (power * 0.1f))
#define MAX_DECREETIMES_PERDAY (3)

enum {
	WEAK_COUNTRYINDEX,
	COUNTRY_HONOUR,
	COUNTRY_NEWINFO,

	COUNTRY_SET_KINGUIN,
	COUNTRY_SET_PROTECTOR1UIN,
	COUNTRY_SET_PROTECTOR2UIN,
	COUNTRY_SET_GENUIN,
	COUNTRY_SET_COUNTRYPOWER,
	COUNTRY_CHANGE_COUNTRYPOWER,
	COUNTRY_SET_APPOINTTIMES,
	COUNTRY_SET_COUNTRYGOLD,

	COUNTRY_CHANGE_COUNTRYGOLD,
	COUNTRY_SET_DECREE,
	COUNTRY_APPOINT_REQUEST,
	COUNTRY_IMPOSE,

	COUNTRY_IMPOSE_RESULT,

	COUNTRY_SET_KINGUIN_RESULT,
	COUNTRY_SET_PROTECTOR1UIN_RESULT,
	COUNTRY_SET_PROTECTOR2UIN_RESULT,
	COUNTRY_SET_GENUIN_RESULT,

	COUNTRY_ADD_COUNTRYKINGINFO_RESULT,
	COUNTRY_CLEAR_COUNTRYKINGINFO,
	COUNTRY_CLOSE_COUNTRYSYSTEMMENU,
	COUNTRY_OPEN_COUNTRYSYSTEMMENU,

	COUNTRY_SET_HOMEID,
	COUNTRY_SET_PLAYER_COUNTRYJOB,
};

enum
{
	COUNTRY_JOB_NULL,
	COUNTRY_JOB_KING,
	COUNTRY_JOB_PROTECTOR1,
	COUNTRY_JOB_PROTECTOR2,
	COUNTRY_JOB_GEN,
};

#define COUNTRY_JOB_COUNT (COUNTRY_JOB_GEN + 1)

struct CountryKingInfo
{
	int		nAge;
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
	DWORD	dwTickCnt;
};

struct COUNTRY_GOURP_WEAK_COUNTRYINDEX_Data {
	BYTE byCountyIndex;
	BYTE byStrongCountryIndex;
};

struct COUNTRY_GOURP_COUNTRY_HONOUR_Data {
	BYTE bCountry;
	int	 nHonour;
};

struct COUNTRY_GOURP_COUNTRY_NEWINFO_Data
{
	BYTE	bCountryID;
	int		iCountryHonour;

	DWORD	dwKingUin;
	DWORD	dwProtector1Uin, dwProtector2Uin;
	DWORD	dwGenUin;

	char	szKingName[MAX_NAMELEN];
	char	szProtector1Name[MAX_NAMELEN], szProtector2Name[MAX_NAMELEN];
	char	szGenName[MAX_NAMELEN];

	int		iHomeID;
	char	szHomeName[MAX_NAMELEN];

	int		iCountryPower;
	int		iAppointTimes;
	int		iCountryGold;

	int		iAge;
	DWORD	dwTickCnt;
	
	char	szDecree[1];
};

struct COUNTRY_GOURP_COUNTRY_SET_KINGUIN_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
};

struct COUNTRY_GOURP_COUNTRY_SET_PROTECTOR1UIN_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
};

struct COUNTRY_GOURP_COUNTRY_SET_PROTECTOR2UIN_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
};

struct COUNTRY_GOURP_COUNTRY_SET_GENUIN_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
};

struct COUNTRY_GOURP_COUNTRY_SET_KINGUIN_RESULT_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
	int		nAge;
	DWORD	dwTickCnt;
	int		nHomeID;
	char	szHomeName[MAX_NAMELEN];
};

struct COUNTRY_GOURP_COUNTRY_SET_PROTECTOR1UIN_RESULT_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
};

struct COUNTRY_GOURP_COUNTRY_SET_PROTECTOR2UIN_RESULT_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
};

struct COUNTRY_GOURP_COUNTRY_SET_GENUIN_RESULT_Data
{
	BYTE	bCountryID;
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
};

struct COUNTRY_GOURP_COUNTRY_SET_COUNTRYPOWER_Data
{
	BYTE	bCountryID;
	int		iCountryPower;
};

struct COUNTRY_GOURP_COUNTRY_CHANGE_COUNTRYPOWER_Data
{
	BYTE	bCountryID;
	int		iCountryPower;
};

struct COUNTRY_GOURP_COUNTRY_SET_APPOINTTIMES_Data
{
	BYTE	bCountryID;
	int		iAppointTimes;
};

struct COUNTRY_GOURP_COUNTRY_SET_COUNTRYGOLD_Data
{
	BYTE	bCountryID;
	int		iCountryGold;
};

struct COUNTRY_GOURP_COUNTRY_CHANGE_COUNTRYGOLD_Data
{
	DWORD	dwUin;
	BYTE	bCountryID;
	int		iCountryGold;
};

struct COUNTRY_GOURP_COUNTRY_SET_DECREE_Data
{
	DWORD	dwUin;
	BYTE	bCountryID;
	char	szDecree[1];
};

struct COUNTRY_GOURP_COUNTRY_ADD_COUNTRYKINGINFO_RESULT_Data
{
	BYTE	bCountryID;
	int		nCnt;
	CountryKingInfo xCountryKingInfo[1];
};

struct COUNTRY_GOURP_COUNTRY_SWITCH_COUNTRYINFO_LISTENER_Data
{
	bool	bConnected;
};

struct COUNTRY_GOURP_COUNTRY_APPOINT_REQUEST_Data
{
	DWORD	dwUin;
	BYTE	bCountryID;
	int		nCountryJob;
	char	szName[MAX_NAMELEN];
};

struct COUNTRY_GOURP_COUNTRY_IMPOSE_Data
{
	DWORD	dwUin;
	BYTE	bCountryID;
};

struct COUNTRY_GOURP_COUNTRY_IMPOSE_RESULT_Data
{
	BYTE	bCountryID;
	int		nCountryGold;
	int		nCountryPower;
};

struct COUNTRY_GOURP_COUNTRY_CLEAR_COUNTRYKINGINFO_Data
{
	BYTE	bCountryID;
};

struct COUNTRY_GOURP_COUNTRY_SET_HOMEID_Data
{
	BYTE	bCountryID;
	int		nHomeID;
};

struct COUNTRY_GOURP_COUNTRY_SET_PLAYER_COUNTRYJOB_Data
{
	int		nCountryJob;
};

#pragma pack (pop)
#endif