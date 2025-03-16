#include "AssetManagers/AssetData/Audio.h"

#pragma region コンストラクタ:デストラクタ

Audio::Audio()
	: _eventPath("") {
}

Audio::~Audio() {
}

#pragma endregion

#pragma region パブリック関数

bool Audio::Load(const std::string& eventPath) {
    // イベントパスを保存するだけ
    _eventPath = eventPath;

    // 実際にFMODにイベントが存在するかの検証は別途行う
    // ここでは単純にパスを保存することだけを行う

    //SDL_Log("オーディオアセット作成: %s", eventPath.c_str());
    return true;
}

#pragma endregion
