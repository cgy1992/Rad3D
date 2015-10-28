
xcopy *.h ..\SDK.Win32\include\*.h /i /d /y
xcopy *.inl ..\SDK.Win32\include\*.inl /i /d /y
xcopy Win32\*.h ..\SDK.Win32\include\*.h /i /d /y

xcopy *.h ..\SDK.Android\include\*.h /i /d /y
xcopy *.inl ..\SDK.Android\include\*.inl /i /d /y

pause