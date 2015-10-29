
include $(CLEAR_VARS)

LOCAL_MODULE := libVorbis

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ThirdParty/ogg/include \
	$(LOCAL_PATH)/../../ThirdParty/ogg/source/vorbis

LOCAL_SRC_FILES := \
        ../../ThirdParty/ogg/source/vorbis/mdct.c \
        ../../ThirdParty/ogg/source/vorbis/smallft.c \
        ../../ThirdParty/ogg/source/vorbis/block.c \
        ../../ThirdParty/ogg/source/vorbis/envelope.c \
        ../../ThirdParty/ogg/source/vorbis/window.c \
        ../../ThirdParty/ogg/source/vorbis/lsp.c \
        ../../ThirdParty/ogg/source/vorbis/lpc.c \
        ../../ThirdParty/ogg/source/vorbis/analysis.c \
        ../../ThirdParty/ogg/source/vorbis/synthesis.c \
        ../../ThirdParty/ogg/source/vorbis/psy.c \
        ../../ThirdParty/ogg/source/vorbis/info.c \
        ../../ThirdParty/ogg/source/vorbis/floor1.c \
        ../../ThirdParty/ogg/source/vorbis/floor0.c \
        ../../ThirdParty/ogg/source/vorbis/res0.c \
        ../../ThirdParty/ogg/source/vorbis/mapping0.c \
        ../../ThirdParty/ogg/source/vorbis/registry.c \
        ../../ThirdParty/ogg/source/vorbis/codebook.c \
        ../../ThirdParty/ogg/source/vorbis/sharedbook.c \
        ../../ThirdParty/ogg/source/vorbis/lookup.c \
        ../../ThirdParty/ogg/source/vorbis/bitrate.c \
        ../../ThirdParty/ogg/source/vorbis/vorbisfile.c \
        ../../ThirdParty/ogg/source/vorbis/vorbisenc.c

include $(BUILD_STATIC_LIBRARY)
