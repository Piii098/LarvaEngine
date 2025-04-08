#include "LarvaEngine/GameObjects/UI/DataText.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"

DataText::DataText(Scene& scene, const std::string& fontName, const Vector3& color, int pointSize)
	: Text(scene, fontName, color, pointSize, " ") {
}

DataText::~DataText() {
}

void DataText::SetData(const std::string& dataName) {
	_dataName = dataName;
	_cachedData = GetMainScene().GetData(dataName);

	CreateText(_textComp->GetFontName(), _textComp->GetTextColor(), _textComp->GetPointSize(), FormatText());
}

void DataText::SetLabel(const std::string& label) {
	_labelText = label;

	std::string text = _labelText + GameTypes::ConvertDataToString(_cachedData);

	CreateText(_textComp->GetFontName(), _textComp->GetTextColor(), _textComp->GetPointSize(), FormatText());
}

void DataText::UpdateObject(float deltaTime) {

	GameTypes::DataValue newData = GetMainScene().GetData(_dataName);
	if (_cachedData != newData) {
		_cachedData = newData;
		CreateText(_textComp->GetFontName(), _textComp->GetTextColor(), _textComp->GetPointSize(), FormatText());
	}

}

std::string DataText::FormatText() const {
	std::string dataStr = GameTypes::ConvertDataToString(_cachedData);

	// ラベルがある場合はラベルとデータを結合
	if (!_labelText.empty()) {
		return _labelText + ": " + dataStr;
	}

	// ラベルがない場合はデータのみ
	return dataStr;
}
