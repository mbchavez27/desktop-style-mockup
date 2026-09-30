#include <cstdio>
#include <map>
#include <string>

#include <GLFW/glfw3.h> // GL declarations (same context as the ImGui backend)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ui/IconCache.h"

const AppIcon &GetIcon(const char *path)
{
    // One entry per path for the life of the app; leaked on exit like most UI textures.
    static std::map<std::string, AppIcon> cache;

    auto it = cache.find(path);
    if (it != cache.end())
        return it->second;

    AppIcon icon{};
    int w = 0;
    int h = 0;
    unsigned char *px = stbi_load(path, &w, &h, nullptr, STBI_rgb_alpha);
    if (px == nullptr)
    {
        std::fprintf(stderr, "GetIcon: cannot load '%s': %s\n", path, stbi_failure_reason());
        return cache.emplace(path, icon).first->second;
    }

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(px);

    icon.tex = static_cast<ImTextureID>(static_cast<intptr_t>(id));
    icon.w = w;
    icon.h = h;
    return cache.emplace(path, icon).first->second;
}
