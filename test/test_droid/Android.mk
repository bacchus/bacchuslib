LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := NativeApp
LOCAL_SRC_FILES := nativeapp.cpp renderer.cpp gllib/gllib.cpp
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_C_INCLUDES += gllib .

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_CFLAGS := -Wall -std=gnu++11

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

