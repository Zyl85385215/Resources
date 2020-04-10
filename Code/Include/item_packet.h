#ifndef _ITEM_PACKET_H_
#define _ITEM_PACKET_H_

#include <windows.h>
#include "groupcmd.h"
#pragma pack (push)
#pragma pack (1)


#define		ITEM_BINDSOUL_ATTRIB_PER 20 //add by hyq 2011.04.26
#define		ITEM_PLUS_VERSION 2
#define		ITEM_STAR_PROTECT_VERSION 1
// ������Ʒ���� -------------------------------------------
#define		ITEM_ID_BULLION			60151
#define		ITEM_JUMP_TO_ID_I		60155
#define		ITEM_JUMP_TO_ID_II		60156
#define		ITEM_RESET_ATTRIBUTE_ID 0
#define		RIDE_TOKEN_ID			60147			// ��ID
#define		ITEM_ID_WHITE_HORSE	    60147			// ����ID
#define		ITEM_ID_RED_HORSE	    60148			// ����ID
#define		ITEM_ID_BROWN_HORSE	    60149			// ����ID
#define		SLAVE_TOKEN_ID			60154			// �����ID
#define		ENGAGE_ID				60145			//
#define		ARRAY_ID				65535			// ��
#define		BULLION_POINT			500             //��ͨ��һ�����
#define     BULLION_POINTEX         1200            //�����̻��ĵ���
#define		BULLION_GOLD			1000000
#define     GOLD_ITEM_TRADE         30               //��Ԫ����Ʒ���׼�������

#define		REJUDGE_ITEM_ID			37235
#define		BOOST_CMD_ITEM_ID		35415
#define		CARVE_KAIKONG_ITEM_ID	35054
#define		CARVE_ITEM_ID			35055
#define		RECARVE_ITEM_ID			35056
#define		RECARVE_WING_ITEM_ID	38169
#define     USE_ITEM_POSITION
//12.9.13 �ɳ�Ч��
/*
static int ItemPlusLevelEffect[15] =
{
	10*118/100,22*118/100,35*118/100,50*118/100,85*118/100,115*118/100,150*118/100,190*118/100,235*118/100,
	310*118/100,395*118/100,490*118/100,650*118/100,800*118/100,1000*118/100,
};*/

static int ItemPlusLevelEffect[15] =
{
	18,39,64,93,128,169,218,275,343,
	423,517,628,759,914,1097,
};

static int ItemPlusProvideScores[15] = 
{
	10,
	30,
	80,
	200,
	400,
	800,
	1600,
	3200,
	6400,
	12800,
	25600,
	51200,
	102400,
	204800,
	409600,
};
static int ItemPlusProvideScores2[15] = 
{
	300,
	1008,
	2261,
	4233,
	7141,
	11259,
	16928,
	24573,
	34722,
	48028,
	65300,
	87533,
	115955,
	152072,
	197735,
};

static int ItemPlusNeedScores[16] =
{
	300,
	708,
	1253,
	1972,
	2908,
	4118,
	5669,
	7645,
	10149,
	13306,
	17272,
	22233,
	28422,
	36117,
	45663,
	0,
};
static int ItemPlusAddAttack[16] = {
	0,
	50,
	100,
	150,
	200,
	250,
	300,
	350,
	400,
	450,
	500,
	600,
	700,
	800,
	900,
	1000,
};

enum {
		GET_PLAYERITEMS,
		SEND_PLAYERITEMS,

		USE_ITEM,			//ʹ����Ʒ
		SEND_REMOVE,
		SEND_ADD,
		SEND_GOLD_CHANGED,
		
		CHANGE_SLOT,		//�ı�װ��λ��,body-bag
		SEND_SLOT_CHANGED,
		
		SEND_DURA_CHANGED,
		SEND_WEIGHT_CHANGED,
				
		THROW,
		
		SEND_GITEM_APPEAR,
		SEND_GITEM_DISAPPEAR,
		PICKUP,

		//FOR BANK
		SEND_BANK_ITEMS,
		SEND_BANK_GOLD_CHANGED,
		SEND_BANK_REMOVE,
		SEND_BANK_ADD,
		
		//����׷������
		SEND_TOKEN_INFO,	
		
		//��������
		SEND_EXTEND_INFO,
		SEND_RIDE_INFO,
		SEND_RIDE_HP,
		SEND_RIDE_EXP,
		SEND_RIDE_ONOFF,
		SEND_RIDE_DEATH,
		SEND_RIDE_SP,

		SEND_DATA,			//�Ƿ����Ⱦɫ

		RELEASE_SLAVE,		//�ͷ����
		SEND_EQUIPITEMS,
		SEND_EQUIPRIDEINFO,
		SEND_EQUIPEXTENDINFO,
		GET_EQUIPITEMSINFO,
		ACTIVE_SLAVE,
		CULTI_INFO,             //����Ȧ����----����Ȧ�鿴��ȡ��
		SUIT_EFFECT,
		SPLIT_ITEM,
		MERGE_ITEM,
		ITEM_ERROR,
		PICK_AREA,
		DECOMPOSE,
		THROWPOS,

		SEND_EQUIPEXTEND_DISAPPEAR,  //װ������������ʧ
		CANNOUSPLIT,
		
		CLEANUP,                //�������
		CLEAN_ITEMS,            //ɾ��ȫ������Ʒ
		SWITCH_ITEMS,           //�л����ĸ�����

		ITEM_POSITION,          //��������Ʒλ��
		DEADLINE_CHANGE,		//��Ʒ���ڱ��ı�
		IS_NO_TIME,				//�Ƿ����
		ITEM_UNUSE,

		BAG_PAGE,
		BANK_PAGE,

		SEND_BIND,
		SEND_UNBIND,

		EXCHANGE_POSITION,		//��Ʒ����
		TIMEOUT,			//��Ʒ����

		SURE_TO_USE,
		SEND_EQUIPBINDINFO,
		GET_ITEMBANK,
		SEND_ITEMBANK_ADD,
		SEND_ITEMBANK_TAKE,
		SEND_ITEMBANK_DEL,
		SEND_ITEM_MOHUA,

		SEND_ENCHASE_GEMS_INFO,
		SEND_ENCHASE_GEM_ADD,
		SEND_ENCHASE_GEM_DEL,
		SEND_ENCHASE_GEM_SLOT_ADD,
		SEND_EQUIP_ENCHASE_GEMS_INFO,

		SEND_ITEM_EXP,
		SEND_EQUIPEXPEXTENDINFO,
		SEND_ITEM_JUDGE,
		SEND_EQUIPJUDGE,

		SEND_PLAYER_JUDGE_ITEM,
		SEND_PLAYER_ENCHASE_ITEM,
		SEND_PLAYER_BURROW_ITEM,

		SEND_PLAYER_BIND_ITEM,
		SEND_PLAYER_UNBIND_ITEM,
		SEND_PLAYER_CANCEL_UNBIND_ITEM,
		SEND_PLAYER_CANCEL_DESTROY_ITEM,
		REQUEST_ADD_AUCTION,

		ITEMOPERATION_START,
		ITEMOPERATION_END,

		SEND_PLAYER_EXTIR_ITEM,

		ITEMOPERATION_OPEN,
		UNBINDLINE_CHANGE,

		TRUMP_CUR_POW,

		SEND_TRUMP_INFO,
		SEND_TRUMP_2OTHER_INFO,

		TRUMP_REQUEST_SKILLUP,
		TRUMP_REQUEST_POWER,
		TRUMP_REQUEST_EXPUP,
		TRUMP_REQUEST_RESETS,
		TRUMP_REQUEST_CHANGES,
		TRUMP_REQUEST_ADDSP,

		ITEM_TRUMPEX_OPEND,

		USE_ITEM_CONT,
		REQUEST_ITEM_RANK,

