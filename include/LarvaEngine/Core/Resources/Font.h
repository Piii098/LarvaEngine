#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>
#include "LarvaEngine/Core/Utilities/Math.h"

// ===== 前方宣言 ===== //
class Texture;

/// @brief フォントクラス(アセット)
/// TTFフォントを読み込み、テキストを描画する
/// テキストの描画はテクスチャとして返す
/// アセットマネージャーで管理する
class Font {
public:

	// ===== コンストラクタ・デストラクタ ===== //

	Font();
	~Font();

	// ===== ロード・アンロード ===== //

	/// @brief フォントを読み込む
	/// 10,20,30,40,50,60,70,80,90,100のサイズで読み込む
	/// @param fileName ファイル名
	/// @return 読み込み成功ならtrue
	bool Load(const std::string& fileName);

	/// @brief フォントをアンロードする
	/// フォントを削除する
	void Unload();

	// ===== テキスト描画 ===== //

	Texture* RenderTextCached(const std::string& text, const Vector3& color = Color::White, int pointSize = 30);

	void ClearTextureCache();

private:

	/// @brief テキストを描画する
	/// テキストを描画し、テクスチャとして返す
	/// 10,20,30,40,50,60,70,80,90,100のフォントサイズに対応
	/// @param text 描画するテキスト
	/// @param color テキストの色
	/// @param pointSize フォントサイズ
	/// @return 描画したテキストのテクスチャ
	std::unique_ptr<Texture> RenderText(const std::string& text, const Vector3& color = Color::White, int pointSize = 30);


	std::unordered_map<int, TTF_Font*> _textMap;	///< フォントマップ(フォントサイズとフォントデータ)
	std::unordered_map<std::string, std::unique_ptr<Texture>>_textureCache;
};

// 余地: フォントのアウトラインを追加する
// 備考: TTF_Fontは生ポインタ(SDLより生ポインタで受け取るため)だが、Fontクラス内で閉じているためメモリリークの可能性は低い