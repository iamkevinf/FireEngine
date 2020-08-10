#pragma once

void PlatformInit(void* window, int w, int h);
void PlatformFrame(void(*OnGUI)());
void PlatformFinalize();