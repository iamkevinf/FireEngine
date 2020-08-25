#ifndef __GAME_VIEW_HPP
#define __GAME_VIEW_HPP

namespace bgfx
{
	struct TextureHandle;
}

namespace FireEngine
{
	class GameView
	{
	public:
		static void OnInit();
		static void OnGui();
		static void OnTick(float dTime);
		static void OnExit();
	};
}

#endif // __GAME_VIEW_HPP

