#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>

template <typename T>
class AssetManager {
public:
    AssetManager() = default;

    // デストラクタでリソースを解放
    ~AssetManager() {
        UnloadAll();
    }

    // アセットをロード
    bool Load(const std::string& assetName, const std::string& fileName) {
        // 既に存在するか確認
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            // 既に存在する場合は成功と見なす
            return true;
        }

        // 新しいインスタンスを作成
        T* asset = new T();

        // ロード処理
        if (asset->Load(fileName)) {
            // ロード成功
            _assets.emplace(assetName, asset);
            return true;
        }
        else {
            // ロード失敗
            delete asset;
            SDL_Log("Failed to load asset %s from file %s", assetName.c_str(), fileName.c_str());
            return false;
        }
    }

    // アセットの取得
    T* Get(const std::string& assetName) const {
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            return iter->second;
        }
        return nullptr;
    }

    // 特定のアセットのアンロード
    void Unload(const std::string& assetName) {
        auto iter = _assets.find(assetName);
        if (iter != _assets.end()) {
            delete iter->second;
            _assets.erase(iter);
        }
    }

    // 全アセットのアンロード
    void UnloadAll() {
        for (auto& [name, asset] : _assets) {
            if (asset) {
                delete asset;
            }
        }
        _assets.clear();
    }

protected:
    std::unordered_map<std::string, T*> _assets;
};