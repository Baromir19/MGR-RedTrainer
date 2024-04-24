#pragma once
#include <Windows.h>

class gui
{
public:
	static inline bool bInit = false;
	static inline bool bDetached = false;
	static inline bool bShowMain = false;
	static inline WNDPROC oWindProc = NULL;
	static inline bool bShowWindows[8] = { false, false, false, false, false, false, false, false };

	static inline void Init();
	static inline void RenderWindow();
	static inline void OnEndScene();
	static inline void Shutdown();

	gui()
	{
		Init();
	}
}