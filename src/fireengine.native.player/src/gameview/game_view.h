#ifndef __GAME_VIEW_HPP
#define __GAME_VIEW_HPP

namespace FireEngine
{
	class GameView
	{
	public:
		static void OnInit();
		static void OnTick();
		static void OnEditorGUI();
		static void OnExit();
	};
}

#endif // __GAME_VIEW_HPP