		USE_ITEM_END,
		SEND_FLOWER_CHANGED,
		SEND_ITEM_PLUS,
		SEND_ITEM_PLUS_2OTHER,

		BAG_CELL,            // �������ӣ�����ĸı䣬���BAG_PAGE  add by zp
		BANK_CELL,
		SEND_ITEM_STARLUCK,
		SEND_CANRIDEON_RESULT,//�������ж�������to�ͻ���
		SEND_PLAYERITEMSEX,
		REQUEST_HOME_ADD_ITEM,
		REQUEST_ADD_GOLDAUCTION,
		SEND_ITEM_ID_CHANGE,
		REQUEST_REFRESH_TRUMP,
		REQUEST_ADD_TRUMP_EXP,
		SEND_OPEN_TRUMP_REFRESH,
		TRUMP_REFRESH_FINISH,
		SEND_ITEM_GROW_INFO,
		SEND_ITEM_GROW_INFO_TO_OTHER,
		SEND_PLAYERITEMSEX_OFFLINE, //�й�
		SEND_ITEM_STAR_PROTECT,
		SEND_PLAYER_CARVE_ITEM,
		SEND_ITEM_CARVE,
		SEND_EQUIP_CARVE,
		SEND_UNCARVE,
		OPEN_MINGKE,
		SEND_EQUIP_HOLELEVEL,      //��׵ľ�������
		SEND_EQUIP_FEEDGEM,
		SEND_ITEM_FEEDGEM,

		C_ITEM_WING,
		SEND_ITEM_WING,
		SEND_ITEM_WING_POW,
		SEND_ITEM_WING_TO_OTHER,
		
		SEND_WING_TEMP_CARVE,
		SEND_CLEAR_WING_TEMP_CARVE,
		C_REPLACE_ITEM_WING,

		C_REPUTE_ITEM,
		SEND_ITEM_REPUTE,
	
		C_ITEM_HORSEEQ,
		SEND_ITEM_HORSEEQ,
		SEND_ITEM_HORSEEQ_TO_OTHER,
		SEND_ITEM_HORSEEQ_RESETVAL,
		SEND_ITEM_PLUS_EX,
		C_ITEM_UPGRADESTONE,
		S_ITEM_UPGRADESTONE,

		C_REQUEST_RIDE_CHGID,
		SEND_PLAYERITEMSEX2_OFFLINE,
		SEND_EQUIP_REPUTE,
		REQUEST_FASHION_OPER,
		SEND_PLAYER_ENCHASE_ITEM_EX,
		SEND_PLAYER_ENCHASE_ITEM_EX_TO_OTHER,
		C_SEND_ITEMS_DUP_CLEANUP,
		SEND_DUP_CLEANUP_END,

		C_NEW_MAKE,
		SEND_NEW_HORSE_ATTRIB,
		C_REQUEST_UPGRADE_HORSE,

		C_HORSE_INFO,
		C_HORSE_OPEN,
		C_HORSE_MAHUN,
		C_HORSE_MAHUNINFO,
		C_HORSE_TUNHUN,
		C_HORSE_TUNHUNBACK,
		C_HORSE_FUHUN,
		C_HORSE_FUHUNBACK,
		C_HORSE_REFTIME,
		C_HORSE_REFTIMEBACK,
		C_HunterInfo,
		C_HORSE_CHARInfo,
		C_HORSE_FangXian,
};

enum {
	RIDE_RED_HORSE,
	RIDE_WHITE_HORSE,
	RIDE_BROWN_HORSE,
};
enum
{
    TRADE_ORDINORY,					//��ͨ��Ʒ
	TRADE_SILVER,					//�󶨽�Ԫ����Ʒ
	TRADE_GOLD,						//��Ԫ����Ʒ
	TRADE_BINDGOLD,					//�󶨽�Ԫ����Ʒ
};
//��Ʒλ��
enum {
		SLOT_NULL,
		SLOT_HEAD,
		SLOT_BODY,
		SLOT_HAND,
		SLOT_FOOT,
		SLOT_WEAPON,
		SLOT_ARROW,
		SLOT_NECK,
		SLOT_RING,
		SLOT_WAIST,
		SLOT_DECK,
		SLOT_RIDE,
		SLOT_FASHION,
		SLOT_TRUMP,
		SLOT_TRUMPEX,
		SLOT_SPECIAL,
		SLOT_HAND2,
		SLOT_RIND2,
		SLOT_WING,
		SLOT_EQUIP_MAX,
		
		SLOT_RIDE_BODY=60,
		SLOT_RIDE_FOOT,
		SLOT_RIDE_LASH,
		
		SLOT_LIFE_WOOD,		 //�������ľ
		SLOT_LIFE_HERB,		 //ҩ
		SLOT_LIFE_MINE,		 //��
		SLOT_LIFE_SILK,		 //˿

		SLOT_LIFE_WEAPON,		 //����
		SLOT_LIFE_TEND,			 //����
		SLOT_LIFE_JEWELRY,		  //����
		SLOT_LIFE_DRVG,			  //��ҩ
		SLOT_AUTO_PICK,

		SLOT_FASHION_SKIN,		//��ʾʱװ
		SLOT_FASHION_BAG,		//�¹�ʱװ
		SLOT_MAX_OTHER_EQUIP,	//SLOT_EQUIP_MAX �����λ�õ���Ʒ��Ҫ����typeȡslot,ZYL
		
		SLOT_BOUGHT_STALLAGE = 0xF2,
		SLOT_NEW_STALLAGE = 0xF3, //�����߰�̯״̬ 243
		SLOT_NEW_AUCTION= 0xF4,
		SLOT_BAG		= 0xF5,	// 245
		SLOT_MAIL		= 0xF6, // 246
		SLOT_AUCTION	= 0xF7, // 247
		SLOT_ERROR		= 0xF8,	// 248
		SLOT_SELL		= 0xF9,	// 249
		SLOT_SELLED		= 0xFA,	// 250
		SLOT_GROUND		= 0xFC,
		SLOT_BAD		= 0xFD,
		SLOT_ACTIVE		= 0xFE,
		SLOT_STORE		= 0xFF,
};

#define EQUIP_SLOT_COUNT (SLOT_EQUIP_MAX - 1)
#define TRUMP_MAX_SKILLS	5

enum
{
	item_extend_type_point,
	item_extend_type_percent,
	item_extend_type_max,
};

enum
{
	item_type_other,
	item_type_weapon_single,
	item_type_weapon_double,
	item_type_armour_head,
	item_type_armour_body,
	item_type_armour_hand,
	item_type_armour_foot,	
	item_type_jewelry_neck,	//����
	item_type_jewelry_waist, //����
	item_type_jewelry_finger, //��ָ
	item_type_material_animal,
	item_type_material_plant,
	item_type_material_mine,
	item_type_material_special,	
	item_type_medicine,
	item_type_food,
	item_type_book,
	item_type_juju,
	item_type_gold,
	item_type_task,
	item_type_ride,
	item_type_slave,
	item_type_offhand,	//����
	item_type_otherweapon,	
	item_type_waist,	//����
	item_type_slavefood,
	item_type_slavemedicine,
	item_type_factory,//���ﵰ comment by hyq 2011.04.30
	item_type_big_red,
	item_type_big_blue,
	item_type_judge,
	item_type_enchase,
	item_type_burrow,
	item_type_slave_extend,
	item_type_memory_pos,
	item_type_revive,
	item_type_signal_flare,
	item_type_clarion,
	item_type_stamp,
	item_type_armour_fashion,
	item_type_extir,
	item_type_slave_blue,
	item_type_equit_trump,
	item_type_slave_big_red,
	item_type_slave_big_blue,
	item_type_equip_special,
	item_type_ride_body,
	item_type_ride_foot,
	item_type_ride_lash,
	item_type_life_wood,
	item_type_life_herb,
	item_type_life_mine,
	item_type_life_silk,
	item_type_life_weapon,
	item_type_life_tend,
	item_type_life_jewelry,
	item_type_life_drvg,
	item_type_ridefood,
	item_type_auto_pick,
	item_type_slave_book,
	item_type_wing,
	item_type_baowu,
	item_type_cuilian = 254,
	item_type_max = 255,               
};

