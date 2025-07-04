﻿#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Resources/Texture.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

Texture::Texture(Game& game)
    : _game(game)
    , _textureID(0)
	, _width(0)
	, _height(0){

}

Texture::~Texture() {

}


//==============================================================================
// パブリック関数
//==============================================================================


// ===== ロード・アンロード ===== //

/**
 * テクスチャを読み込む
 * 
 * SOILを使用し、OpenGLへテクスチャを転送しTextureIDを保持する
 */
bool Texture::Load(const std::string& fileName) {
    int channels = 0;
    unsigned char* image = SOIL_load_image(fileName.c_str(), &_width, &_height, &channels, SOIL_LOAD_AUTO);

    if (image == nullptr) {
        SDL_Log("SOIL failed to load image %s : %s", fileName.c_str(), SOIL_last_result());
        return false;
    }

    int format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, image);

    // ミップマップ生成
    glGenerateMipmap(GL_TEXTURE_2D);

    // フィルタリング設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ラップモード設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SOIL_free_image_data(image);

    return true;
}
/**
 * テクスチャをアンロードする
 * 
 * テクスチャIDを削除する
 */
void Texture::Unload() {
	glDeleteTextures(1, &_textureID);
}


/**
 * SDL_Surfaceからテクスチャを作成する
 *
 * SDL_Surfaceからテクスチャを作成し、OpenGLへテクスチャを転送する
 * TextureIDを保持する
 */
void Texture::CreateFromSurface(SDL_Surface* surface) {
    if (!surface) {
        SDL_Log("ERROR: Null surface provided to CreateFromSurfaceSafe");
        return;
    }

    // 既存のテクスチャがあれば解放
    if (_textureID != 0) {
        glDeleteTextures(1, &_textureID);
        _textureID = 0;
    }

    // サーフェスの寸法を保存
    _width = surface->w;
    _height = surface->h;

    // RGBA形式に変換 - SDL3の新しいAPIを使用
    SDL_Surface* rgbaSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    if (!rgbaSurface) {
        SDL_Log("Failed to convert surface to RGBA32: %s", SDL_GetError());
        return;
    }

    // テクスチャを生成
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // 1バイトアライメントを設定（テキストレンダリングに重要）
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // OpenGLにテクスチャデータを転送
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,     // 内部フォーマットはRGBAに固定
        _width,
        _height,
        0,
        GL_RGBA,     // データフォーマットもRGBAに固定
        GL_UNSIGNED_BYTE,
        rgbaSurface->pixels
    );

    // OpenGLエラーがあれば検出
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        SDL_Log("OpenGL error in CreateFromSurfaceSafe: %d", glError);
        SDL_DestroySurface(rgbaSurface);
        return;
    }

    // フィルタリング設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // アライメント設定をリセット
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // 変換したサーフェスを解放
    SDL_DestroySurface(rgbaSurface);

    return;
}


/**
 * テクスチャをアクティブ化する
 *
 * テクスチャIDをバインドする
 */
void Texture::SetActive() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::SetActive(int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}
