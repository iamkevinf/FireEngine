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
		static void OnTick(float dTime);
		static void OnExit();
		static bgfx::TextureHandle GetTexture();
	};
}

#endif // __GAME_VIEW_HPP