enum
{	
	itemflag_nosave,
	itemflag_removeondeath,
	itemflag_nodrop,
	itemflag_nosell,
	itemflag_notrade,
	itemflag_noauction,
	itemflag_nobank,
	itemflag_noconfbank,
	itemflag_disppearonuse,
	itemflag_callscript,
	itemflag_savelog,      // add by dxx
	itemflag_noupgrade,
	itemflag_nolose,
	itemflag_nostallage,
	itemflag_notransfer,
	itemflag_subdura,
	itemflag_nospace,
	itemflag_getexp,
	itemflag_bejudged,
	itemflag_judge_weapon,
	itemflag_judge_armor,
	itemflag_judge_jewelry,
	itemflag_nomohua,
	itemflag_noburrow,
	itemflag_LoopUsed,
	itemflag_no_destroy_in_manual,
	itemflag_get_only_simple,
	itemflag_cannotuse_in_battle, // add by xl
	itemflag_cannotuse_in_peace,
	itemflag_max
};

enum
{
	bind_null,
	bind_heart,
	bind_equip,
	bind_soul,
	bind_pick,
	bind_lock,
	bind_max,
};

enum
{
	job_wu,
	job_jiang,
	job_xia,
	job_dao,
	job_yao,
	job_max
};

enum
{
	item_error_null,
	item_error_con,
	item_error_dex,
	item_error_str,
	item_error_wis,
	item_error_ing,
	item_error_lucky,
	item_error_level,
	item_error_notime,
	item_error_sex,
	item_error_job,
	item_error_onstate,
	item_error_ondelay,
	item_error_nobaseinfo,
	item_error_noitem,
	item_error_notinbag,
	item_error_maplimit,
	item_error_max,
};

//������ض��� add by dxx
#define _SAVE_ITEM_LOG

enum 
{
	//operation
	/*------------------------------------------------------------------*/
	//         ���Ը��ݽ���create���¼����ж���Ʒ�Ĳ�������             //

	item_action_buy_from_shop,      //�̵깺��
	item_action_take_from_quest,    //������
	item_action_take_from_war,      //ս������
	item_action_take_from_system,   //ϵͳ����
	item_action_take_from_make,     //�ϳɲ���
	item_action_take_from_pickup,   //��ȡ���
	item_action_born_from_eolith,	//ԭʯ����

	item_action_use,                //ʹ��
	item_action_throw,              //����
	item_action_levelup,            //����
	item_action_return,             //�ָ�ԭ��
	item_action_storage_in_bank,    //����ֿ�
	item_action_take_in_bank,       //�Ӳֿ���ȡ��

	item_action_rebuild,			//װ������
	item_action_transfer,			//����ת��
	item_action_bind,				//װ����
	item_action_eolith,				//ԭʯ����
	item_action_start_unbind,		//��ʼ���
	item_action_unbind_cancel,		//ȡ�����
	item_action_unbind_successful,	//���ɹ�

	item_action_mohua,				//ħ��
	item_action_mohua_fail_1,		//ħ��ʧ��,ħ���ȼ�-1
	item_action_mohua_fail_2,		//ħ��ʧ��,ħ���ȼ�����

	item_action_use_status,			//ʹ�����
	item_action_desc,			    //��������

	item_action_paimai_add,			//��������
	item_action_paimai_tuihui,		//�����˻�
	item_action_paimai_buy,			//�������
	
	//take
	item_action_take_in_trade,      //���׻��
	item_action_take_in_stallage,   //��̯���
	item_action_take_from_player,   //���ͻ��
	item_action_take_from_script,	//�ű����
	item_action_take_from_pagetrans,//���Ż��
	item_action_take_from_mail,		//�ʼ����
	item_action_take_from_split,	//�ֽ���
	item_action_take_from_master,
	//give
	item_action_give_in_trade,      //���׸���
	item_action_give_in_stallage,   //��̯����
	item_action_give_from_player,   //���͸���
	item_action_give_from_mail,		//�ʼ�����

	//destroy
	item_action_destroy_in_make,    //�ϳ�����
	item_action_destroy_in_shop,    //�̵��۳�
	item_action_destroy_in_levelup, //��������
	item_action_destroy_in_throw,  //ǿ������
	item_action_destroy_in_dura,    //�;�����
	item_action_destroy_on_ground,  //��������
	item_action_destroy_in_combin,  //�ϲ�����

	item_action_destroy_in_rebuild,	//��������
	item_action_destroy_in_transfer,//ת������
	item_action_destroy_in_bind,	//������
	item_action_destroy_in_unbind,	//�������
	item_action_destroy_in_eolith,	//ԭʯ����
	item_action_destroy_in_pandorabox,	//�o�C�����N��
	item_action_destroy_in_mohua,	//ħ������
	item_action_destroy_in_marry,	//��������
	item_action_destroy_in_pagetrans,	//����ת��
	item_action_destroy_in_createhome,	//�������
	item_action_destroy_in_quest,	//��������
	item_action_destroy_in_script,	//�ű�����
	item_action_destroy_in_skill,	//��������
	item_action_destroy_in_split,	//�ֽ�����
	item_action_destroy_in_mail,	//�ʼ�����
	item_action_destroy_in_slave_evolution,						//�������ʱ����
	item_action_destroy_in_slave_mainattrib_syncretize,			//������������Ѫʱ����
	item_action_destroy_in_slave_subattrib_syncretize,			//���︱������Ѫʱ����
	item_action_destroy_in_slave_divinatoryvalue_syncretize,	//��������������Ѫʱ����
	item_action_take_from_xuanxiu,			 //ѡ����

	item_action_throw_by_system,			 //ϵͳ����
	item_action_destroy_in_throw_by_system,  //ϵͳ��������
	item_action_destroy_in_slave_bind,		 //�����
	item_action_destroy_in_slave_unbind,	 //������
	
	item_action_destroy_in_addtime,			 //�ӳ�����

	item_action_destroy_trump,				 //��������

	item_action_gems_burrow,				 //װ�����
	item_action_gems_enchase,				 //��Ƕ��ʯ
	item_action_gems_extir,					 //ժ����ʯ
	item_action_gems_burrow_failure,		 //װ�����ʧ��
	item_action_gems_enchase_failure,		 //��Ƕ��ʯʧ��
	item_action_gems_extir_failure,			 //ժ����ʯʧ��
	item_action_gems_trump_resetskill,		 //ˢ�·�������
	item_action_gems_trump_changeskill,		 //�����������
	item_action_destroy_in_use,				 //ʹ������
	item_action_judge,						 //����
	item_action_destroy_in_createoath,		 //��������
	item_action_take_from_gems_extir,		 //ժ����ʯ���
	item_action_buy_from_buysold,
	item_action_plus_up,					 //װ���ɳ�
	item_action_plus_failed,				 //װ���ɳ�ʧ��
	item_action_take_from_itemmarket,		  //�������Ļ��
	item_action_destroy_from_itemmarket,	  //������������
	item_action_destroy_from_gemupgrade_failed,//��ʯ�ϳ�ʧ��
	item_action_plus_refresh,				  //װ��ϴ�ɳ�
	item_action_destroy_from_plus,				//�ɳ�����
	item_action_equipt_take_off,				//��װ��
	item_action_homebank_get,                 //���ֿ���
	item_action_homebank_save,                //����ֿ�
	item_action_take_from_newbe_gift,         //���ֽ���
	item_action_take_from_team_pickup,        //����ROLL���
	item_action_sys_paimai_add,						//����ϵͳ����
	item_action_timeout,					//��������
	item_action_acterget,                   //�˻�
	item_action_plus_refresh_oneparam,		//ϴ�ɳ���������
	item_action_choujiang_get,
	item_action_choujiang_take,
	item_action_carve,						//����
	item_action_hole_levelup,               //ϴ��
	item_action_feedgem,
	item_action_request,
	item_action_take_in_newstallage,           //�����°�̯
	item_action_give_in_newstallage,			//���°�̯
	item_action_back_in_newstallage,           //�°�̯ȡ��
	item_action_slave_ronghe_fail,			//�����ں�ʧ��
	item_action_newstallage_add,
	item_action_newstallage_sell,           //�չ�
	item_action_newstallage_buy,            //���չ�
	item_action_newstallage_get,            //ȡ�ر��չ���Ʒ
	item_action_replace,					//��Ʒ�һ�
	item_action_give_online,					//���߶һ�
	item_action_give_outline,					//���߶һ�
	item_action_give_purpose,					//Ŀ��ϵͳ
	item_action_gems_up,
	item_action_add_from_send_flower,
	item_action_max,
};

