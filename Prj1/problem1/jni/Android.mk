LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := syscall_ptree.c              # your source code
LOCAL_MODULE := helloARM                # output file name
LOCAL_CFLAGS += -pie -fPIE              # These two line cannot be
LOCAL_LDFLAGS += -pie -fPIE             # changed.
LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)