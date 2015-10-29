

include $(CLEAR_VARS)

LOCAL_MODULE := libFreeType
#LOCAL_LDLIBS := -ldl -lstdc++

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"
LOCAL_CFLAGS += -O2

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ThirdParty/freetype/builds/unix \
	$(LOCAL_PATH)/../../ThirdParty/freetype/include

LOCAL_SRC_FILES := \
	../../ThirdParty/freetype/src/base/ftbbox.c \
	../../ThirdParty/freetype/src/base/ftbitmap.c \
	../../ThirdParty/freetype/src/base/ftglyph.c \
	../../ThirdParty/freetype/src/base/ftlcdfil.c \
	../../ThirdParty/freetype/src/base/ftstroke.c \
	../../ThirdParty/freetype/src/base/ftxf86.c \
	../../ThirdParty/freetype/src/base/ftbase.c \
	../../ThirdParty/freetype/src/base/ftsystem.c \
	../../ThirdParty/freetype/src/base/ftinit.c \
	../../ThirdParty/freetype/src/base/ftgasp.c \
	../../ThirdParty/freetype/src/raster/raster.c \
	../../ThirdParty/freetype/src/sfnt/sfnt.c \
	../../ThirdParty/freetype/src/smooth/smooth.c \
	../../ThirdParty/freetype/src/autofit/autofit.c \
	../../ThirdParty/freetype/src/truetype/truetype.c \
	../../ThirdParty/freetype/src/cff/cff.c \
	../../ThirdParty/freetype/src/psnames/psnames.c \
	../../ThirdParty/freetype/src/pshinter/pshinter.c

include $(BUILD_STATIC_LIBRARY)