//���������� add by dxx
enum
{
	ITEM_BAG,
	BATTLE_BAG,
};

enum
{
	RESIST_NULL,
	RESIST_YUNXUAN ,		// �ɵֿ���ѣ
	RESIST_DINGSHENG ,		// �ɵֿ�����
	RESIST_LIUXUE ,			// �ɵֿ���Ѫ
	RESIST_BINGDONG ,		// �ɵֿ�����
	RESIST_JIANSU ,			// �ɵֿ�����
	RESIST_ZHUOSHAO ,		// �ɵֿ�����
	RESIST_PONEIFANG ,		// �ɵֿ����ڷ�
	RESIST_POWAIFANG ,		// �ɵֿ������
	RESIST_ZHONGDU ,		// �ɵֿ��ж�
	RESIST_JIHUIFU,			// �ɵֿ����ظ�
	RESIST_XVRUO,			// �ɵֿ�����
	RESIST_FENGFA,			// �ɵֿ��ⷨ
	RESIST_ZENGSHANG,		// �ɵֿ�����
	//RESIST_ADDZENGSHANG,	// �̶�����˫��
	RESIST_MAX
};

enum ACHIEVE_SOMETHING
{
	SEND_ACHIEVEMENT_INFO2C,	//�ɾ�
	SEND_LOGINGIFT_INFO2C,
	REQUEST_LOGINGIFT,
	SEND_SIMPLEINFO,
	REQUEST_PLANT,
	REQUEST_GETPLANT,
	OPEN_LOGINGIFT_MENU,
	REQUEST_ACHIEVE_AWARD,
	RETURN_ACHIEVE_RESULT,
	ASK_FOR_TREASURE,
	INVITE_FOR_TREASURE,
	TREASURE_TIME_OUT,
	ASK_TREASURE_SEEK_RESULT,
	CANCEL_TREASURE,
	TREASURE_ARRIVE,
	REQUEST_FLYBOOT,
	CHECK_FLYBOOT,
	INVITE_ALL2TREASURE,
	USE_FLYBOOT_RESULT,
	CHECK_ACHIEVE_AWORD,
	REQUEST_SET_JOB,
	REQUEST_OPEN_NEW_ACHIEVEMENT,
	RETURN_NEW_ACHIEVEMENT_INFO,
	REQUEST_FOR_ACHIEVEMENT_AWARD,
	REQUEST_OPEN_RELIGION,
	REQUEST_CHANGE_RELIGION,
};

struct DaylyGiftInfo{
	int nCnt;
	int nGold;
	DWORD itemid[3];
	BYTE itemCnt[3];
};

struct ACHIEVEMENT_REQUEST_FOR_ACHIEVEMENT_AWARD_Data{
	DWORD ID;
};

struct ACHIEVEMENT_RETURN_NEW_ACHIEVEMENT_INFO_Data
{
	int nStartPos;
	int nCurPos;
	int nPoint;
	int nDoneCnt;
	int nMaxCnt;
	DWORD dwCurID;
	BYTE bCurState;
	int nProcess;
	DWORD szID[5];
	BYTE szState[5];
	bool ifEnd;
};


struct ACHIEVEMENT_REQUEST_CHANGE_RELIGION_Data
{
	BYTE bReligion;
};

struct ACHIEVEMENT_REQUEST_OPEN_NEW_ACHIEVEMENT_Data
{
	int nStartPos;
	int nCurPos;
};

struct ACHIEVEMENT_RETURN_ACHIEVE_RESULT_Data
{
	DWORD id;
};

struct ACHIEVEMENT_REQUEST_ACHIEVE_AWARD_Data
{
	DWORD id;
};

struct ACHIEVEMENT_USE_FLYBOOT_RESULT_Data
{
	int nQid;
};

struct ACHIEVEMENT_CHECK_ACHIEVE_AWORD_Data
{
	DWORD id;
	bool ifCanAccept;
};

struct ACHIEVEMENT_REQUEST_SET_JOB_Data
{
	BYTE bJob;
};

struct ACHIEVEMENT_SEND_SIMPLEINFO_Data
{
	DWORD endtime;
	BYTE rlt;
	BYTE lvl;
	bool visable;
};

struct ACHIEVEMENT_INVITE_ALL2TREASURE_Data
{

};

struct ACHIEVEMENT_REQUEST_LOGINGIFT_Data
{
	BYTE index;
};

struct ACHIEVEMENT_SEND_LOGINGIFT_INFO2C_Data
{
	BYTE times;
	BYTE getFlag[7];
	DaylyGiftInfo giftInfo[7];
};

struct ACHIEVEMENT_SEND_ACHIEVEMENT_INFO2C_Data
{
	int nCnt;
	int nDoneCnt;
	bool isSingle;
	DWORD point;
	DWORD id;
	int nMaxCnt;
	BYTE totalId[1];
};

struct ACHIEVEMENT_CHECK_FLYBOOT_Data
{
	BYTE bFreeTimes;
	BYTE nPos;
	bool ifVip;
};

struct ACHIEVEMENT_REQUEST_FLYBOOT_Data
{
	DWORD dwQID;
	DWORD dwMapIndex;
	DWORD dwPoint;
};

struct ACHIEVEMENT_ASK_FOR_TREASURE_Data
{
	BYTE bTreasureTimes;
};

struct ACHIEVEMENT_INVITE_FOR_TREASURE_Data
{
	BYTE times;
	int nCurTreasure;
	POINT ptTreasure;
	DWORD m_dwTime;
};

struct ACHIEVEMENT_ASK_TREASURE_SEEK_RESULT_Data
{
	bool ifSuccess;
};

struct ItemCarveInfo
{
	DWORD	dwcCanUseType;
	DWORD	dwResistType[6];
	DWORD	dwResistValue[6];
	char	szCarvedName[20];
};

class HorseSoulInfo
{
public:
	DWORD dwMasterUin;
	DWORD dwIndex;
	BYTE bQuality;
	BYTE bLevel;
	BYTE bType;
	DWORD bExp;
};
class HunterInfo
{
public:
	DWORD Level;
	DWORD Exp;
};

class HorseEqInfo
{
public:
	BYTE	bLv;
	BYTE	bQuality;
	BYTE	bEffPLv[4];
	DWORD	dwEffPExp[4];
	DWORD	dwAttrib[5];
	DWORD	dwExp;
	DWORD	dwDura;			//�;�
	int		nSimpleAttrib[5];	//��������
	WORD	wFirstType;
	DWORD	dwSoul[4];
	BYTE	bQualityLevel;
	DWORD	dwQualityExp;

