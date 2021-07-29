LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := ffmpeg/lib/$(TARGET_ARCH_ABI)/libavutil.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := ffmpeg/lib/$(TARGET_ARCH_ABI)/libswscale.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := Rescale
LOCAL_LDLIBS := \
	-llog \
	-ljnigraphics \

LOCAL_SRC_FILES := rescale.c
LOCAL_SHARED_LIBRARIES := \
    avutil \
	swscale \

include $(BUILD_SHARED_LIBRARY)