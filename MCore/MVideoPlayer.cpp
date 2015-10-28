#include "MVideoPlayer.h"
#include "MAudioSystem.h"
#include "MHWBufferManager.h"
#include "MResourceManager.h"
#include "MRoot.h"

namespace Rad {

#define VIDEO_QUEUE_SIZE 8

	const char * VideoPlayerName()
	{
		static int i = 0;
		static char text[32];

		sprintf(text, "_VPlayer_%d", i++);

		return text;
	}

	class VideoPlayerThread : public Thread
	{
	public:
		VideoPlayerThread(VideoPlayer * vp) : Thread(false), i_vp(vp) {}
		virtual ~VideoPlayerThread() {}

		virtual void Run()
		{
			int npixels = i_vp->GetVideo()->i_width * i_vp->GetVideo()->i_height;
			int nframes = VIDEO_QUEUE_SIZE * npixels * 3;

			while (i_vp->GetState() != VideoPlayer::STOPED)
			{
				if (i_vp->_getQueueSize() < nframes)
				{
					MUTEX_ENTER(i_vp->__mutex);

					i_vp->_updateQueue(true);
				}
				else
				{
					Thread::Sleep(1);
				}
			}
		}

	protected:
		VideoPlayer * i_vp;
	};

	VideoPlayer::VideoPlayer(IVideoPtr video, ePixelFormat format, int flags)
	{
		d_assert (format == ePixelFormat::R8G8B8 || format == ePixelFormat::R5G6B5);

		mVideo = video;
		mFlags = flags;
		mState = STOPED;

		mAudioChannel = -1;
		mVideoTexture = HWBufferManager::Instance()->NewTexture(
			VideoPlayerName(), mVideo->i_width, mVideo->i_height, -1, format, eUsage::DYNAMIC_MANAGED);

		mInternalTime = 0;
		mLastTime = 0;
		mThread = NULL;

		mBuffer.Resize(mVideo->i_width * mVideo->i_height * 3);
	}

	VideoPlayer::~VideoPlayer()
	{
		Stop();
	}

	void VideoPlayer::Track()
	{
		if (mState != PLAYING)
			return ;

		float rate = 1.0f / mVideo->i_frame_rate;

		// sync video & audio
		float playTime = mInternalTime;
		if (mAudioChannel != -1)
			playTime = AudioSystem::Instance()->GetSoundPlayTime(mAudioChannel);
		else
			playTime += Root::Instance()->GetFrameTime();

		// need replay
		if (playTime < mInternalTime && (mFlags & VIDEO_FLAG_LOOPED))
		{
			d_printf("replay video");

			MUTEX_ENTER(__mutex);

			mQueue.Clear();
			mLastTime = 0;

			mVideo->Seek(0);
		}

		// update texture
		mInternalTime = playTime;
		if (mInternalTime - mLastTime >= rate)
		{
			int npixels = mVideo->i_width * mVideo->i_height;

			while (mInternalTime - mLastTime >= rate * 2)
			{
				MUTEX_ENTER(__mutex);

				if (mQueue.Size() > 0)
					mQueue.Erase(0, npixels * 3);
				else
					_updateQueue(false);

				mLastTime += rate;
			}

			if (mInternalTime - mLastTime >= rate)
			{
				MUTEX_ENTER(__mutex);

				if (mQueue.Size() == 0)
					_updateQueue(true);

				_updateTexture(&mQueue[0]);

				mQueue.Erase(0, npixels * 3);

				mLastTime += rate;
			}
		}
	}

