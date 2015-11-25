/*
*	Video Player
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MVideo.h"
#include "MTexture.h"
#include "MTimer.h"
#include "MThread.h"

namespace Rad {

	class M_ENTRY VideoPlayer
	{
	public:
		enum {
			STOPED,
			PLAYING,
			PAUSED,
		};

		Mutex __mutex;

	public:
		VideoPlayer(IVideoPtr video, ePixelFormat format, int flags);
		~VideoPlayer();

		void
			Start();
		void
			Pasue();
		void
			Resume();
		void
			Stop();
		int
			GetState() { return mState; }

		IVideoPtr
			GetVideo() { return mVideo; }
		int
			GetAudioChannel() { return mAudioChannel; }
		TexturePtr
			GetVideoTexture() { return mVideoTexture; }

		void
			Track();
		void
			_updateQueue(bool fill);
		int
			_getQueueSize() { return mQueue.Size(); }

	protected:
		void
			_updateTexture(byte * pixels);

	protected:
		IVideoPtr mVideo;
		int mFlags;
		int mState;

		int mAudioChannel;
		TexturePtr mVideoTexture;

		float mInternalTime;
		float mLastTime;

		Array<byte> mBuffer;
		Array<byte> mQueue;
		Thread * mThread;
	};

#define VIDEO_FLAG_LOOPED 0x01
#define VIDEO_FLAG_AUDIO 0x02
#define VIDEO_FLAG_MULTI_THREAD 0x20


	// make sure filename open with FileStream, if it too large.
	//		it has 2 stream(audio & video)
	//
	// @format must be R8G8B8 or R5G6B5
	//
	M_ENTRY VideoPlayer *
		VDO_CreatePlayer(const String & filename, ePixelFormat format = ePixelFormat::R8G8B8, int flags = 0);
	M_ENTRY VideoPlayer *
		VDO_CreatePlayer(DataStreamPtr stream, ePixelFormat format = ePixelFormat::R8G8B8, int flags = 0);
	M_ENTRY VideoPlayer *
		VDO_CreatePlayer(IVideoPtr vdo, ePixelFormat format = ePixelFormat::R8G8B8, int flags = 0);
	M_ENTRY void
		VDO_DestroyPlayer(VideoPlayer * player);
}