#pragma once
#include <lib.h>

namespace gui
{
	inline bool bInit = false;
	inline bool bDetached = false;
	inline bool bShowMain = false;
	inline bool bTestWindow = false;

	struct bWindowStruct
	{
		bool bShow;
		cVec2 vecWindowPos;
		void(__stdcall* render)(bWindowStruct*);

		bWindowStruct()
		{
			bShow = false;
			render = nullptr;
		}

		bWindowStruct(void(__stdcall* render)(bWindowStruct*)) : render(render)
		{
			bShow = false;
		};

		void Draw()
		{
			if (bShow && render && bShowMain)
				render(this);
		}
	};

	inline lib::StaticArray<bWindowStruct, 8> windows;

	inline void HideAllWindows(bWindowStruct *exception = nullptr)
	{
		for (auto& window : windows)
		{
			if (&window != exception)
				window.bShow = false;
		}
	}

	void RenderWindow();

	namespace OnReset
	{
		void Before();
		void After();
	}

	void LoadStyle();
	void setGUIWindows();

	void OnEndScene();
}