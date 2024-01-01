#pragma once
#include <Windows.h>

namespace gui
{
	inline bool bInit = false;
	inline bool bDetached = false;
	inline bool bShowMain = false;
	inline WNDPROC oWindProc = NULL;
	inline bool bShowWindows[8] = { false, false, false, false, false, false, false, false };

	void Init();
	void RenderWindow();
	void OnEndScene();
	void Shutdown();
}