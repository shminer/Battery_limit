LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := jztools
LOCAL_SRC_FILES := \
	batt_limit.c utils.c jztools.c dc.c

LOCAL_CFLAGS += -pie -fPIE 
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)

APP_PLATFORM        := android-12
APP_ABI             := arm64-v8a
#APP_ABI := arm64-v8a