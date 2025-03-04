#pragma once

#include "Components/Component.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Utilities/Math.h"
#include <string>
#include <unordered_map>

class Shader;
template<typename T>
class AssetManager;
class Texture;

class TextureComponent : public Component {
public:

	TextureComponent(GameObject* parent, int drawLayer = 150);
    virtual ~TextureComponent();

    virtual void Draw(Shader* shader);
    void SetTexture(const std::string& textureName);


	/*アクセサ*/

	int TexWidth() const { return _texWidth; };
	int TexHeight() const { return _texHeight; };
	int DrawLayer() const { return _drawLayer; };

	void SelfLightColor(Vector3 color) { _selfLightColor = color; };
	void SelfLightIntensity(float intensity) { _selfLightIntensity = intensity; };

	const Vector2& TexOffset() const { return _texOffset; }
	const Vector2& TexScale() const { return _texScale; }
	void TexOffset(const Vector2& offset) { _texOffset = offset; }
	void TexScale(const Vector2& scale) { _texScale = scale; }

	void FlipX(bool flag) { _flipX = flag; };
	void FlipY(bool flag) { _flipY = flag; };

protected:

	AssetManager<Texture>* _textureManager;

	Texture* _texture;
    int _texWidth;
    int _texHeight;

	Vector2 _texOffset;  // テクスチャの切り取り開始位置
	Vector2 _texScale;   // テクスチャの切り取りスケール

	Vector3 _selfLightColor;
	float _selfLightIntensity;

	int _drawLayer; //描写順序、昇順に描写する

	bool _flipX;
	bool _flipY;

};
