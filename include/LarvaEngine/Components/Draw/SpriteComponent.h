﻿#pragma once

#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include <string>
#include <unordered_map>

class Shader;
template<typename T>
class AssetManager;
class Texture;
class AABB2D;

class SpriteComponent : public Component {
public:

	enum class HorizontalAlign {
		Left,   // 左揃え
		Center, // 中央揃え（デフォルト）
		Right   // 右揃え
	};

	enum class VerticalAlign {
		Top,    // 上揃え
		Middle, // 中央揃え（デフォルト）
		Bottom  // 下揃え
	};

	SpriteComponent(GameObject& parent, int bufferLayer, int drawLayer = 50);

	virtual ~SpriteComponent();

	virtual void Render(Shader& shader) const;
	void SetTexture(const std::string& textureName);
	void SetTexture(Texture* texture) { _texture = texture; }


	/*アクセサ*/

	void GetAABB(AABB2D& outAABB) const;
	AABB2D GetAABB() const;

	int TexWidth() const { return _texWidth; };
	int TexHeight() const { return _texHeight; };
	int DrawLayer() const { return _drawLayer; };

	void SelfLightColor(Vector3 color) { _selfLightColor = color; };
	void SelfLightIntensity(float intensity) { _selfLightIntensity = intensity; };

	const Vector2& TexOffset() const { return _texOffset; }
	const Vector2& TexScale() const { return _texScale; }
	void TexOffset(const Vector2& offset) { _texOffset = offset; }
	void TexScale(const Vector2& scale) { _texScale = scale; }

	void PositionOffset(const Vector2& offset) { _positionOffset = offset; }

	void Color(const Vector3& color) { _color = color; }

	void Alpha(float alpha) { _alpha = alpha; };

	void FlipX(bool flag) { _flipX = flag; };
	void FlipY(bool flag) { _flipY = flag; };

	void SetHorizontalAlign(HorizontalAlign align) { _horizontalAlign = align; };
	void SetVerticalAlign(VerticalAlign align) { _verticalAlign = align; };

	int BufferLayer() const { return _bufferLayer; };


protected:

	AssetManager<Texture>& _textureManager;

	Texture* _texture;
	int _texWidth;
	int _texHeight;

	Vector2 _texOffset;  // テクスチャの切り取り開始位置
	Vector2 _texScale;   // テクスチャの切り取りスケール

	Vector2 _positionOffset;

	Vector3 _color;

	Vector3 _selfLightColor;
	float _selfLightIntensity;

	float _alpha;

	int _drawLayer; //描写順序、昇順に描写する

	HorizontalAlign _horizontalAlign;
	VerticalAlign _verticalAlign;

	bool _flipX;
	bool _flipY;

};
