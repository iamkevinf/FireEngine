#pragma once

void PlatformInit(void* window);
void PlatformFrame(void(*OnGUI)());
void PlatformFinalize();