	//BYTE	byType[4];		//0��ʾû����,1-8��ʾ��������
	//BYTE	byLv[4];		//0��ʾû��Ƕ,����Ϊ��Ӧ�ȼ�,��������ȡ��ֵ
	//WORD	wPoint[4];		//��ǰ��ι��ֵ,������Ҫ�������ļ�
	//WORD	wCarveVal[2];	//����ת����
	//DWORD	dwTime;			//����ʱ��
	//BYTE	byQueity;		//Ʒ��
	//BYTE	byQueityPoint;	//Ʒ�ʵ�
	//BYTE	byHorseLv;		//����ȼ�
	//WORD	wHorseExp;		//���ﾭ��
	//BYTE	byAddLv;		//���Եȼ�
};

class EnchaseExInfo
{
public:
	BYTE	byType[5];		//0��ʾû����,1-8��ʾ��������
	BYTE	byLv[5];		//0��ʾû��Ƕ,����Ϊ��Ӧ�ȼ�,��������ȡ��ֵ
	BYTE	byEnchaseCount;
	BYTE	byHoldCount;
};

struct ItemReputeInfo
{
	DWORD	dwResistType[3];	//95��װ������
	DWORD	dwResistValue[3];
};

struct EQUIPITEM
{
	BYTE	slot;//λ��
	DWORD	itemID;
	WORD	Dura;
	WORD	DuraMax;
	WORD	colorIndex; //add by hh 6.11
};

struct CLIENTITEM
{
	DWORD	itemIndex;
	BYTE	slot;//λ��
	DWORD	itemID;
	WORD	Dura;
	WORD	DuraMax;
	DWORD	dwCount;
	WORD	colorIndex; //add by hh 6.11
	int		price;
	int		nDeadTime;
	int		nUnbindDeadTime;
#ifdef USE_ITEM_POSITION
	WORD    wPosition;
#endif
	BYTE    byMohuaLvl;
	BYTE	bPlusLevel;
	int		nPlusScore;
	LONGLONG llItemKey;
};

struct ItemExpExtend
{
	BYTE	bLevel;
	DWORD	dwExp;
};

struct ItemBindInfo
{
	WORD	wBind;
	DWORD	dwBindParam;
};

struct ItemGemInfo
{
	int		nSize;
	BYTE	pbData[1];
};

struct TrumpSPData
{
	BYTE	byVertion;
	LONGLONG	llItemKey;	//���ݿ�
	WORD	wSZSkills[TRUMP_MAX_SKILLS];		//���ݿ�
	WORD	wSkillPoint;		//100���ʾ1,320 ��ʾ3�㼼��+20�㾭�� ���ݿ���
	DWORD	dwCurExp;			//���ݿ�
	WORD	wMaxPower;		//���ݿ�
	WORD	wCurPower;		//���ݿ�
	WORD	wLvl;				//���ݿ�
	WORD	wID;
};

struct ItemStarProtect 
{
	ItemStarProtect()
	{
		bVersion = ITEM_PLUS_VERSION;
		for (int i=0;i<20;i++)
		{
			bFailTimes[i] = 0;
		}
	}
	BYTE bVersion;
	BYTE bFailTimes[20];
};

struct ItemPlusUpInfo
{
	ItemPlusUpInfo()
	{
		for (int i=0;i<10;i++)
		{
			wPlusParam[i] = 0;
		}
		bVersion = ITEM_PLUS_VERSION;
	}
	BYTE bVersion;
	WORD wPlusParam[10];
};


struct CLIENTITEMEX
{
	CLIENTITEM item;
	ItemPlusUpInfo plusInfo;
};

struct CLIENTITEMEX2
{
	DWORD dwUin;
	CLIENTITEM item;
	ItemPlusUpInfo plusInfo;
};

struct ItemPlusParamLvlInfo
{
	BYTE bParamLvl[5];
};

struct ItemExtendInfo		// ��Ҫ�����������ϣ���Ϊ���ø� ItemExtendMapRide & ItemExtendMapSlave ���ڴ�ռ�
{   //fix by dxx
	//Ҫ��ItemBase�е�UPö��һ�£���Ҫ�����޸�
	
	BYTE	level;      // װ���ȼ����ǣ�
	WORD	apN;
	WORD	apW;
	WORD	atkN;
	WORD	atkW;
	WORD	atkSpeed;
	WORD	moveSpeed;
	WORD	defN;
	WORD	defW;
	WORD	dpN;
	WORD	dpW;
	WORD	maxHp;
	WORD	maxMp;
	WORD    ppN;
	WORD    ppW;
	WORD    maxDura;

	//�츳����
	WORD    str;
	WORD    con;
	WORD    ing;
	WORD    dex;
	WORD    wis;
	WORD    luk;

	//��������
	BYTE	rebuildTimes;

	//��������
	BYTE	type;
};


struct ItemExtendMapRide  // ��Ҫ�����������ϣ���Ϊ���ø�ItemExtend���ڴ�ռ�
{
	BYTE	level;
	WORD	atkW;
	WORD	atkN;
	WORD	defN;
	WORD	defW;
	WORD	sinew;
	WORD	maxSinew;
	WORD	growRate;
	DWORD	exp;		// ��ӦItemExtend.maxSp & ItemExtend.maxMp ���� DWORD
};

struct ItemExtendMapSlave  // ��Ҫ�����������ϣ���Ϊ���ø�ItemExtend���ڴ�ռ�
{
	BYTE	level;
	WORD	atkW;
	WORD	atkN;
	WORD	defN;
	WORD	defW;
	WORD	ap;
	WORD	dp;
	WORD	baseIndex;
	DWORD	exp;		// ��ӦItemExtend.maxSp & ItemExtend.maxMp ���� DWORD
};

struct 	RideItemData
{
	WORD	rideType;
	WORD	maxHp;
	WORD	hp;
	WORD	sp;
	WORD	maxSp;
	DWORD	exp;
	DWORD	expMax;	
	WORD	natk;
	WORD	ndef;
	WORD	watk;
	WORD	wdef;
	BYTE	rideLevel;
};

struct SlaveItemData
{
	WORD	enemyType;
	WORD	maxHp;
	WORD	hp;
	DWORD	exp;
	DWORD	expMax;	
	BYTE	slaveLevel;
	char	name[32];
	WORD    time;
	WORD	atkW;
	WORD	atkN;
	WORD	defW;
	WORD	defN;
};

struct ExtendData
{
	//һ������
	WORD    wStr;
	WORD    wCon;
	WORD    wIng;
	WORD    wDex;
	WORD    wWis;
	WORD    wLuk;

	//��������
	WORD	wApN;
	WORD	wApW;
	WORD	wAtkN;
	WORD	wAtkW;
	WORD	wAtkSpeed;
	WORD	wMoveSpeed;
	WORD	wDefN;
	WORD	wDefW;
	WORD	wDpN;
	WORD	wDpW;
	WORD	wMaxHp;
	WORD	wMaxMp;
	WORD    wPpN;
	WORD    wPpW;
	int		nAtkYY;
	int		nDefYY;
	int		nJin;
	int		nMu;
	int		nShui;
	int		nHuo;
	int		nTu;
	WORD	wCriticalAtk;
	WORD	wCriticalDef;
	WORD	wMinHp;
	WORD	wMinMp;
	WORD	wPhysicDamage;
	WORD	wPhysicDamagePer;
	WORD	wGetPhysicDamage;
	WORD	wGetPhysicDamagePer;
	WORD	wMagicDamage;
	WORD	wMagicDamagePer;
	WORD	wGetMagicDamage;
	WORD	wGetMagicDamagePer;
	WORD	wCureEffect;
	WORD	wCureEffectPer;
	WORD	wGetCureEffect;
	WORD	wGetCureEffectPer;


