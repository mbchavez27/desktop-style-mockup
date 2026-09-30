#pragma once

#include "imgui.h" // ImTextureID

// A PNG loaded once into an OpenGL texture. Empty (tex == 0) on failure.
struct AppIcon
{
    ImTextureID tex = 0;
    int w = 0;
    int h = 0;

    bool ok() const { return tex != 0; }
};

// Loads assets/images/<file> on first use, returns the cached texture after.
// UI thread only. Never throws; missing files log to stderr and yield !ok().
const AppIcon &GetIcon(const char *path);
