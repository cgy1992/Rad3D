
include $(CLEAR_VARS)

LOCAL_MODULE     := libOpenAL
LOCAL_CFLAGS     := -ffast-math -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../libOpenAL/include \
	$(LOCAL_PATH)/../libOpenAL/OpenAL32/Include
	
LOCAL_SRC_FILES  := \
	../libOpenAL/OpenAL32/alAuxEffectSlot.c \
    ../libOpenAL/OpenAL32/alBuffer.c        \
    ../libOpenAL/OpenAL32/alEffect.c        \
    ../libOpenAL/OpenAL32/alError.c         \
    ../libOpenAL/OpenAL32/alExtension.c     \
    ../libOpenAL/OpenAL32/alFilter.c        \
    ../libOpenAL/OpenAL32/alListener.c      \
    ../libOpenAL/OpenAL32/alSource.c        \
    ../libOpenAL/OpenAL32/alState.c         \
    ../libOpenAL/OpenAL32/alThunk.c         \
    ../libOpenAL/Alc/ALc.c                  \
    ../libOpenAL/Alc/alcConfig.c            \
    ../libOpenAL/Alc/alcDedicated.c         \
    ../libOpenAL/Alc/alcEcho.c              \
    ../libOpenAL/Alc/alcModulator.c         \
    ../libOpenAL/Alc/alcReverb.c            \
    ../libOpenAL/Alc/alcRing.c              \
    ../libOpenAL/Alc/alcThread.c            \
    ../libOpenAL/Alc/ALu.c                  \
    ../libOpenAL/Alc/bs2b.c                 \
    ../libOpenAL/Alc/helpers.c              \
    ../libOpenAL/Alc/hrtf.c                 \
    ../libOpenAL/Alc/mixer.c                \
    ../libOpenAL/Alc/panning.c              \
    ../libOpenAL/Alc/backends/android.c     \
    ../libOpenAL/Alc/backends/loopback.c    \
    ../libOpenAL/Alc/backends/null.c        \
					            

include $(BUILD_STATIC_LIBRARY)
