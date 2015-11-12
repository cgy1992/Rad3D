#include "GLUtil.h"
#include "MDebug.h"

namespace Rad {

#ifdef M_PLATFORM_WIN32

	//EglStruct gEGL;

	//int EGL_CreateContext(HWND hWnd)
	//{
	//	EGLint numConfigs;

	//	EGLint attribList[] = {
	//		EGL_RED_SIZE, 8,
	//		EGL_GREEN_SIZE, 8,
	//		EGL_BLUE_SIZE, 8,
	//		EGL_DEPTH_SIZE, 16,
	//		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	//		EGL_NONE
	//	};

	//	// Get Display
	//	gEGL.display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	//	if ( gEGL.display == EGL_NO_DISPLAY )
	//	{
	//		assert( 0 && "eglGetDisplay failed" );      
	//		return FALSE;
	//	}

	//	// Initialize EGL
	//	if ( ! eglInitialize( gEGL.display, NULL, NULL) )
	//	{
	//		assert( 0 && "eglInitialize failed" );
	//		return FALSE;
	//	}

	//	// Get configs
	//	/*if ( ! eglGetConfigs( gEGL.display, NULL, 0, &numConfigs ) )
	//	{
	//		assert( 0 && "eglGetConfigs failed" );      
	//		return FALSE;
	//	}*/

	//	// Choose config
	//	if ( !eglChooseConfig(gEGL.display, attribList, &gEGL.config, 1, &numConfigs) )
	//	{
	//		assert( 0 && "eglChooseConfig failed" );      
	//		return FALSE;
	//	}

	//	// Create a surface
	//	gEGL.surface = eglCreateWindowSurface( gEGL.display, gEGL.config, NativeWindowType( hWnd ), NULL );
	//	if ( gEGL.surface == EGL_NO_SURFACE )
	//	{
	//		assert( 0 && "eglCreateWindowSurface failed" );      
	//		return FALSE;
	//	}

	//	// Create a GL context
	//	EGLint ctxAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
	//	gEGL.context = eglCreateContext( gEGL.display, gEGL.config, EGL_NO_CONTEXT, ctxAttribList );
	//	if ( gEGL.context == EGL_NO_CONTEXT )
	//	{
	//		assert( 0 && "eglCreateContext failed" );      
	//		return FALSE;
	//	}   

	//	// Make the context current
	//	if ( !eglMakeCurrent( gEGL.display, gEGL.surface, gEGL.surface, gEGL.context ) )
	//	{
	//		assert( 0 && "eglMakeCurrent failed" );      
	//		return FALSE;
	//	}

	//	return TRUE;
	//}

	//void EGL_DestroyContext()
	//{
	//	eglDestroyContext( gEGL.display, gEGL.context );
	//	eglDestroySurface( gEGL.display, gEGL.surface );
	//	eglTerminate( gEGL.display );
	//}

	//void EGL_SwapBuffer()
	//{
	//	eglSwapBuffers(gEGL.display, gEGL.surface);
	//}

	struct WGL_Structer
	{
		HWND hwnd;
		HDC hdc;
		HGLRC hrc;

		WGL_Structer()
		{
			hwnd = NULL;
			hdc = NULL;
			hrc = NULL;
		}
	};

	static WGL_Structer gWGL;

	LRESULT WINAPI WGL_DummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, umsg, wp, lp);
	}

	void WGL_Init()
	{
		LPCSTR dummyText = "_WglDummy_";
		HINSTANCE hinst = GetModuleHandle( NULL );

		WNDCLASS dummyClass;
		memset(&dummyClass, 0, sizeof(WNDCLASS));
		dummyClass.style = CS_OWNDC;
		dummyClass.hInstance = hinst;
		dummyClass.lpfnWndProc = WGL_DummyWndProc;
		dummyClass.lpszClassName = dummyText;
		RegisterClass(&dummyClass);

		HWND hwnd = CreateWindow(dummyText, dummyText,
			WS_POPUP | WS_CLIPCHILDREN,
			0, 0, 32, 32, 0, 0, hinst, 0);

		HDC hdc = GetDC(hwnd); 

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_SWAP_EXCHANGE;
		pfd.iPixelType = PFD_TYPE_RGBA;

		int format;
		if ((format = ChoosePixelFormat(hdc, &pfd)) != 0)
			SetPixelFormat(hdc, format, &pfd);

		HGLRC hrc = wglCreateContext(hdc);

		wglMakeCurrent(hdc, hrc);

		GLenum err = glewInit();

		d_assert (err == GLEW_OK);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		UnregisterClass(dummyText, hinst);

		wglSwapIntervalEXT(0);
	}

	void WGL_SetPixelFormat(HDC hDC, const RenderSystem::Config & config)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = config.colorBits;
		pfd.cAlphaBits = config.alphaBits; 
		pfd.cDepthBits = config.depthBits;
		pfd.cStencilBits = config.stencilBits;

		int PixelFormat = 0;
		{
			int iAttributes[] = {
				WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
				WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB,			config.colorBits,
				WGL_ALPHA_BITS_ARB,			config.alphaBits,
				WGL_DEPTH_BITS_ARB,			config.depthBits,
				WGL_STENCIL_BITS_ARB,		config.stencilBits,
				WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB,		config.msaa ? GL_TRUE : GL_FALSE,
				WGL_SAMPLES_ARB,			4,
				0,							0
			};

			float fAttributes[] = {
				0, 0
			};

			UINT numFormats = 0;

			if (WGLEW_GET_FUN(__wglewChoosePixelFormatARB) != NULL)
				wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &PixelFormat, &numFormats);

			if (numFormats == 0)
				PixelFormat = ChoosePixelFormat(hDC, &pfd);
		}

		SetPixelFormat(hDC, PixelFormat, &pfd);
	}

	void WGL_CreateContext(HWND hWnd, const RenderSystem::Config & config)
	{
		gWGL.hwnd = hWnd;

		gWGL.hdc = GetDC(hWnd);

		WGL_Init();

		WGL_SetPixelFormat(gWGL.hdc, config);

		gWGL.hrc = wglCreateContext(gWGL.hdc);

		d_assert (gWGL.hrc != NULL);

		wglMakeCurrent(gWGL.hdc, gWGL.hrc);
	}

	void WGL_DestroyContext()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(gWGL.hrc);
		ReleaseDC(gWGL.hwnd, gWGL.hdc);

		gWGL.hwnd = NULL;
		gWGL.hdc = NULL;
		gWGL.hrc = NULL;
	}

	void WGL_SwapBuffers()
	{
		SwapBuffers(gWGL.hdc);
	}

#endif

}