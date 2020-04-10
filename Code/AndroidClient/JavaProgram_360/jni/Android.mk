LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := iconv
LOCAL_SRC_FILES := libiconv.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos_game_shared

LOCAL_MODULE_FILENAME := libgame

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
$(warning  $(MY_CPP_LIST))
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../android/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DataFile/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/IncludeCfg/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Include/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/MapFile/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/NetFile/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/UIFile/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp \
					$(MY_CPP_LIST:$(LOCAL_PATH)/%=%)\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
	     $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include \
		   $(NDK_ROOT)/sources/cxx-stl/stlport/stlport \
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
