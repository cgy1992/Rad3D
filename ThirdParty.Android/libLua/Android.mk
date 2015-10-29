
include $(CLEAR_VARS)

LOCAL_MODULE := libLua

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../libLua/include \
	$(LOCAL_PATH)/../libLua/source
	
LOCAL_SRC_FILES  := \
	../libLua/source/lapi.c \
    ../libLua/source/lauxlib.c \
    ../libLua/source/lbaselib.c \
    ../libLua/source/lcode.c \
    ../libLua/source/ldblib.c \
    ../libLua/source/ldebug.c \
    ../libLua/source/ldo.c \
    ../libLua/source/ldump.c \
    ../libLua/source/lfunc.c \
    ../libLua/source/lgc.c \
    ../libLua/source/linit.c \
    ../libLua/source/liolib.c \
	../libLua/source/llex.c \
	../libLua/source/lmathlib.c \
	../libLua/source/lmem.c \
	../libLua/source/loadlib.c \
	../libLua/source/lobject.c \
	../libLua/source/lopcodes.c \
	../libLua/source/loslib.c \
	../libLua/source/lparser.c \
	../libLua/source/lstate.c \
    ../libLua/source/lstring.c \
    ../libLua/source/lstrlib.c \
    ../libLua/source/ltable.c \
    ../libLua/source/ltablib.c \
    ../libLua/source/ltm.c \
    ../libLua/source/lundump.c \
    ../libLua/source/lvm.c \
    ../libLua/source/lzio.c \
    ../libLua/source/print.c

include $(BUILD_STATIC_LIBRARY)
