#include "GLInclude.h"
#include "GLRenderSystem.h"
#include "GLTexture.h"
#include "GLRenderTarget.h"
#include "GLRenderTarget.h"
#include "GLHWBufferManager.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLHWBufferManager.h"
#include "GLShaderFXManager.h"
#include "GLUtil.h"

namespace Rad {

	GLuint mFrameBufferRead = 0;
	GLuint mFrameBufferDraw = 0;

#ifdef M_PLATFORM_WIN32
	GLRenderSystem::GLRenderSystem(HWND hWnd, const Config & config)
#else
	GLRenderSystem::GLRenderSystem(const Config & config)
#endif
	{
		mConfig = config;

#ifdef M_PLATFORM_WIN32
		WGL_CreateContext(hWnd, mConfig);
#endif
		mHWBufferManager = new GLHWBufferManager;
		mShaderFXManager = new GLShaderFXManager;

		const char * vendor = (const char *)glGetString(GL_VENDOR);
		const char * render = (const char *)glGetString(GL_RENDERER);
		const char * version = (const char *)glGetString(GL_VERSION);
		const char * extensions = (const char *)glGetString(GL_EXTENSIONS);

		mCaps.vender = vendor;
		mCaps.renderer = render;
		mCaps.vender = version;

		mCaps.pixelFormats[ePixelFormat::DXT1_RGB] = false;
		mCaps.pixelFormats[ePixelFormat::DXT3_RGBA] = false;
		mCaps.pixelFormats[ePixelFormat::DXT5_RGBA] = false;
		mCaps.pixelFormats[ePixelFormat::ETC1_RGB] = false;
		//mCaps.pixelFormats[ePixelFormat::ETC2_RGBA] = false;
		mCaps.pixelFormats[ePixelFormat::PVRTC1_RGB] = false;
		//mCaps.pixelFormats[ePixelFormat::PVRTC1_RGBA] = false;

		if (strstr(extensions, "GL_EXT_texture_compression_dxt1") != NULL ||
			strstr(extensions, "GL_EXT_texture_compression_s3tc") != NULL)
		{
			d_printf("-: PixelFormat DXT1_RGBA DXT3_RGBA DXT5_RGBA support!");

			//mCaps.pixelFormats[ePixelFormat::DXT1_RGB] = true;
			//mCaps.pixelFormats[ePixelFormat::DXT3_RGBA] = true;
			//mCaps.pixelFormats[ePixelFormat::DXT5_RGBA] = true;
		}

		if (strstr(extensions, "GL_OES_compressed_ETC1_RGB8_texture") != NULL)
		{
			d_printf("-: PixelFormat ETC1_RGB support!");

			mCaps.pixelFormats[ePixelFormat::ETC1_RGB] = true;
		}

		if (strstr(extensions, "GL_IMG_texture_compression_pvrtc") != NULL)
		{
			d_printf("-: PixelFormat PVRTC1_RGB PVRTC1_RGBA support!");

			mCaps.pixelFormats[ePixelFormat::PVRTC1_RGB] = true;
		}

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
		mCaps.pixelFormats[ePixelFormat::DXT1_RGB] = false;
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
		mCaps.pixelFormats[ePixelFormat::DXT3_RGBA] = false;
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
		mCaps.pixelFormats[ePixelFormat::DXT5_RGBA] = false;
#endif

#ifndef GL_ETC1_RGB8_OES
		mCaps.pixelFormats[ePixelFormat::ETC1_RGB] = false;
#endif

#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
		mCaps.pixelFormats[ePixelFormat::ETC1_RGB] = false;
#endif

#ifndef M_PLATFORM_WIN32
		mCaps.pixelFormats[ePixelFormat::R16F] = false;
		mCaps.pixelFormats[ePixelFormat::R16G16F] = false;
		mCaps.pixelFormats[ePixelFormat::R16G16B16A16F] = false;
		mCaps.pixelFormats[ePixelFormat::R32F] = false;
		mCaps.pixelFormats[ePixelFormat::R32G32F] = false;
		mCaps.pixelFormats[ePixelFormat::R32G32B32A32F] = false;
#endif

		//
		mScreenQuad = new RenderOp;

		mScreenQuad->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT4);
		mScreenQuad->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mScreenQuad->vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 6, 4);

		mScreenQuad->primCount = 2;
		mScreenQuad->primType = ePrimType::TRIANGLE_STRIP;

		mScreenQuadRT = new RenderOp;

		mScreenQuadRT->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT4);
		mScreenQuadRT->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		mScreenQuadRT->vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 6, 4);

		mScreenQuadRT->primCount = 2;
		mScreenQuadRT->primType = ePrimType::TRIANGLE_STRIP;

		_updateScreenQuad(config.width, config.height);

		//
		OnInit();
	}

	GLRenderSystem::~GLRenderSystem()
	{
		OnShutdown();

		if (mFrameBufferDraw != 0)
		{
			glDeleteFramebuffers(1, &mFrameBufferRead);
			glDeleteFramebuffers(1, &mFrameBufferDraw);
			mFrameBufferDraw = 0;
			mFrameBufferRead = 0;
		}

		delete mScreenQuad;
		delete mScreenQuadRT;

		delete mShaderFXManager;
		delete mHWBufferManager;

#ifdef M_PLATFORM_WIN32
		WGL_DestroyContext();
#endif
	}

	void GLRenderSystem::OnLostDevice()
	{
		if (mFrameBufferDraw != 0)
		{
			glDeleteFramebuffers(1, &mFrameBufferRead);
			glDeleteFramebuffers(1, &mFrameBufferDraw);
			mFrameBufferDraw = 0;
			mFrameBufferRead = 0;
		}

		mShaderFXManager->OnLostDevice();
		d_assert(glGetError() == 0);

		mHWBufferManager->OnLostDevice();
		d_assert(glGetError() == 0);

		E_LostDevice();
		d_assert(glGetError() == 0);
	}

	void GLRenderSystem::OnResetDevice()
	{
		d_assert(glGetError() == 0);

		mShaderFXManager->OnResetDevice();
		d_assert(glGetError() == 0);

		mHWBufferManager->OnResetDevice();
		d_assert(glGetError() == 0);

		E_ResetDevice();
		d_assert(glGetError() == 0);
	}

	void GLRenderSystem::OnResize(int w, int h)
	{
		if (mConfig.width != w || mConfig.height != h)
		{
			mConfig.width = w;
			mConfig.height = h;

			_updateScreenQuad(w, h);

			E_Resize(w, h);
		}
	}

	void GLRenderSystem::Begin()
	{
		//
		mBatchCount = 0;
		mPrimitiveCount = 0;

		//
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		d_assert (glGetError() == 0);
	}

	void GLRenderSystem::End()
	{
	#ifdef M_PLATFORM_WIN32
		WGL_SwapBuffers();

		d_assert (glGetError() == 0);
	#endif

		_clearState();
	}

	void GLRenderSystem::Finish()
	{
		glFinish();
	}

	void GLRenderSystem::SetViewport(const Viewport & viewport)
	{
		Viewport vp = viewport;

		int rtWidth = mConfig.width;
		int rtHeight = mConfig.height;

		if (mCurrentRenderTarget[0] != NULL)
		{
			rtWidth = mCurrentRenderTarget[0]->GetWidth();
			rtHeight = mCurrentRenderTarget[0]->GetHeight();
		}

		if (vp.w == 0 || vp.h == 0)
		{
			vp.x = 0;
			vp.y = 0;
			vp.w = rtWidth;
			vp.h = rtHeight;
		}

		d_assert (
			vp.x >= 0 && vp.x + vp.w <= rtWidth &&
			vp.y >= 0 && vp.y + vp.h <= rtHeight);

		GLint x = vp.x;
		GLint y = rtHeight - (vp.y + vp.h);
		GLint w = vp.w;
		GLint h = vp.h;

		glViewport(x, y, w, h);

		d_assert (glGetError() == 0);
	}

	void GLRenderSystem::Clear(eClearMode mode, const Float4 & color, float depth, int stencil)
	{
		if (mode == eClearMode::NONE)
			return ;

		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(depth);
		glClearStencil(stencil);

		d_assert (glGetError() == 0);

		GLbitfield clearFlag = 0;

		if (mode & eClearMode::COLOR)
			clearFlag |= GL_COLOR_BUFFER_BIT;
	
		if (mode & eClearMode::DEPTH)
			clearFlag |= GL_DEPTH_BUFFER_BIT;
	
		if (mode & eClearMode::STENCIL)
			clearFlag |= GL_STENCIL_BUFFER_BIT;

		glDepthMask(GL_TRUE);
		glStencilMask(GL_TRUE);

		mRenderStateChanged = true;

		glClear(clearFlag);

		d_assert (glGetError() == 0);
	}

	void GLRenderSystem::SetProjTM(const Mat4 & projTM)
	{
		Mat4 adjTM;
		if (mCurrentRenderTarget[0] == NULL)
		{
			adjTM = Mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 2, 0,
				0, 0,-1, 1);
		}
		else
		{
			adjTM = Mat4(
				1, 0, 0, 0,
				0,-1, 0, 0,
				0, 0, 2, 0,
				0, 0,-1, 1);
		}

		RenderRegister::Instance()->SetProjTM(projTM * adjTM);
	}

	void GLRenderSystem::PrepareRendering()
	{
		if (!mRenderTargetChanged)
			return ;

		if (mCurrentRenderTarget[0] != NULL)
		{
			FixedArray<GLenum, MAX_HW_RENDERTARGET> targets;

			glBindFramebuffer(GL_FRAMEBUFFER, mHWBufferManager->GetFrameBuffer());

			for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
			{
				GLRenderTarget * pGLRenderTarget = (GLRenderTarget *)mCurrentRenderTarget[i];
				if (pGLRenderTarget != NULL)
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pGLRenderTarget->GetGLTexture(), 0);

					targets.PushBack(GL_COLOR_ATTACHMENT0 + i);
				}
				else
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
				}

				d_assert (glGetError() == 0);
			}

			d_assert (glGetError() == 0);

			GLDepthBuffer * pGLDepthBuffer = (GLDepthBuffer *)mCurrentDepthBuffer;
			if (pGLDepthBuffer)
			{
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pGLDepthBuffer->GetGLDepthBuffer());

				if (pGLDepthBuffer->HasStencilBuffer())
				{
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pGLDepthBuffer->GetGLDepthBuffer());
				}
			}
			else
			{
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
			}

