#ifndef __LAYER_H__
#define __LAYER_H__

namespace FireEngine
{
	enum class Layer
	{
		Default = 0,
		TransparentFX = 1,
		IgnoreRaycast = 2,
		Highlighting = 3,
		Water = 4,
		UI = 5,
		Scene = 6,
		Terrain = 7,
		Character = 8,
		Editor = 31
	};
}

#endif // __LAYER_H__