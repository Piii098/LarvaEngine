#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>

/*
使用方法
このクラスはアセットの管理を行います。アセットをロード、取得、アンロードする機能を提供します。
*/

/// @brief アセットマネージャークラス
/// アセットの管理を行う
/// ロード、アンロード、取得を行う
/// テンプレートでアセットクラスを指定する
/// @tparam T Load関数とUnload関数を持つアセットクラス
template <typename T>
class AssetManager {
public:

	// ===== コンストラクタ・デストラクタ ===== //
    
    AssetManager() = default; // コンストラクタ

    ~AssetManager() {
        UnloadAll(); // 全アセットをアンロード
    }


	// ===== ロード・アンロード ===== //

	/// @brief アセットのロード
	/// アセットをロードする
	/// @param assetName 登録するアセット名
	/// @param fileName ファイル名
	/// @return ロード成功ならtrue
    bool Load(const std::string& assetName, const std::string& fileName) { // アセット名とファイル名を受け取る
        // 既に存在するか確認
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            // 既に存在する場合は成功と見なす
            return true; // 成功
        }

        // 新しいインスタンスを作成
        T* asset = new T();

        // ロード処理
        if (asset->Load(fileName)) { // ファイルからアセットをロード
            // ロード成功
            _assets.emplace(assetName, std::move(asset)); // アセットをマップに追加
            return true; // 成功
        }
        else {
            // ロード失敗
            delete asset; // メモリを解放
            SDL_Log("Failed to load asset %s from file %s", assetName.c_str(), fileName.c_str()); // ログ出力
            return false; // 失敗
        }
    }

	/// @brief アセットの参照を取得
	/// アセットの参照を返し、存在しない場合はダミーを返す
    /// @param assetName 
    /// @return 
    T* Get(const std::string& assetName) const { // アセット名を受け取る

		auto iter =  _assets.find(assetName); // アセットを検索
		if (iter != _assets.end()) {
			return iter->second.get(); // アセットのポインタを返す
		}
		else {
			return nullptr; // 存在しない場合はnullptrを返す
		}
    }

	/// @brief アセットのアンロード
    /// 
	/// @param assetName アンロードするアセット名
    void Unload(const std::string& assetName) { // アセット名を受け取る
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
			iter->secomd.reset(); // メモリを解放
            _assets.erase(iter); // マップから削除
        }
    }

	/// @brief 全アセットのアンロード
    void UnloadAll() { // 全アセットをアンロード
		for (auto& asset : _assets) {
            asset.second.reset(); // メモリを解放
		}
		_assets.clear(); // マップをクリア
    }

protected:

    std::unordered_map<std::string, std::unique_ptr<T>> _assets; // アセットを格納するマップ
};