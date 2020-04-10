LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := iconv
LOCAL_SRC_FILES := libiconv.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos_game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../android/JniUtil.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameScene.cpp\
                   ../../Classes/GlobalFunction.cpp\
                   ../../Classes/LoginScene.cpp\
                   ../../Classes/ReconnectGame.cpp\
                   ../../Classes/SoundCenter.cpp\
                   ../../Classes/UpdateScene.cpp\
                   ../../Classes/xnExtstr.cpp\
                   ../../Classes/xnlist.cpp\
                   ../../Classes/DataFile/AmzDataBase.cpp\
                   ../../Classes/DataFile/BuffObj.cpp\
                   ../../Classes/DataFile/ClientChat.cpp\
                   ../../Classes/DataFile/ClientFlag.cpp\
                   ../../Classes/DataFile/ClientQuest.cpp\
                   ../../Classes/DataFile/ClientAchievement.cpp\
                   ../../Classes/DataFile/EquipData.cpp\
                   ../../Classes/DataFile/FamilyCenter.cpp\
                   ../../Classes/DataFile/FortCenter.cpp\
                   ../../Classes/DataFile/GameActivity.cpp\
                   ../../Classes/DataFile/GameAwardData.cpp\
                   ../../Classes/DataFile/HorseData.cpp\
                   ../../Classes/DataFile/ItemData.cpp\
                   ../../Classes/DataFile/RankData.cpp\
                   ../../Classes/DataFile/RelationshipData.cpp\
                   ../../Classes/DataFile/SkillData.cpp\
                   ../../Classes/DataFile/SlaveData.cpp\
                   ../../Classes/DataFile/StateData.cpp\
                   ../../Classes/DataFile/UserData.cpp\
                   ../../Classes/IncludeCfg/ActivedegreeCfg.cpp\
                   ../../Classes/IncludeCfg/AchievementCfg.cpp\
                   ../../Classes/IncludeCfg/EnemyConfig.cpp\
                   ../../Classes/IncludeCfg/EnemyDropConfig.cpp\
                   ../../Classes/IncludeCfg/EquipConfig.cpp\
                   ../../Classes/IncludeCfg/ExpMultipleCfg.cpp\
                   ../../Classes/IncludeCfg/FaceConfig.cpp\
                   ../../Classes/IncludeCfg/GameAwardCfg.cpp\
                   ../../Classes/IncludeCfg/GameActivityCfg.cpp\
                   ../../Classes/IncludeCfg/HorseConfig.cpp\
                   ../../Classes/IncludeCfg/ItemConfig.cpp\
                   ../../Classes/IncludeCfg/MallConfig.cpp\
                   ../../Classes/IncludeCfg/MapConfig.cpp\
                   ../../Classes/IncludeCfg/MapCopyConfig.cpp\
                   ../../Classes/IncludeCfg/MapStaticCfg.cpp\
                   ../../Classes/IncludeCfg/MentoringCfg.cpp\
                   ../../Classes/IncludeCfg/QuestConfig.cpp\
                   ../../Classes/IncludeCfg/SkillConfig.cpp\
                   ../../Classes/IncludeCfg/SkinConfig.cpp\
                   ../../Classes/IncludeCfg/StateConfig.cpp\
                   ../../Classes/IncludeCfg/SlaveConfig.cpp\
                   ../../Classes/IncludeCfg/StarSysConfig.cpp\
                   ../../Classes/IncludeCfg/TestAwardCfg.cpp\
                   ../../Classes/IncludeCfg/TestLibraryCfg.cpp\
                   ../../Classes/IncludeCfg/TextConfig.cpp\
                   ../../Classes/IncludeCfg/TitleConfig.cpp\
                   ../../Classes/IncludeCfg/UserConfig.cpp\
                   ../../Classes/IncludeCfg/VipConfig.cpp\
                   ../../Classes/MapFile/EffectObj.cpp\
                   ../../Classes/MapFile/EffectSprite.cpp\
                   ../../Classes/MapFile/GuideCenter.cpp\
                   ../../Classes/MapFile/GuideLayer.cpp\
                   ../../Classes/MapFile/MapAIObj.cpp\
                   ../../Classes/MapFile/MapControl.cpp\
                   ../../Classes/MapFile/MapLayer.cpp\
                   ../../Classes/MapFile/MapMouse.cpp\
                   ../../Classes/MapFile/MapObj.cpp\
                   ../../Classes/MapFile/MapObjManagers.cpp\
                   ../../Classes/MapFile/MapOthers.cpp\
                   ../../Classes/MapFile/MapPlayer.cpp\
                   ../../Classes/MapFile/MapSprite.cpp\
                   ../../Classes/MapFile/MapTile.cpp\
                   ../../Classes/MapFile/MObjActControl.cpp\
                   ../../Classes/MapFile/ParticleCenter.cpp\
                   ../../Classes/MapFile/ParticleLayer.cpp\
                   ../../Classes/NetFile/ODSocket.cpp\
                   ../../Classes/NetFile/PacketCenter.cpp\
                   ../../Classes/NetFile/SocketManager.cpp\
                   ../../Classes/UIFile/ActivityMenu.cpp\
                   ../../Classes/UIFile/AutoMenu.cpp\
                   ../../Classes/UIFile/AchievementMenu.cpp\
                   ../../Classes/UIFile/BagMenu.cpp\
                   ../../Classes/UIFile/BankMenu.cpp\
                   ../../Classes/UIFile/ChatMenu.cpp\
                   ../../Classes/UIFile/ControlMenu.cpp\
                   ../../Classes/UIFile/CreateRoleMenu.cpp\
                   ../../Classes/UIFile/CtrlObj.cpp\
                   ../../Classes/UIFile/EquipStarMenu.cpp\
                   ../../Classes/UIFile/EquipDuraMenu.cpp\
                   ../../Classes/UIFile/FriendMenu.cpp\
                   ../../Classes/UIFile/FriendChooseMenu.cpp\
                   ../../Classes/UIFile/FamilyMenu.cpp\
                   ../../Classes/UIFile/GetNumMenu.cpp\
                   ../../Classes/UIFile/GameAwardMenu.cpp\
                   ../../Classes/UIFile/GreenModeMenu.cpp\
                   ../../Classes/UIFile/HorseObjMenu.cpp\
                   ../../Classes/UIFile/Interface.cpp\
                   ../../Classes/UIFile/InterfaceLayer.cpp\
                   ../../Classes/UIFile/ItemInfoMenu.cpp\
                   ../../Classes/UIFile/LoginMenu.cpp\
                   ../../Classes/UIFile/LuckBetMenu.cpp\
                   ../../Classes/UIFile/MainMenu.cpp\
                   ../../Classes/UIFile/MailMenu.cpp\
                   ../../Classes/UIFile/MallMenu.cpp\
                   ../../Classes/UIFile/ManorMenu.cpp\
                   ../../Classes/UIFile/MapCopyMenu.cpp\
                   ../../Classes/UIFile/McAwardMenu.cpp\
                   ../../Classes/UIFile/MiniRadio.cpp\
                   ../../Classes/UIFile/MissonMenu.cpp\
                   ../../Classes/UIFile/MyTextInput.cpp\
                   ../../Classes/UIFile/MentoringMenu.cpp\
                   ../../Classes/UIFile/MakeLayer.cpp\
                   ../../Classes/UIFile/MakeSkillMenu.cpp\
                   ../../Classes/UIFile/MarryMenu.cpp\
                   ../../Classes/UIFile/NASaleMenu.cpp\
                   ../../Classes/UIFile/PetObjMenu.cpp\
                   ../../Classes/UIFile/RankMenu.cpp\
                   ../../Classes/UIFile/SelLineMenu.cpp\
                   ../../Classes/UIFile/ShopMenu.cpp\
                   ../../Classes/UIFile/SkillMenu.cpp\
                   ../../Classes/UIFile/SkillSPMenu.cpp\
                   ../../Classes/UIFile/StarSysMenu.cpp\
                   ../../Classes/UIFile/SlaveFormCtrl.cpp\
                   ../../Classes/UIFile/StoneComposeMenu.cpp\
                   ../../Classes/UIFile/TeamMenu.cpp\
                   ../../Classes/UIFile/TxtLinkView.cpp\
                   ../../Classes/UIFile/UserParamMenu.cpp\
                   ../../Classes/UIFile/VipMenu.cpp\
                   ../../Classes/UIFile/WTestMenu.cpp\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
		   $(LOCAL_PATH)/../../android \
		   $(LOCAL_PATH)/../../ThirdPlat \
		   $(LOCAL_PATH)/../../Classes/DataFile \
		   $(LOCAL_PATH)/../../Classes/UIFile \
		   $(LOCAL_PATH)/../../Classes/IncludeCfg \
		   $(LOCAL_PATH)/../../Classes/MapFile \
		   $(LOCAL_PATH)/../../Classes/NetFile \
		   $(LOCAL_PATH)/../../Classes/Include \
		   $(LOCAL_PATH)/../../../cocos2d-2.0-x-2.0.3/cocos2dx/platform/android/jni \

LOCAL_CFLAGS := -w

LOCAL_SHARED_LIBRARIES := iconv

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static


LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_CPPFLAGS += -fexceptions

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android)\


$(call import-module,cocos2dx)\
$(call import-module,extensions)
