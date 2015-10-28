
xcopy *.h ..\SDK.Win32\include\*.h /i /d /y
xcopy *.inl ..\SDK.Win32\include\*.inl /i /d /y
xcopy Platform\win32\*.h ..\SDK.Win32\include\*.h /i /d /y
xcopy ..\ThirdParty\xml\*.hpp ..\SDK.Win32\include\*.hpp /i /d /y

xcopy *.h ..\SDK.Android\include\*.h /i /d /y
xcopy *.inl ..\SDK.Android\include\*.inl /i /d /y
xcopy Platform\android\*.h ..\SDK.Android\include\*.h /i /d /y
xcopy ..\ThirdParty\xml\*.hpp ..\SDK.Android\include\*.hpp /i /d /y

pause