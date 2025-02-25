#pragma once

#include "Component.h"
#include "Utilities/Texture.h"
#include <string>
#include <unordered_map>

class Shader;

class TextureComponent : public Component {
public:

	#pragma region スプライト管理クラス

	class TextureManager {
	public:
		TextureManager();
		~TextureManager() = default;

		void Load(const std::string& fileName);
		void Unload();

		Texture* GetTexture(const std::string& textureName);
	private:
		std::unordered_map<std::string, Texture*> _textures;

	};

	#pragma endregion

	TextureComponent(GameObject* parent, int drawLayer = 150);
    virtual ~TextureComponent();

    virtual void Draw(Shader* shader);
    void SetTexture(const std::string& textureName);

	int TexWidth() const { return _texWidth; };
	int TexHeight() const { return _texHeight; };
	int DrawLayer() const { return _drawLayer; };
	static TextureManager& S_TextureManager() { return s_TextureManager; };

	void SelfLightColor(Vector3 color) { _selfLightColor = color; };
	void SelfLightIntensity(float intensity) { _selfLightIntensity = intensity; };

	void FlipX(bool flag) { _flipX = flag; };
	void FlipY(bool flag) { _flipY = flag; };

protected:

	static TextureManager s_TextureManager;

	Texture* _texture;
    int _texWidth;
    int _texHeight;

	Vector3 _selfLightColor;
	float _selfLightIntensity;

	int _drawLayer; //描写順序、昇順に描写する

	bool _flipX;
	bool _flipY;

};
