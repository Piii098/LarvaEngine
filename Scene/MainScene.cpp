#include "Scene/MainScene.h"
#include "Scene/UI/UIScene.h"
#include <algorithm>

#pragma region コンストラクタデストラクタ

MainScene::MainScene(SceneManager* manager)
	: Scene(manager) {
}

MainScene::~MainScene() {
}

#pragma endregion

#pragma region パブリック関数

void MainScene::Initialize() {
	LoadData();
}

void MainScene::InputScene(Input* input) {
	if (_currentSubScene != nullptr) {
		_currentSubScene->InputScene(input);
	}

	for (auto& ui : _uiScenes) {
		if (ui->GetState() == Scene::STATE::ACTIVE) {
			ui->ProcessInput(input);
		}
	}
}

void MainScene::UpdateScene(float deltaTime) {
	if (_currentSubScene != nullptr) {
		_currentSubScene->UpdateScene(deltaTime);
	}

	for (auto& ui : _uiScenes) {
		if (ui->GetState() == Scene::STATE::ACTIVE) {
			ui->Update(deltaTime);
		}
	}

	for (auto ui : _uiScenes) {
		if (ui->GetState() == Scene::STATE::CLOSE) {
			delete ui;
			ui = nullptr;
		}
	}

}

void MainScene::Render(Shader* shader, int bufferLayer) {

	for (auto& obj : _objects) {
		obj->Render(shader, bufferLayer);
	}

}

void MainScene::RenderUIs(Shader* shader) {
	for (auto& ui : _uiScenes) {
		ui->Render(shader);
	}
}

void MainScene::AddUIScene(UIScene* uiScene) {
	_uiScenes.emplace_back(uiScene);
}

void MainScene::RemoveUIScene(UIScene* uiScene) {
	auto iter = std::find(_uiScenes.begin(), _uiScenes.end(), uiScene);
	if (iter != _uiScenes.end()) {
		std::iter_swap(iter, _uiScenes.end() - 1);
		_uiScenes.pop_back();
	}
}



void MainScene::Shutdown() {
}

void MainScene::SetData(const std::string& key, GameTypes::DataValue value) {
	
	_data[key] = value;
}

void MainScene::SetGetter(const std::string& key, std::function<GameTypes::DataValue()> getter) {
	_getter[key] = getter;
}

GameTypes::DataValue MainScene::GetData(const std::string& key) {
	// まずデータが存在するか確認
	auto iterData = _data.find(key);
	if (iterData == _data.end()) {
		return std::monostate{};  // データが存在しない
	}

	// ゲッターがあるか確認
	auto iterGetter = _getter.find(key);
	if (iterGetter != _getter.end()) {

		// ゲッターが存在する場合は現在値を取得して比較
		auto currentValue = iterGetter->second();
		auto& sceneValue = iterData->second;
		if (sceneValue != currentValue) {
			// 値が変わっていれば更新
			sceneValue = currentValue;
		}

	}

	// 最終的には_data内の値を返す
	return iterData->second;
}

#pragma endregion