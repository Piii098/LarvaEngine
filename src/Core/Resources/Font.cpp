#include <vector>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/Resources/Texture.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

Font::Font(Game& game)
    : _game(game) {

}

Font::~Font() {
    if (!_textMap.empty()) {
        Unload();
    }
}

//==============================================================================
// パブリック関数
//==============================================================================

// ===== ロード・アンロード ===== //

/**
 * フォントを読み込む
 * 
 * TTFフォントを読み込み、フォントサイズとフォントデータを保持する
 */
bool Font::Load(const std::string& fileName) {

	// フォントサイズのリスト
    std::vector<int> fontSizes = {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100
    };

	// フォントサイズごとにフォントを読み込む
    for (auto& size : fontSizes) {

        TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
        if (font == nullptr) {
            SDL_Log("Failed to load font %s with size %d: %s",fileName.c_str(), size, SDL_GetError());
            return false;
        }
        _textMap.emplace(size, font);
    }

    return true;
}

/**
 * フォントをアンロードする
 *
 * フォントを削除する
 */
void Font::Unload() {
	for (auto& font : _textMap) {
		TTF_CloseFont(font.second);
	}
	_textMap.clear();
}

// ===== テキスト描画 ===== //


/**
 * テキストを描画する
 *
 * SDL_ttfを使用してテキストを描画し、SDL_SurfaceをOpenGL用のテクスチャクラスに変換し返す
 * RenderText_Blendedを使用してテキストを描画
 */
Texture* Font::RenderTextCached(const std::string& text, const Vector3& color, int pointSize) {

    std::string cacheKey = text + "_" +
        std::to_string(color.x) + "_" +
        std::to_string(color.y) + "_" +
        std::to_string(color.z) + "_" +
        std::to_string(pointSize);

    auto iter = _textureCache.find(cacheKey);
    if (iter != _textureCache.end()) {
        return iter->second.get(); // キャッシュから取得
    }

    std::unique_ptr<Texture> newTexture = RenderText(text, color, pointSize);
    if (newTexture) {
		_textureCache[cacheKey] = std::move(newTexture); // キャッシュに追加
    }
    return _textureCache[cacheKey].get();

}

/**
 * テクスチャキャッシュをクリアする
 *
 * テクスチャキャッシュをクリアする
 */
void Font::ClearTextureCache() {
	_textureCache.clear();
}

/**
 * テキストを描画する
 *
 * SDL_ttfを使用してテキストを描画し、SDL_SurfaceをOpenGL用のテクスチャクラスに変換し返す
 * RenderText_Blendedを使用してテキストを描画
 */
std::unique_ptr<Texture> Font::RenderText(const std::string& text, const Vector3& color, int pointSize) {

    // SDLカラーを正しく設定
    SDL_Color sdlColor{};
    sdlColor.r = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.b = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    auto iter = _textMap.find(pointSize);
    if (iter != _textMap.end()) {
        TTF_Font* font = iter->second;

		TTF_SetFontOutline(font, 0);

        SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), 0, sdlColor);
   
        if (surf != nullptr) {
            auto texture = std::make_unique<Texture>(_game);
            texture->CreateFromSurface(surf);
            SDL_DestroySurface(surf);

            return std::move(texture); // 呼び出し元に所有権を渡す
        }
    }
    else {
        SDL_Log("Font size %d not found in font map", pointSize);
    }

    return nullptr;
}


