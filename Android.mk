LOCAL_PATH := $(call my-dir)

TARGET_ARCH_ABI := arm64-v8a

include $(CLEAR_VARS)
LOCAL_MODULE := hook

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Build the modloader shared library
include $(CLEAR_VARS)
LOCAL_MODULE	        := modloader
LOCAL_SRC_FILES         := ./extern/beatsaber-hook/libs/arm64-v8a/libmodloader.so
LOCAL_EXPORT_C_INCLUDES := ./extern/beatsaber-hook/include/
include $(PREBUILT_SHARED_LIBRARY)

# Build the beatsaber-hook shared library, SPECIFICALLY VERSIONED!
include $(CLEAR_VARS)
LOCAL_MODULE	        := beatsaber-hook
LOCAL_SRC_FILES         := ./extern/beatsaber-hook/libs/arm64-v8a/libbeatsaber-hook_2019_2_1f1_0_1_2.so
LOCAL_EXPORT_C_INCLUDES := ./extern/beatsaber-hook/shared/
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
# Include the two libraries
LOCAL_SHARED_LIBRARIES := modloader
LOCAL_SHARED_LIBRARIES += beatsaber-hook
LOCAL_LDLIBS     := -llog
LOCAL_CFLAGS     := -I'c:/Program Files/Unity/Hub/Editor/2019.3.1f1/Editor/Data/il2cpp/libil2cpp'
LOCAL_MODULE     := scorepercent
LOCAL_CPPFLAGS   := -std=c++2a
LOCAL_C_INCLUDES := ./include ./src
LOCAL_SRC_FILES  += $(call rwildcard,src/,*.cpp)
include $(BUILD_SHARED_LIBRARY)