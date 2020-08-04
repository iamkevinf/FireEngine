#include "wrapper.h"

#include "../app/app.h"
#include "../loader/loader.h"

namespace FireEngine
{
    int feApp_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
    {
        return WindowMain(title, OnInit, OnGUI, OnExit);
    }

    bool feApp_SetWindowTitle(const char16_t* title)
    {
        return SetWindowTitle(title);
    }

    bgfx::TextureHandle feLoadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
    {
        return loadTexture(_name, _flags, _skip, _info, _orientation);
    }

    bgfx::ShaderHandle feLoadShader(const char* name)
    {
        return loadShader(name);
    }

}

