#include <vector>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include "Utilities/Font.h"
#include "AssetManagers/AssetData/Texture.h"

#pragma region コンストラクタ:デストラクタ

Font::Font() {
}

Font::~Font() {
}

#pragma endregion

#pragma region パブリック関数

bool Font::Load(const std::string& fileName) {

	std::vector<int> fontSizes = {
		10, 20, 30, 40, 50, 60, 70, 80, 90, 100
	};

	for (auto& size : fontSizes) {
		TTF_Font* font = TTF_OpenFont("Assets/DelaSukoGothicOne-R.ttf", size);
		if (font == nullptr) {
			SDL_Log("Failed to load font %s", fileName.c_str());
			return false;
		}
		_textMap.emplace(size, font);
	}

	return true;
}

void Font::Unload() {
	for (auto& font : _textMap) {
		TTF_CloseFont(font.second);
	}
	_textMap.clear();
}

Texture* Font::RenderText(const std::string& text, const Vector3& color, int pointSize) {
    Texture* texture = nullptr;

    // SDLカラーを正しく設定
    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.b = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    auto iter = _textMap.find(pointSize);
    if (iter != _textMap.end()) {
        TTF_Font* font = iter->second;

        // SDL3_ttfの正しいシグネチャを使用
        // text.c_str()は文字列、0はヌル終端文字列を使用することを示す
        SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), 0, sdlColor);
   
        if (surf != nullptr) {
            texture = new Texture();
            
            texture->CreateFromSurface(surf);
            SDL_DestroySurface(surf);
        }
    }
    else {
        SDL_Log("Font size %d not found in font map", pointSize);
    }

    return texture;
}

#pragma endregion

