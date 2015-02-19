LOCAL_PATH := $(call my-dir)

#declare the prebuilt library
include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg-prebuilt
LOCAL_SRC_FILES := android/armv7-a/libffmpeg.so
LOCAL_EXPORT_C_INCLUDES := android/armv7-a/include
LOCAL_EXPORT_LDLIBS := android/armv7-a/libffmpeg.so
LOCAL_PRELINK_MODULE := true
include $(PREBUILT_SHARED_LIBRARY)

#the andzop library
include $(CLEAR_VARS)
LOCAL_ALLOW_UNDEFINED_SYMBOLS=false
LOCAL_MODULE := youtube-extreme
LOCAL_SRC_FILES := youtube-extreme.c http_response.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/android/armv7-a/include $(LOCAL_PATH)
LOCAL_SHARED_LIBRARY := ffmpeg-prebuilt
LOCAL_LDLIBS    := -llog -lz -lm $(LOCAL_PATH)/android/armv7-a/libffmpeg.so
include $(BUILD_SHARED_LIBRARY)
