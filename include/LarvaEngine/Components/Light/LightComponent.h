﻿#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class LightComponent : public Component {
public:
	LightComponent(GameObject& parent,int bufferLayer, int updateLayer = 150);
	~LightComponent() override;



	void RenderLight(Shader& shader);

	/*ゲッターセッター*/
	void LightScale(const Vector2& scale) { _lightScale = scale; };
	void LightOffset(const Vector2& offset) { _lightOffset = offset; };
	void LightColor(const Vector3& color) { _lightColor = color; };
	void LightIntensity(float intensity) { _lightIntensity = intensity; };
	void LightRange(float range) { _lightRange = range; };

	Vector3& LightColor()  { return _lightColor; };
	float LightIntensity() const  { return _lightIntensity; };
	float LightRange() const { return _lightRange; };
	int BufferLayer() const { return _bufferLayer; };

private:

	Vector3 _lightColor;
	float _lightIntensity;
	Vector2 _lightScale;

	Vector3 _lightOffset;

	int _bufferLayer;
	float _lightRange;
};