	//�������ͣ���ֵor�ٷֱȣ�
	BYTE	bType;
};

struct ITEM_ITEM_ERROR_Data
{
	BYTE	bError;	
};

struct ITEM_SPLIT_ITEM_Data
{
	DWORD	dwItemIndex;
	BYTE	bCnt;
};

struct ITEM_MERGE_ITEM_Data
{
	DWORD	dwFromIndex;
	DWORD	dwToIndex;
};

struct ITEM_SEND_EQUIPITEMS_Data{
	DWORD		uin;
	BYTE		count;
	BYTE		byJob;
	BYTE		bySex;
	int			nLevel;
	char		szName[32];
	EQUIPITEM	item[1];
};
struct ITEM_SEND_PLAYERITEMS_Data {
	BYTE count;
	CLIENTITEM item[1];
};

struct ITEM_SEND_PLAYERITEMSEX_Data {
	BYTE count;
	CLIENTITEMEX item[1];
};

struct ITEM_SEND_PLAYERITEMSEX_OFFLINE_Data
{
	BYTE count;
	CLIENTITEMEX item[1];
};

struct ITEM_SEND_PLAYERITEMSEX2_OFFLINE_Data
{
	BYTE count;
	CLIENTITEMEX2 item[1];
};

struct ITEM_USE_ITEM_Data {
	DWORD	itemIndex;	
	bool	bAuto;// ϵͳ�Զ�ʹ����Ʒ�����ֶ�
};

struct ITEM_USE_ITEM_CONT_Data {
	WORD	itemID;	
	BYTE	cnt;
};

struct ITEM_REQUEST_ITEM_RANK_Data {
	DWORD itemIndex;
};

struct ITEM_ACTIVE_SLAVE_Data {
	DWORD	itemIndex;	
};
struct ITEM_TRUMP_CUR_POW_Data {
	DWORD	itemIndex;	
	WORD	wPow;
};

struct ITEM_GET_EQUIPITEMSINFO_Data{
	DWORD uin;
};

struct ITEM_SEND_ITEM_STARLUCK_Data {
	DWORD itemIndex;
	BYTE  slot;
	int	nStarLuck;
};

struct ITEM_SEND_ITEM_GROW_INFO_Data 
{
	DWORD itemIndex;
	BYTE slot;
	int nGrowLevel;
	int nGrowExp;
};

struct ITEM_SEND_ITEM_GROW_INFO_TO_OTHER_Data 
{
	DWORD dwUin;
	DWORD itemIndex;
	BYTE slot;
	int nGrowLevel;
	int nGrowExp;
};

struct ITEM_SEND_ITEM_WING_TO_OTHER_Data 
{
	DWORD	dwUin;
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bSlot;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//Ʒ��
	int		nRepairTime;	//�޸�����
	DWORD	dwQuaVal[4];	//4������
};

struct ITEM_SEND_ITEM_HORSEEQ_TO_OTHER_Data 
{
	DWORD	dwUin;
	DWORD	dwIndex;
	BYTE	bSlot;
	HorseEqInfo xHorseEqInfo;
};

struct ITEM_SEND_PLAYER_ENCHASE_ITEM_EX_TO_OTHER_Data 
{
	DWORD	dwUin;
	DWORD	dwIndex;
	BYTE	bSlot;
	EnchaseExInfo xEnchaseExInfo;
};

struct ITEM_C_ITEM_UPGRADESTONE_Data 
{
	DWORD	itemindex;
	DWORD	itemuseid;
	BYTE	byUseBind;
};

struct ITEM_C_REQUEST_RIDE_CHGID_Data 
{
	DWORD	itemindex;
	DWORD	itemuseid;
};

struct ITEM_REQUEST_FASHION_OPER_Data 
{
	BYTE	oper;
	DWORD	itemindex;
	DWORD	val;
	
};
struct ITEM_SEND_ITEM_HORSEEQ_RESETVAL_Data 
{
	DWORD	dwIndex;
	int		Val1;
	int		Val2;
};

struct ITEM_SEND_WING_TEMP_CARVE_Data
{
	DWORD	dwIndex;
	ItemCarveInfo  carveinfo;
};

struct ITEM_REQUEST_VIEW_INFO_Data{
	DWORD dwRequestUin;
};

struct ITEM_SEND_REMOVE_Data {
	DWORD	itemIndex;
	bool    blSellBuy;
};

struct ITEM_SEND_ADD_Data {
	CLIENTITEM	item;
};

struct ITEM_SEND_DATA_Data { //hh
	DWORD	itemIndex;
	WORD	wData;
};

struct ITEM_DEADLINE_CHANGE_Data
{
	DWORD dwItemIndex;
	BYTE  bSlot;
	int	  nDeadTime;
};

struct ITEM_UNBINDLINE_CHANGE_Data
{
	DWORD dwItemIndex;
	BYTE  bSlot;
	int	  nUnbindDeadTime;
};

struct ITEM_IS_NO_TIME_Data
{
	DWORD	dwItemIndex;
	BYTE	bSlot;
	BOOL	bIsNoTime;
};

struct ITEM_SEND_GOLD_CHANGED_Data {
	INT		Gold;//�ı���Gold
	INT     GoldBang;
};

struct ITEM_CHANGE_SLOT_Data {
	DWORD	itemIndex;
	BYTE	oldslot;
	BYTE	newsolot;
};

struct ITEM_ITEM_UNUSE_Data {
	DWORD	itemIndex;
	BYTE	slot;

};
struct ITEM_SEND_SLOT_CHANGED_Data {
	DWORD	itemIndex;
	BYTE	oldslot;
	BYTE	newslot;
};

struct ITEM_THROW_Data{
	DWORD	itemIndex;
};

struct ITEM_THROWPOS_Data
{
	DWORD	itemIndex;
	int		x;
	int		y;
};
struct ITEM_DECOMPOSE_Data
{
	DWORD	itemIndex;
};

struct ITEM_SEND_DURA_CHANGED_Data {
	DWORD	itemIndex;
	WORD	dura;
	WORD	duraMax;
};

struct ITEM_SEND_WEIGHT_CHANGED_Data {
	WORD	weight;
	WORD	maxWeight;
};

struct ITEM_SEND_GITEM_APPEAR_Data {
	DWORD	itemIndex;		// �ڵ�ͼ�ϵı��
	WORD	itemID;			// ��Ʒ��ID, 0xFFFF ����Ʊ������
	WORD	x;
	WORD	y;
};

struct ITEM_SEND_GITEM_DISAPPEAR_Data {
	DWORD	itemIndex;
};

struct ITEM_PICKUP_Data{
	DWORD	itemIndex;
};


//FOR BANK
struct ITEM_SEND_BANK_ITEMS_Data {
	BYTE count;
	CLIENTITEM item[1];
};

struct ITEM_SEND_BANK_GOLD_CHANGED_Data {
	INT		Gold;//�ı���Gold
};

struct ITEM_SEND_BANK_REMOVE_Data {
	DWORD	itemIndex;
};

struct ITEM_SEND_BANK_ADD_Data {
	CLIENTITEM	item;
};

struct ITEM_SEND_TOKEN_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	SlaveItemData slaveData;
};

