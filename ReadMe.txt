Author: Myway(SiZhong.Wang)
E-MAIL: MSoft1115@hotmal.com

~I'm sorry, i can write little engish.

	Rad3D has more bugs, but it has good design.

1. Engine Structer
	1.1 AudioSystem
		-- ALAudioSystem: use for windows, ios
		-- SLAudioSystem: use for android

	1.2 RenderSystem
		-- GLRenderSystem: opengl & opengles 2.0 integration
		-- NullRenderSystem: use for some tools

	1.3 DI8InputSystem -- DirectInput integration

	1.4 MCore -- Engine lib
	
	1.5 MGUI - GUI System
	
	1.6 MParticleFX - ParticleSystem plugin
	
	1.7 MPhysics - ode physics integration

2. Tools
	2.1 Export
		- MaxExport2010: 3ds max 2010 exporter
	
	2.2 MeshViewer - Mesh Viewer

	2.3 MPKTool - Generate MPK(compressed and encrypt archive)
	
	2.4 MStudio - SceneEditor, ParticleEditor, MaterialEditor...
	
	2.5 ResourceViewer - You can drag file to MStudio

	2.6 UIEditor - Create GUI Lookfeel & Layout

3. Depends
	OpenAL
	OpenSL
	OpenGL
	OpenGL ES
	FastLZ
	FreeType
	Iconv
	ODE
	rapidxml
	lodpng
	jpeg decoder
	etc
	ogg vorbis
	ogg eheora
	lua

4. How to Complie Rad3D

	4.1 Windows (SDK auto copy to SDK.win32)
		a. Open Rad3D.sln with vs2010/vs2010+
		b. Build engine floder
		c. Build other floder
	
	4.2 Android
		a. Build ThirdParty.Android
		b. Build SDK.android
		c. Build Samples.android
		d. Open eclipse with Samples.android/java
	
	

5. How to new scecen in MStudio
	copy scene/empty and rename
	edit empty.setting, if you need
	open new scene in MStudio

