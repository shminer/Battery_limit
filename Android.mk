LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := batt_limit
LOCAL_SRC_FILES := batt_limit.c

LOCAL_CFLAGS += -pie -fPIE 
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)

APP_PLATFORM        := android-12
APP_ABI             := arm64-v8a
#APP_ABI := arm64-v8a