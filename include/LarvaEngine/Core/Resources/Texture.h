#pragma once
#include <string>
#include "LarvaEngine/Core/Utilities/Math.h"

// ===== 前方宣言 ===== //
struct SDL_Surface;


/// @brief テクスチャクラス(アセット)
/// SOILを使用してテクスチャを読み込み、textureIDを保持する
/// テクスチャのアクティブ化を行う、アセットマネージャーで管理する
class Texture {
public:

	// ===== コンストラクタ・デストラクタ ===== //

	Texture();
	~Texture();

	// ===== ロード・アンロード ===== //

	/// @brief テクスチャを読み込む
	/// 補正なしで読み込む
	/// @param fileName ファイル名
	/// @return 読み込み成功ならtrue
	bool Load(const std::string& fileName);
	
	/// @brief テクスチャをアンロードする
	/// テクスチャIDを削除する
	void Unload();
	
	/// @brief SDL_Surfaceからテクスチャを作成する
	/// 
	/// @param surface 変換させるSDL_Surface
	void CreateFromSurface(SDL_Surface* surface);

	/// @brief テクスチャをアクティブ化する
	/// テクスチャIDをバインドする
	void SetActive();

	// ===== ゲッター ===== //

	int Width() const { return _width; };
	int Height() const { return _height; };

private:

	unsigned int _textureID;	///< テクスチャID
	int _width;				    ///< 幅
	int _height;			    ///< 高さ
	
};