#ifdef M_DEBUG
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				d_assert (0 && "depth buffer format check failed.");
			}
#endif

			glDrawBuffers(targets.Size(), &targets[0]);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		d_assert (glGetError() == 0);

		mRenderTargetChanged = false;
	}

	void GLRenderSystem::ReadPixels(void * pixels, int x, int y, int w, int h)
	{
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	void GLRenderSystem::StretchRect(RenderTarget * rtDest, RectI * rcDest, RenderTarget * rtSrc, RectI * rcSrc, eTexFilter filter)
	{
#ifdef M_PLATFORM_WIN32
		if (mFrameBufferDraw == 0)
		{
			glGenFramebuffers(1, &mFrameBufferRead);
			glGenFramebuffers(1, &mFrameBufferDraw);
		}

		RectI drc, src;
		if (rcDest == NULL)
		{
			drc.x1 = 0;
			drc.y1 = 0;
			drc.x2 = rtDest->GetWidth();
			drc.y2 = rtDest->GetHeight();
		}
		else
		{
			drc = *rcDest;
		}

		if (rcSrc == NULL)
		{
			src.x1 = 0;
			src.y1 = 0;
			src.x2 = rtSrc->GetWidth();
			src.y2 = rtSrc->GetHeight();
		}
		else
		{
			src = *rcSrc;
		}

		GLRenderTarget * pGLDest = (GLRenderTarget *)rtDest;
		GLRenderTarget * pGLSrc = (GLRenderTarget *)rtSrc;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBufferRead);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pGLSrc->GetGLTexture(), 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBufferDraw);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pGLDest->GetGLTexture(), 0);

		glBlitFramebuffer(
			src.x1, src.y1, src.x2, src.y2, 
			drc.x1, drc.y1, drc.x2, drc.y2, 
			GL_COLOR_BUFFER_BIT,
			filter == eTexFilter::POINT ? GL_NEAREST : GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		d_assert (glGetError() == 0);
#else
		d_assert (0 && "not support!");
#endif
	}

	void GLRenderSystem::_bindRenderState()
	{
		if (mRenderStateChanged)
		{
			// OpenGL ES only GL_FILL, but it's useful for debug.
#ifdef M_PLATFORM_WIN32
			switch (mCurrentRenderState.FillMode)
			{
			case eFillMode::POINT:
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;

			case eFillMode::FRAME:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

			case eFillMode::SOLID:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			}
#endif
			switch (mCurrentRenderState.CullMode)
			{
			case eCullMode::NONE:
				glDisable(GL_CULL_FACE);
				break;

			case eCullMode::FRONT:
				glEnable(GL_CULL_FACE);
				glCullFace(mCurrentRenderTarget[0] == NULL ? GL_BACK : GL_FRONT);
				break;

			case eCullMode::BACK:
				glEnable(GL_CULL_FACE);
				glCullFace(mCurrentRenderTarget[0] == NULL ? GL_FRONT : GL_BACK);
				break;
			}

			switch (mCurrentRenderState.DepthMode)
			{
			case eDepthMode::NONE:
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				break;

			case eDepthMode::ALWAYS:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_ALWAYS);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_EQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::LESS:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::LESS_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::GREATER:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GREATER);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::GREATER_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GEQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::N_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_EQUAL);
				glDepthMask(GL_FALSE);
				break;

			case eDepthMode::N_LESS:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glDepthMask(GL_FALSE);
				break;

			case eDepthMode::N_LESS_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDepthMask(GL_FALSE);
				break;

			case eDepthMode::N_GREATER:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GREATER);
				glDepthMask(GL_FALSE);
				break;

			case eDepthMode::N_GREATER_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GEQUAL);
				glDepthMask(GL_FALSE);
				break;
			}

			switch (mCurrentRenderState.BlendMode)
			{
			case eBlendMode::OPACITY:
			case eBlendMode::ALPHA_TEST:
				glDisable(GL_BLEND);
				break;

			case eBlendMode::ALPHA_BLEND:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;

			case eBlendMode::ADD:
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				break;

			case eBlendMode::MUL:
				glEnable(GL_BLEND);
				glBlendFunc(GL_ZERO, GL_SRC_COLOR);
				break;

			case eBlendMode::COLOR_BLEND:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
				break;
			}

			if (mColorWriteEnable)
			{
				glColorMask(
					mCurrentRenderState.ColorMode & eColorMode::RED, 
					mCurrentRenderState.ColorMode & eColorMode::GREEN,
					mCurrentRenderState.ColorMode & eColorMode::BLUE,
					mCurrentRenderState.ColorMode & eColorMode::ALPHA);
			}
			else
			{
				glColorMask(false, false, false, false);
			}

			d_assert (glGetError() == 0);

			mRenderStateChanged = false;
		}
	}

	void GLRenderSystem::_bindShaderPass()
	{
		d_assert (mCurrentShaderPass != NULL);

		GLShaderPass * pGLShaderPass = (GLShaderPass *)mCurrentShaderPass;

		if (mShaderPassChanged)
		{
			glUseProgram(pGLShaderPass->GetGLShaderProgram());

			mShaderPassChanged = false;
		}

		pGLShaderPass->UploadUniform();

		pGLShaderPass->BindSamplerState();
	}

	void GLRenderSystem::_bindSamplers()
	{
		for (int i = 0; i < MAX_HW_SAMPLER; ++i)
		{
			if (mTextureChanged[i])
			{
				glActiveTexture(GL_TEXTURE0 + i);

				if (mCurrentTextures[i] != NULL)
				{
					if (TYPE_OF(GLTexture, mCurrentTextures[i]))
					{
						GLTexture * p = (GLTexture *)mCurrentTextures[i];

						glBindTexture(GL_TEXTURE_2D, p->GetGLTexture());
					}
					else
					{
						d_assert (TYPE_OF(GLRenderTexture, mCurrentTextures[i]));

						GLRenderTexture * p = (GLRenderTexture *)mCurrentTextures[i];

						glBindTexture(GL_TEXTURE_2D, p->GetGLTexture());
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				d_assert (glGetError() == 0);

				mTextureChanged[i] = false;
			}
		}
	}

	#define VBO_BUFFER_OFFSET(buff, i) ((const char *)buff + (i))

	void _bindVertexElement(VertexDeclaration & vdecl, GLShaderPass * pass, const void * buffer, int startVertex)
	{
		int stride = vdecl.GetVertexSize();
		int offset = startVertex * stride;

		for (int i = 0; i < vdecl.GetElementCount(); ++i)
		{
			const VertexElement & elem = vdecl.GetElement(i);

			GLenum type = GLUtil::GetGLVertType(elem.Type);
			GLsizei size = GLUtil::GetGLVertTypeCount(elem.Type);
			GLint handle = pass->GetAttribute(elem.Semantic);

			if (handle != -1)
			{
				glEnableVertexAttribArray(handle);
				glVertexAttribPointer(handle, size, type, GL_FALSE, stride, VBO_BUFFER_OFFSET(buffer, offset));

				d_assert(glGetError() == 0);
			}

			offset += VertexElement::GetTypeSize(elem.Type);
		}
	}

	void _unbindVertexElement()
	{
		for (int i = 0; i < eVertexSemantic::MAX; ++i)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLRenderSystem::Render(RenderOp * rop)
	{
		if (rop->primCount == 0)
			return ;

		d_assert (mCurrentShaderPass != NULL && rop->vertexBuffers[0] != NULL);

		mRenderRegister->Begin();

		_bindRenderState();
		_bindSamplers();
		_bindShaderPass();

		GLenum GLprimType = GLUtil::GetGLPrimType(rop->primType);
		int drawCount = GLUtil::GetGLDrawCount(rop->primType, rop->primCount);
		int vertCount = rop->vertexBuffers[0]->GetCount();

		for (int i = 0; i < 1; ++i)
		{
			GLVertexBuffer * pGLVertexBuffer = (GLVertexBuffer *)rop->vertexBuffers[i].c_ptr();

			if (pGLVertexBuffer != NULL)
			{
				d_assert(pGLVertexBuffer->GetCount() == vertCount);

				if (mCurrentVertexBuffer != rop->vertexBuffers[i].c_ptr())
				{
					glBindBuffer(GL_ARRAY_BUFFER, pGLVertexBuffer->GetGLBuffer());

					mCurrentVertexBuffer = rop->vertexBuffers[i].c_ptr();
				}

				d_assert (glGetError() == 0);

				_bindVertexElement(rop->vertexDeclarations[i], (GLShaderPass *)mCurrentShaderPass, NULL, 0);
			}
		}
		
		for (int i = 1; i < MAX_HW_VERTEX_STREAM; ++i)
		{
			GLVertexBuffer * pGLVertexBuffer = (GLVertexBuffer *)rop->vertexBuffers[i].c_ptr();

			if (pGLVertexBuffer != NULL)
			{
				d_assert(pGLVertexBuffer->GetCount() == vertCount);

				glBindBuffer(GL_ARRAY_BUFFER, pGLVertexBuffer->GetGLBuffer());

				d_assert (glGetError() == 0);

				_bindVertexElement(rop->vertexDeclarations[i], (GLShaderPass *)mCurrentShaderPass, NULL, 0);

				mCurrentVertexBuffer = NULL;
			}
		}

		if (rop->indexBuffer != NULL)
		{
			GLIndexBuffer * pGLIndexBuffer = (GLIndexBuffer *)rop->indexBuffer.c_ptr();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pGLIndexBuffer->GetGLBuffer());
			d_assert (glGetError() == 0);

			glDrawElements(GLprimType, drawCount, pGLIndexBuffer->GetStride() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, NULL);
		}
		else
		{
			glDrawArrays(GLprimType, 0, drawCount);
		}

		_unbindVertexElement();

		mRenderRegister->End();

		mBatchCount += 1;
		mPrimitiveCount += rop->primCount;
	}

	void GLRenderSystem::RenderEx(VertexDeclaration * vdecl, const void * vertexData, const void * indexData, ePrimType primType, int primCount)
	{
		d_assert (vdecl != NULL && vertexData != NULL);

		if (primCount == 0)
			return ;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mRenderRegister->Begin();

		_bindRenderState();
		_bindSamplers();
		_bindShaderPass();

		GLenum GLprimType = GLUtil::GetGLPrimType(primType);
		int drawCount = GLUtil::GetGLDrawCount(primType, primCount);

		_bindVertexElement(*vdecl, (GLShaderPass *)mCurrentShaderPass, vertexData, 0);

		if (indexData != NULL)
		{
			glDrawElements(GLprimType, drawCount, GL_UNSIGNED_SHORT, indexData);
		}
		else
		{
			glDrawArrays(GLprimType, 0, drawCount);
		}

		_unbindVertexElement();

		mRenderRegister->End();

		mBatchCount += 1;
		mPrimitiveCount += primCount;
	}

	void GLRenderSystem::_updateScreenQuad(int w, int h)
	{
		float halfInvWidth = 0.5f / w;
		float halfInvHeight = 0.5f / h;

		float * vert = (float *)mScreenQuad->vertexBuffers[0]->Lock(eLockFlag::WRITE);
		{
			float x = 0, y = 0, z = 0;

			*vert++ = -1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 0; *vert++ = 0;

			*vert++ = 1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 1; *vert++ = 0;

			*vert++ = -1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 0; *vert++ = 1;

			*vert++ = 1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 1; *vert++ = 1;
		}
		mScreenQuad->vertexBuffers[0]->Unlock();

		vert = (float *)mScreenQuadRT->vertexBuffers[0]->Lock(eLockFlag::WRITE);
		{
			float x = 0, y = 0, z = 0;

			*vert++ = -1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 0; *vert++ = 0;

			*vert++ = 1; *vert++ = -1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 0 + halfInvHeight;
			*vert++ = 1; *vert++ = 0;

			*vert++ = -1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 0 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 0; *vert++ = 1;

			*vert++ = 1; *vert++ = 1; *vert++ = 0; *vert++ = 1;
			//*vert++ = 1 + halfInvWidth; *vert++ = 1 + halfInvHeight;
			*vert++ = 1; *vert++ = 1;
		}
		mScreenQuadRT->vertexBuffers[0]->Unlock();
	}

	void GLRenderSystem::RenderScreenQuad(ShaderFX * fx)
	{
		SetWorldTM(Mat4::Identity);

		for (int i = 0; i < fx->GetPassCount(); ++i)
		{
			SetShaderPass(fx->GetPass(i), true);
			Render(mCurrentRenderTarget[0] != NULL ? mScreenQuadRT : mScreenQuad);
		}
	}

}