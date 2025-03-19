#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>

/*
使用方法
このクラスはアセットの管理を行います。アセットをロード、取得、アンロードする機能を提供します。
*/
template <typename T>
class AssetManager {
public:
    AssetManager() = default; // コンストラクタ

    // デストラクタでリソースを解放
    ~AssetManager() {
        UnloadAll(); // 全アセットをアンロード
    }

    // アセットをロード
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
            _assets.emplace(assetName, asset); // アセットをマップに追加
            return true; // 成功
        }
        else {
            // ロード失敗
            delete asset; // メモリを解放
            SDL_Log("Failed to load asset %s from file %s", assetName.c_str(), fileName.c_str()); // ログ出力
            return false; // 失敗
        }
    }

    // アセットの取得
    T* Get(const std::string& assetName) const { // アセット名を受け取る
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            return iter->second; // アセットを返す
        }
        return nullptr; // 存在しない場合はnullptrを返す
    }

    // 特定のアセットのアンロード
    void Unload(const std::string& assetName) { // アセット名を受け取る
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            delete iter->second; // メモリを解放
            _assets.erase(iter); // マップから削除
        }
    }

    // 全アセットのアンロード
    void UnloadAll() { // 全アセットをアンロード
        for (auto& [name, asset] : _assets) {
            if (asset) {
                delete asset; // メモリを解放
            }
        }
        _assets.clear(); // マップをクリア
    }

protected:
    std::unordered_map<std::string, T*> _assets; // アセットを格納するマップ
};