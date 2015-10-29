
xcopy *.h ..\..\SDK.Win32\tools\MStudio\*.h /i /d /y

xcopy ..\Util\*.h ..\..\SDK.Win32\tools\Util\*.h /i /d /y
xcopy ..\Util\*.cpp ..\..\SDK.Win32\tools\Util\*.cpp /i /d /y

del ..\..\SDK.Win32\tools\MStudio\stdafx.h

pause