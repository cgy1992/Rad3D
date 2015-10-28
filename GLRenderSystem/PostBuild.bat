
xcopy *.h ..\SDK.Win32\include\*.h /i /d /y
xcopy *.inl ..\SDK.Win32\include\*.inl /i /d /y

xcopy ..\ThirdParty.Win32\gl\win32\glew32.dll ..\SDK.Win32\dll\debug\ /i /d /y
xcopy ..\ThirdParty.Win32\gl\win32\glew32.dll ..\SDK.Win32\dll\profile\ /i /d /y
xcopy ..\ThirdParty.Win32\gl\win32\glew32.dll ..\SDK.Win32\dll\release\ /i /d /y

xcopy *.h ..\SDK.Android\include\*.h /i /d /y
xcopy *.inl ..\SDK.Android\include\*.inl /i /d /y

pause