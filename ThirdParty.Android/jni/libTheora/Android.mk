
include $(CLEAR_VARS)

LOCAL_MODULE := libTheora

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ThirdParty/ogg/include \
	$(LOCAL_PATH)/../../ThirdParty/ogg/source/theora

LOCAL_SRC_FILES := \
    ../../ThirdParty/ogg/source/theora/analyze.c \
	../../ThirdParty/ogg/source/theora/apiwrapper.c \
	../../ThirdParty/ogg/source/theora/bitpack.c \
	../../ThirdParty/ogg/source/theora/decapiwrapper.c \
	../../ThirdParty/ogg/source/theora/decinfo.c \
	../../ThirdParty/ogg/source/theora/decode.c \
	../../ThirdParty/ogg/source/theora/dequant.c \
	../../ThirdParty/ogg/source/theora/encapiwrapper.c \
	../../ThirdParty/ogg/source/theora/encfrag.c \
	../../ThirdParty/ogg/source/theora/encinfo.c \
	../../ThirdParty/ogg/source/theora/encode.c \
	../../ThirdParty/ogg/source/theora/encoder_disabled.c \
	../../ThirdParty/ogg/source/theora/enquant.c \
	../../ThirdParty/ogg/source/theora/fdct.c \
	../../ThirdParty/ogg/source/theora/fragment.c \
	../../ThirdParty/ogg/source/theora/huffdec.c \
	../../ThirdParty/ogg/source/theora/huffenc.c \
	../../ThirdParty/ogg/source/theora/idct.c \
	../../ThirdParty/ogg/source/theora/info.c \
	../../ThirdParty/ogg/source/theora/internal.c \
	../../ThirdParty/ogg/source/theora/mathops.c \
	../../ThirdParty/ogg/source/theora/mcenc.c \
	../../ThirdParty/ogg/source/theora/quant.c \
	../../ThirdParty/ogg/source/theora/rate.c \
	../../ThirdParty/ogg/source/theora/state.c \
	../../ThirdParty/ogg/source/theora/tokenize.c \
	../../ThirdParty/ogg/source/theora/x86_vc/mmxencfrag.c \
	../../ThirdParty/ogg/source/theora/x86_vc/mmxfdct.c \
	../../ThirdParty/ogg/source/theora/x86_vc/mmxfrag.c \
	../../ThirdParty/ogg/source/theora/x86_vc/mmxidct.c \
	../../ThirdParty/ogg/source/theora/x86_vc/mmxstate.c \
	../../ThirdParty/ogg/source/theora/x86_vc/x86cpu.c \
	../../ThirdParty/ogg/source/theora/x86_vc/x86enc.c \
	../../ThirdParty/ogg/source/theora/x86_vc/x86state.c

include $(BUILD_STATIC_LIBRARY)