	void VideoPlayer::_updateQueue(bool fill)
	{
		if (mState != PLAYING)
			return ;

		if (fill)
		{
			int nreads = mVideo->ReadFrame(&mBuffer[0]);
			if (nreads == 0)
			{
				if (mFlags & VIDEO_FLAG_LOOPED)
				{
					mVideo->Seek(0);

					nreads = mVideo->ReadFrame(&mBuffer[0]);
				}
				else
				{
					Stop();
				}
			}

			if (nreads == mBuffer.Size())
			{
				int i = mQueue.Size();

				mQueue.Resize(i + mBuffer.Size());

				memcpy(&mQueue[i], &mBuffer[0], mBuffer.Size());
			}
		}
		else
		{
			int nreads = mVideo->ReadFrame(NULL);
			if (nreads == 0)
			{
				if (mFlags & VIDEO_FLAG_LOOPED)
				{
					mVideo->Seek(0);

					nreads = mVideo->ReadFrame(NULL);
				}
				else
				{
					Stop();
				}
			}
		}
	}

	void VideoPlayer::_updateTexture(byte * pixels)
	{
		int npixels = mVideo->i_width * mVideo->i_height;

		if (mVideoTexture->GetFormat() == ePixelFormat::R8G8B8)
		{
			void * dest = mVideoTexture->Lock(eLockFlag::WRITE);
			{
				memcpy(dest, pixels, 3 * npixels);
			}
			mVideoTexture->Unlock();
		}
		else
		{
			word * dest = (word *)mVideoTexture->Lock(eLockFlag::WRITE);
			{
				for (int i = 0; i < npixels; ++i)
				{
					int r = pixels[i * 3 + 0];
					int g = pixels[i * 3 + 1];
					int b = pixels[i * 3 + 2];

					r /= 8, g /= 4, b /= 8;

					dest[i] = (word)(r << 11 | g << 5 | b);
				}
			}
			mVideoTexture->Unlock();
		}
	}

	void VideoPlayer::Start()
	{
		d_assert (mState == STOPED);

		if (mVideo->i_audio != NULL && (mFlags & VIDEO_FLAG_AUDIO))
		{
			int flags = 0;

			if (mFlags & VIDEO_FLAG_LOOPED)
				flags |= AUDIO_FLAG_LOOPED;

			if (mFlags & VIDEO_FLAG_MULTI_THREAD)
				flags |= VIDEO_FLAG_MULTI_THREAD;

			mAudioChannel = AudioSystem::Instance()->PlaySound(mVideo->i_audio, 0, flags);
		}

		mState = PLAYING;
		
		if (mFlags & VIDEO_FLAG_MULTI_THREAD)
		{
			d_assert (mThread == NULL);

			mThread = new VideoPlayerThread(this);
			mThread->Start();
		}

		mLastTime = 0;
		mInternalTime = 0;
	}

	void VideoPlayer::Pasue()
	{
		d_assert (mState == PLAYING);

		mState = PAUSED;

		if (mAudioChannel != -1)
			AudioSystem::Instance()->PauseSound(mAudioChannel);
	}

	void VideoPlayer::Resume()
	{
		d_assert (mState == PAUSED);

		mState = PLAYING;

		if (mAudioChannel != -1)
			AudioSystem::Instance()->ResumeSound(mAudioChannel);
	}

	void VideoPlayer::Stop()
	{
		if (mAudioChannel != -1)
			AudioSystem::Instance()->StopSound(mAudioChannel);

		mAudioChannel = -1;

		mState = STOPED;

		safe_delete (mThread);
		mQueue.Clear();
	}


	//
	VideoPlayer * VDO_CreatePlayer(const String & filename, ePixelFormat format, int flags)
	{
		if (filename == "")
			return NULL;

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL)
		{
			d_log("!: file open failed, '%s'", filename.c_str());
			return NULL;
		}

		return VDO_CreatePlayer(stream, format, flags);
	}

	VideoPlayer * VDO_CreatePlayer(DataStreamPtr stream, ePixelFormat format, int flags)
	{
		IVideoPtr vdo = VDO_Load(stream);

		if (vdo == NULL)
		{
			d_log("!: load video failed");
			return NULL;
		}

		return VDO_CreatePlayer(vdo, format, flags);
	}

	VideoPlayer * VDO_CreatePlayer(IVideoPtr vdo, ePixelFormat format, int flags)
	{
		return new VideoPlayer(vdo, format, flags);
	}
}