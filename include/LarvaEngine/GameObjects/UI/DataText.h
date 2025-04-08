#pragma once
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Core/Utilities/DataTypes.h"

class DataText : public Text {
public:
	DataText(Scene& scene, const std::string& fontName, const Vector3& color, int pointSize);

	~DataText() override;

	void SetData(const std::string& dataName);

	void SetLabel(const std::string& label);

	void UpdateObject(float deltaTime) override;

private:

	std::string FormatText() const;
	
	std::string _labelText;

	std::string _dataName;
	GameTypes::DataValue _cachedData;

};