struct ITEM_SEND_RIDE_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	RideItemData rideInfo;
};
struct ITEM_SEND_EQUIPRIDEINFO_Data {
	DWORD	uin;
	BYTE	slot;
	RideItemData rideInfo;
};
struct ITEM_SEND_RIDE_HP_Data{
	DWORD	itemIndex;
	BYTE	slot;
	WORD	hp;
};
struct ITEM_SEND_RIDE_EXP_Data{
	DWORD	itemIndex;
	BYTE	slot;
	DWORD	exp;
};
struct ITEM_SEND_RIDE_SP_Data{
	DWORD	itemIndex;
	BYTE	slot;
	WORD	sp;
};
struct ITEM_SEND_RIDE_ONOFF_Data
{
	BYTE	onoff;
};
struct ITEM_SEND_CANRIDEON_RESULT_Data
{
	bool bResult;
};
struct ITEM_SEND_RIDE_DEATH_Data
{
	BYTE	dir;
	POINT	pos;
	BYTE	type;
};
struct ITEM_SEND_EXTEND_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	ItemExtendInfo extend;
};
struct ITEM_SEND_EQUIPEXTENDINFO_Data {
	DWORD	uin;
	BYTE	slot;	
	ItemExtendInfo extend;
};
struct ITEM_SEND_EQUIPBINDINFO_Data {
	DWORD   uin;
	BYTE	slot;
	ItemBindInfo bindInfo;
};
struct ITEM_RELEASE_SLAVE_Data {
	DWORD	itemIndex;
	BYTE	slot;
};

struct ITEM_CULTI_INFO_Data {        // ��������Ȧ���������鿴����Ȧ�����ȡ��
	BYTE animal[5];
	WORD meat;	
	BYTE operation;       // 1:�鿴  2:ȡ��
	DWORD owner;
};

struct ITEM_SUIT_EFFECT_Data{
	bool flag;
	int effect;
	DWORD dwUin;
};

struct ITEM_SEND_EQUIPEXTEND_DISAPPEAR_Data{    //װ������������ʧ
	DWORD    itemIndex;
	BYTE    slot;
};

struct ITEM_SEND_BIND_Data{    //װ����
	DWORD    itemIndex;
	BYTE    slot;
	ItemBindInfo bindInfo;
};

struct ITEM_SEND_UNBIND_Data{    //װ�����
	DWORD    itemIndex;
	BYTE    slot;
};

struct CHAT_ITEM_LINK_Data{      //add by cf
	DWORD m_id;
    WORD  m_Dura;
	BOOLEAN m_bUp;
	ItemExtendInfo m_pExtData;
};

struct ITEM_CLEANUP_Data
{
	DWORD dwOldItemIndex;          //��ɾ����
	DWORD dwNewItemIndex;          //��Ʒȥ��
	WORD wNum;                   //���Ϻ�����
};

struct ITEM_CLEAN_ITEMS_Data
{
	BYTE bType;                  //��������
};

struct ITEM_SWITCH_ITEMS_Data
{
	BYTE bType;
};

//client send item's new position to server
struct ITEM_ITEM_POSITION_Data
{
	DWORD itemIndex;
	BYTE slot;
	int  newSlot;
	BYTE position;
};

struct ITEM_EXCHANGE_POSITION_Data
{
	DWORD itemIndexA;
	BYTE slotA;
	DWORD itemIndexB;
	BYTE slotB;
};

//count of bag pages
struct ITEM_BAG_PAGE_Data
{
	int nCnt;
};

struct ITEM_BAG_CELL_Data
{
	int iCellCnt;
};

struct ITEM_BANK_PAGE_Data
{
	int nCnt;
};

struct ITEM_BANK_CELL_Data
{
	int nCnt;
};

struct ITEM_TIMEOUT_Data
{
	DWORD dwIndex;
	BYTE bSlot;
};

struct ITEM_SEND_ITEMBANK_ADD_Data
{
	DWORD dwOrderNum;
	DWORD dwItemID;
	BYTE  byItemData;
};

struct ITEM_SEND_ITEMBANK_DEL_Data
{
	DWORD dwOrderNum;
};
struct ITEM_SEND_ITEMBANK_TAKE_Data
{
	DWORD dwOrderNum;
};

struct ITEM_SEND_ITEM_MOHUA_Data
{
	DWORD dwuin;
	DWORD dwIndex;
	BYTE  bSlot;
	BYTE  bLevel;
};

struct ITEM_SEND_ENCHASE_GEMS_INFO_Data
{
	BYTE	bSlot;
	DWORD	dwIndex;

	int		nSize;
	BYTE 	pbData[1];
};

struct ITEM_SEND_TRUMP_INFO_Data
{
	BYTE	bSlot;
	DWORD	dwIndex;

	TrumpSPData 	tSPData;
};

struct ITEM_SEND_ENCHASE_GEM_ADD_Data
{
	BYTE	bSlot;
	DWORD	dwIndex;

	BYTE	bPos;
	int		nGemID;
};

struct ITEM_SEND_ENCHASE_GEM_DEL_Data
{
	BYTE	bSlot;
	DWORD	dwIndex;

	BYTE	bPos;
};

struct ITEM_SEND_ENCHASE_GEM_SLOT_ADD_Data
{
	BYTE	bSlot;
	DWORD	dwIndex;
};

struct ITEM_SEND_EQUIP_ENCHASE_GEMS_INFO_Data
{
	DWORD	dwUin;

	DWORD	dwIndex;
	BYTE	bSlot;

	int		nSize;
	BYTE 	pbData[1];
};

struct ITEM_SEND_TRUMP_2OTHER_INFO_Data
{
	DWORD	dwUin;

	DWORD	dwIndex;
	BYTE	bSlot;

	BYTE 	pbData[1];
};

struct ITEM_TRUMP_REQUEST_SKILLUP_Data
{
	DWORD	dwTrumpIndx;
	WORD	wSkillID;
};

struct ITEM_TRUMP_REQUEST_POWER_Data
{
	DWORD	dwTrumpIndx;
};

struct ITEM_TRUMP_REQUEST_RESETS_Data
{
	DWORD	dwTrumpIndx;
};

struct ITEM_TRUMP_REQUEST_CHANGES_Data
{
	DWORD	dwTrumpIndx;
};

struct ITEM_TRUMP_REQUEST_ADDSP_Data
{
	DWORD	dwTrumpIndx;
};

struct ITEM_TRUMP_REQUEST_EXPUP_Data
{
	DWORD	dwTrumpIndx;
	DWORD	dwEquipIndx;
	DWORD	dwToolIndx;
};

struct ITEM_SEND_ITEM_EXP_Data
{
	DWORD	dwIndex;
	BYTE	bSlot;

	ItemExpExtend xExpExtend;

	bool	bLevelup;
};

struct ITEM_SEND_EQUIPEXPEXTENDINFO_Data
{
	DWORD	dwUin;

	DWORD	dwIndex;
	BYTE	bSlot;

	ItemExpExtend xExpExtend;
};
struct ITEM_SEND_ITEM_JUDGE_Data
{
	DWORD		dwIndex;
	BYTE		bSlot;

	ExtendData	xExtendData;
};

struct ITEM_SEND_EQUIPJUDGE_Data
{
	DWORD	dwUin;

	DWORD	dwIndex;
	BYTE	bSlot;

	ExtendData	xExtendData;
};

struct ITEM_SEND_PLAYER_JUDGE_ITEM_Data
{
	DWORD dwJudgeIndex;
	DWORD dwExpendIndex;
};

struct ITEM_SEND_PLAYER_ENCHASE_ITEM_Data
{
	DWORD dwEnchaseIndex;
	DWORD dwExpendIndex;
	DWORD dwLuckIndex;
};

struct ITEM_C_NEW_MAKE_Data
{
	DWORD dwUseItem;
	DWORD dwParts[4];
};

struct ITEM_SEND_PLAYER_BURROW_ITEM_Data
{
	DWORD dwBurrowIndex;
	DWORD dwExpendIndex;
	DWORD dwLuckIndex;
};

struct ITEM_SEND_PLAYER_EXTIR_ITEM_Data
{
	DWORD dwExtirIndex;
	DWORD dwExpendIndex;
	int	  nPos;
};

