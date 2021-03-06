#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define ARENA_TARGET_ALREADY_INARENA    5
#define ARENA_TARGET_NO_ENOUGHMONEY     6
#define ARENA_PLAYER_NO_ENOUGHMONEY     7
#define ARENA_PLAYER_ALREADY_INARENA    8
#define ARENA_PLAYER_TARGET_HP_PROBLEM  9
#define ARENA_NOTUSE                    10
#define FRIEND_ADD_ERROR_ADDSELF        11
#define FRIEND_ADD_ERROR_ALREADY_FRIEND 12
#define FRIEND_ADD_ERROR_ALREADY_ALLY   13
#define FRIEND_ADD_ERROR_REFUSED        14
#define MESSAGE_SEND_FAILD              15
#define BAG_REACH_MAX                   16
#define BAG_REACH_MIN                   17
#define BAG_PAGE_OVERTIME               18
#define BAG_PAGE_ADD                    19
#define BAG_FULL                        20
#define STORAGE_FULL                    21
#define STORAGE_REACH_MAX               22
#define STORAGE_REACH_MIN               23
#define STORAGE_PAGE_ADD                24
#define SYORAGE_PAGE_OVERTIME           25
#define BAITAN_CANNOT_RIDE              26
#define BAITAN_CANNOT_AREA              27
#define HOME_CREATE_ERROR_NAME          28
#define HOME_CREATE_ERROR_MONEY         29
#define HOME_CREATE_ERROR_LEVEL         30
#define HOME_CREATE_ERROR_ITEM          31
#define HOME_CREATE_ERROR_NUMBER        32
#define HOME_REBUILD_NOMONEY            33
#define HOME_CREATE_SUCCESS             34
#define HOME_ADDMEMBER_ERROR_NORIGHT    35
#define HOME_ADDMEMBER_ERROR_OTHER      36
#define HOME_ADDMEMBER_ERROR_FULL       37
#define HOME_LEAVEHOME_ERROR_SELLONESELF 38
#define HOME_LEAVEHOME_ERROR_BACKBONE   39
#define HOME_EXPEL_ERROR_NORIGHT        40
#define HOME_EXPEL_ERROR_BACKBONE       41
#define HOME_EXPEL_ERROR_POSITIONHIGH   42
#define HOME_LVLUP_ERROR_MAX            43
#define HOME_LVLUP_ERROR_MONEY          44
#define HOME_RESLVLUP_ERROR_LEVEL       45
#define HOME_RESLVLUP_ERROR_MONEY       46
#define HOME_APPOINT_MASTER             47
#define HOME_APPOINT_MEMBER             48
#define HOME_APPOINT_VICEMASTER         49
#define HOME_APPOINT_OLDMAN             50
#define HOME_APPOINT_ERROR_EXIST        51
#define HOME_DEGRADE_MEMBER             52
#define HOME_SELLONESELF_ERROR_EXISTNOW 53
#define HOME_SELLONESELF_ERROR_MASTERONLINE 54
#define HOME_OTHER_CANT_CHANGE_PLACARD  55
#define HOME_OTHER_CANT_DRAW_MONEY      56
#define HOME_OTHER_HEIR                 57
#define STATE_RIDE_CANNOT_USE           58
#define STATE_WALK_CANNOT_USE           59
#define STATE_ATTACK_CANNOT_USE         60
#define STATE_RUN_CANNOT_USE            61
#define STATE_CANNOTUSE_ITEM            62
#define GROUP_TARGET_INGROUP            63
#define GROUP_TARGET_REJUST             64
#define GROUP_TARGET_NOREPLAY           65
#define GROUP_TARGET_BATTLE             66
#define GROUP_TARGET_SELF_BATTLE        67
#define GROUP_NO_FULL                   68
#define GROUP_OUT_WHENMAP               69
#define ITEM_CANNOT_USE                 70
#define ITEM_CANNOT_TRADE               71
#define ITEM_CANNOT_STORAGE             72
#define ITEM_CANNOT_BAITAN              73
#define ITEM_CANNOT_DROP                74
#define ITEM_CANNOT_UPGRADE             75
#define ITEM_CANNOT_MAKE                76
#define ITEM_CANNOT_SPLIT               77
#define ITEM_IN_DELAY                   78
#define ITEM_NOSELL_INSHOP              79
#define ITEM_HAVE_SELLED                80
#define SLAVE_CANNOT_CAPTURE            81
#define SLAVE_CAPTURE_SUCCESS           82
#define SLAVE_CAPTURE_FILED             83
#define SLAVE_STATE_DEATH               84
#define SLAVE_ATTACK_LOOKER             85
#define SLAVE_NOMAX                     86
#define SLAVE_NOFULL                    87
#define SLAVE_CALL_SLAVE                88
#define SLAVE_NAME_ERROR                89
#define SLAVE_NOTEXIST                  90
#define SLAVE_GETAWAY                   91
#define SLAVE_MAKE_NOITEM               92
#define SLAVE_MAKEERROR_LEVEL           93
#define SKILL_USE_COOL                  94
#define SKILL_USE_NOMP                  95
#define SKILL_USE_NOFURY                96
#define SKILL_CANNOT_USE                97
#define SKILL_LESS_LEVEL                98
#define SKILL_HAVE_LEARNED              99
#define SKILL_USE_ERROR_NOT_MATCH       100
#define GAME_ICON                       101
#define SKILL_USE_ERROR_LIFESKILL_MAX   101
#define SKILL_USE_ERROR_TARGET_TYPE     102
#define SKILL_USE_ERROR_NEED_TARGET     103
#define SKILL_USE_ERROR_NO_MAP          104
#define SKILL_USE_ERROR_NO_SKILL        105
#define SKILL_USE_ERROR_NO_TARGET       106
#define SKILL_USE_ERROR_TOO_FAR         107
#define SKILL_USE_ERROR_ITEMS           108
#define SKILL_USE_ERROR_HP              109
#define SKILL_USE_ERROR_ACTION          110
#define SKILL_USE_ERROR_ATTR            111
#define SKILL_USE_ERROR_JOB             112
#define SKILL_USE_ERROR_STATE_RIDE      113
#define SKILL_USE_ERROR_STATE_RUN       114
#define SKILL_USE_ERROR_STATE_WALK      115
#define SKILL_USE_ERROR_TOSELF          116
#define SKILL_USE_ERROR_TOMONSTER       117
#define SKILL_USE_ERROR_TOPLAYER        118
#define SKILL_USE_ERROR_BOSS            119
#define SKILL_USE_ERROR_TOSUICONG       120
#define SKILL_USE_ERROR_TARGET_LVL      121
#define SKILL_USE_ERROR_UNKNOW_TARGET   122
#define SKILL_LVLUP_ERROR_SHULIAN       123
#define SKILL_LVLUP_ERROR_NOSKILL       124
#define PK_CANNOT_ATTACK_LESSLEVEL      125
#define PK_TARGET_FARAWAY               126
#define XIDIAN_COMPLETE_XIDIAN          127
#define JOB_COMPLETE                    128
#define JOB_ERROR_ITEM                  129
#define PLAYER_ARIVE_NOPILL             130
#define PLAYER_LEVEL_UPADDPOINT         131
#define PLAYER_SUNPOINT_NOTLESSTEN      132
#define TAKEOFF_ALL_FIRST               133
#define QUEST_NOFILL                    134
#define QUEST_BAGFULL_GETFIELD          135
#define QUEST_COMPLETE_FIELD_ERROR      136
#define QUEST_BAGFULL_COMPELETFIELD     137
#define QUEST_BAGFULL_GETITEMFIELD      138
#define WAR_BUILD_SUCCESS               139
#define WAR_BUILD_ERROR                 140
#define WAR_BUILD_ERROR_BEGIN_LATER     141
#define WAR_BATTLE_WAITING              142
#define WAR_APPLY_SUCCESS               143
#define WAR_APPLY_ERROR_LEVELMIN        144
#define WAR_APPLY_ERROR_LEVELMAX        145
#define WAR_ERROR_COUNTRY_ERROR         146
#define WAR_APPLY_ERROR_WAREND          147
#define WAR_APPLY_ERROR_LOSE            148
#define WAR_APPLY_ERROR_FULL            149
#define WAR_APPLY_ERROR_NOWAR           150
#define WAR_APPLY_ERROR_CHOOSEFORCE     151
#define WAR_RESULT_LEAVE_WAR            152
#define WAR_RESULT_LOSE                 153
#define WAR_ERROR_EXPLOIT_NOTENOUGH     154
#define WAR_ERROR_SAVEPOS_ERROR         155
#define WAR_OPEN_ERROR_PEOPLE_NOTENOUGH 156
#define WAR_RESULT_TIE                  157
#define MALLSHOP_CLOSE                  158
#define MALLSHOP_TARGET_NOGET           159
#define MALLSHOP_PLAYER_NOBUY           160
#define ONLINE_TIME_TIRED               161
#define ONLINE_TIME_UNHEALTHY           162
#define OTHER_GM_CANT_OPRATE            163
#define OTHER_PLAYER_NOEXIST            164
#define OTHER_OUTGAME_BYGM              165
#define OTHER_NO_CHAT                   166
#define OTHER_NOWORLD_CHAT              167
#define OTHER_NORIGHT_COME              168
#define OTHER_CANNOT_USERIDE            169
#define OTHER_YOU_ARE_NO1               170
#define OTHER_ERROR_NORIGHT             171
#define OTHER_NOENOUGH_MONEY            172
#define OTHER_ERROR_WRONG_OPERATION     173
#define SLAVE_NO_ACTIVE                 174
#define SLAVE_CANNOT_USE_ITEM           175
#define SELLBUY_ITEM_NOT_EXISTS         176
#define SELLBUY_NOT_ENOUGH_MONEY        177
#define SELLBUY_NOT_ENOUGH_SELL_RATE    178
#define SELLBUY_NOT_ENOUGH_SELL_SPACE   179
#define SELLBUY_NOT_ENOUGH_BUY_SPACE    180
#define SELLBUY_FAIL_UNKNOW             181
#define ITEM_CANNOT_AUCTION             182
#define OTHER_PLAYER_NOT_IN_MAP         183
#define SLAVE_CANNOT_PACKET             184
#define PLAYER_PK_PROTECT				185
#define WAR_APPLY_ERROR_ONBATTLE        186
#define WAR_APPLY_ERROR_WRONG_FORCE		187
#define TRADE_ERROR_NOT_ENOUGH_LEVEL_TO_BUY_GOLD_ITEM    188
#define ITEM_TRANSFER_SIX               189
#define ITEM_LVLUP_PROTECT              190
#define WAR_CANNOT_TAKE_OFF             191
#define MAP_CANNOT_JUMP_TO				192
#define GM_OPERATE_ERROR_NOPOP			193
#define SLAVE_LEVEL_ERROR               194
#define CANNOT_SET_GROUP_HEADER_IN_ZF	195
#define CANNOT_JUMP_MAP                 196
#define LOGOUT_BECAUSE_OF_USING_TOOLS	197
#define FRIEND_ADD_ERROR_ADDGM			198
#define GAME_TITLE						199
#define GAME_TITLE1						200
#define SKILL_USE_NOT_IN_FURY			201
#define SLAVE_EVOLUTION_FAILED		202
#define SLAVE_EVOLUTION_SUCCESSFUL	203
#define SLAVE_MAINATTRIB_SYNCRETIZE_FAILED		204
#define SLAVE_MAINATTRIB_SYNCRETIZE_SUCCESSFUL	205
#define SLAVE_SUBATTRIB_SYNCRETIZE_FAILED		206
#define SLAVE_SUBATTRIB_SYNCRETIZE_SUCCESSFUL	207
#define SLAVE_DIVINATORYVALUE_SYNCRETIZE_FAILED	208
#define SLAVE_DIVINATORYVALUE_SYNCRETIZE_SUCCESSFUL	209
#define CENTERSERVER_NOXUANXIUINFO				210
#define CHANGE_HOME_NAME_FAILED_NAME_EXISTED	211
#define CHANGE_HOME_NAME_FAILED_NO_DROIT		212
#define NEW_AUCTION_TRYBUG_ERROR				213
#define OTHER_OUTGAME_BYSYSTEM			214
#define BAG_NOT_ENOUGH                           215
#define FRIEND_ADD_ERROR_FRIENDLIST_FULL		216
#define HEIBAI_CANT_GROUP						217
#endif