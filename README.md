# file-activity-monitoring
* Show console in MFC
add below code to stdafx.h(before vs2019) or pch.h (vs2019)
```
#ifdef _UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
```