struct ITEM_SEND_PLAYER_BIND_ITEM_Data
{
	DWORD dwBindIndex;
	DWORD dwExpendIndex;
};
struct ITEM_SEND_PLAYER_UNBIND_ITEM_Data
{
	DWORD dwUnBindIndex;
	DWORD dwExpendIndex;
};
struct ITEM_SEND_PLAYER_CANCEL_UNBIND_ITEM_Data
{
	DWORD dwCancelUnBindIndex;
	DWORD dwExpendIndex;
};
struct ITEM_SEND_PLAYER_CANCEL_DESTROY_ITEM_Data
{
	DWORD dwCancelDestroyIndex;
	DWORD dwExpendIndex;
};

struct ITEM_REQUEST_ADD_AUCTION_Data
{	
	DWORD itemindex;//������Ԫ��ʱ,����ΪԪ������
	BYTE type;		//0:Ԫ��  1:���	//2:����Ԫ��
	int	price;
	int iBuyItNow;	//һ�ڼ�
};

struct ITEM_REQUEST_ADD_GOLDAUCTION_Data
{
	DWORD itemindex;
	BYTE  type;
	int   price;
	int   iBuyItNow;
};

struct ITEM_REQUEST_HOME_ADD_ITEM_Data
{
	DWORD itemindex;
};

struct ITEM_SEND_FLOWER_CHANGED_Data
{
	INT nFlower;
};

struct ITEM_SEND_ITEM_PLUS_EX_Data
{
	DWORD dwuin;
	DWORD dwIndex;
	BYTE  bSlot;
	BYTE  bLevel;
	int	  nScore;
	bool  bAddToForm;
	DWORD dwAddPlusItemId;
	ItemPlusUpInfo pUpInfo;
};

struct ITEM_SEND_ITEM_PLUS_Data
{
	DWORD dwuin;
	DWORD dwIndex;
	BYTE  bSlot;
	BYTE  bLevel;
	int	  nScore;
	bool  bAddToForm;
	DWORD dwAddPlusItemId;
	ItemPlusUpInfo pUpInfo;
};

struct ITEM_SEND_ITEM_STAR_PROTECT_Data 
{
	DWORD dwIndex;
	BYTE bSlot;
	ItemStarProtect Protect;
};

struct ITEM_SEND_ITEM_ID_CHANGE_Data 
{
	DWORD dwUin;
	DWORD dwIndex;
	BYTE bSlot;
	int	nItemID;
};

struct ITEM_REQUEST_REFRESH_TRUMP_Data 
{
	int nItemIndex;
};

struct ITEM_SEND_ITEM_PLUS_2OTHER_Data 
{
	DWORD dwuin;
	DWORD dwIndex;
	BYTE  bSlot;
	BYTE  bLevel;
	int	  nScore;
	ItemPlusUpInfo pUpInfo;
};

struct ITEM_SEND_ITEM_CARVE_Data
{
	DWORD		dwIndex;
	BYTE		bSlot;

	ItemCarveInfo xCarveInfo;
};

struct ITEM_SEND_ITEM_REPUTE_Data
{
	DWORD		dwIndex;
	BYTE		bSlot;
	DWORD		itemID;
	ItemReputeInfo xReputeInfo;
};


struct ITEM_SEND_EQUIP_CARVE_Data
{
	DWORD		dwUin;
	DWORD		dwIndex;
	BYTE		bSlot;

	ItemCarveInfo xCarveInfo;
};
struct ITEM_SEND_EQUIP_REPUTE_Data
{
	DWORD		dwUin;
	DWORD		dwIndex;
	BYTE		bSlot;

	ItemReputeInfo xReputeInfo;
};

struct ITEM_SEND_ITEM_FEEDGEM_Data
{
	DWORD    dwIndex;
	BYTE     bSlot;
	DWORD    dwValue;
};

struct ITEM_C_ITEM_WING_Data
{
	DWORD	dwIndex;
	BYTE	bOpertion;
	BYTE	bValEx;
};

struct ITEM_C_ITEM_HORSEEQ_Data
{
	DWORD	dwIndex;
	BYTE	bOpertion;
	DWORD	bValEx;
	int		bItemType;
};

struct ITEM_C_REPUTE_ITEM_Data
{
	DWORD	dwIndex;
	BYTE	bOpertion;			//0:���� 
	DWORD	val;
};

struct ITEM_C_REPLACE_ITEM_WING_Data
{
	DWORD	dwIndex;
	BYTE	bBeReplace;
};

struct ITEM_SEND_ITEM_WING_Data
{
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bSlot;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//Ʒ��
	int		nRepairTime;	//�޸�����
	DWORD	dwQuaVal[4];	//4������
	int		nSPVal;
};

struct ITEM_SEND_ITEM_HORSEEQ_Data
{
	DWORD	dwIndex;
	BYTE	bSlot;
	HorseEqInfo xHorseEqInfo;
};

struct ITEM_C_REQUEST_UPGRADE_HORSE_Data
{
	DWORD dwIndex;
	BYTE bType;
	DWORD itemID;
	BYTE bchoosetype;
	BYTE bchoosetalent;
};

struct ITEM_SEND_NEW_HORSE_ATTRIB_Data
{
	DWORD dwIndex;
	DWORD	nDura;
	DWORD dwBaseAttrib[5];
	int nSimpleAttrib[5];
};

struct ITEM_SEND_PLAYER_ENCHASE_ITEM_EX_Data
{
	DWORD	dwIndex;
	BYTE	bSlot;
	EnchaseExInfo xEnchaseExInfo;
};

struct ITEM_SEND_ITEM_WING_POW_Data
{
	DWORD	dwIndex;
	BYTE	bSlot;
	BYTE	bStepLv;
	int		nStepPow;
};

struct ITEM_SEND_PLAYER_CARVE_ITEM_Data
{
	DWORD dwCarveIndex;
	DWORD dwExpendIndex;
	BYTE	bPos;
};

struct ITEM_SEND_UNCARVE_Data{   
	DWORD    itemIndex;
	BYTE    slot;
};

struct ITEM_SEND_EQUIP_HOLELEVEL_Data{
	DWORD    itemIndex;
	BYTE	 slot;
	int	     nHoleLevel;
	int      nHoleProtect;
};

struct ITEM_SEND_EQUIP_FEEDGEM_Data{
	DWORD    dwUin;
	DWORD    itemIndex;
	BYTE     slot;
	int      nGemValue;
};

struct ITEM_C_HORSE_OPEN_Data{
	bool    Open;
};

struct ITEM_C_HORSE_INFO_Data
{
	BYTE		m_byCnt;		//���
	HunterInfo  Hunter;
	DWORD       RefreshRemainSecond;
	HorseSoulInfo _HorseSoulInfo[2];
};
struct ITEM_C_HORSE_MAHUNINFO_Data
{
	HorseSoulInfo _HorseSoulInfo;
};
struct ITEM_C_HORSE_MAHUN_Data
{
	int type;
};
struct ITEM_C_HORSE_TUNHUN_Data
{
	int num;
	int MaHunIndex1;
	int MaHunIndex2[2];
};
struct ITEM_C_HORSE_TUNHUNBACK_Data
{
	int num;
	HorseSoulInfo NewHorseSoulInfo;
	int           RemoveIndex[2];
};
struct ITEM_C_HORSE_FUHUN_Data
{
	int MaIndex;
	int MaHunIndex;
};
struct ITEM_C_HORSE_FUHUNBACK_Data
{
	int pos;
};
struct ITEM_C_HORSE_REFTIME_Data
{
	DWORD RefreshTime;
};
struct ITEM_C_HORSE_REFTIMEBACK_Data
{
	bool  bReflesh;
	BYTE  YuanBaoNum;
};
struct ITEM_C_HunterInfo_Data
{
	HunterInfo Hunter;
};
struct ITEM_C_HORSE_CHARInfo_Data
{
	int TiLi;
};
#pragma pack (pop)

#endif