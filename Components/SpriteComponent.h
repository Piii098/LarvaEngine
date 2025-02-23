#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>
#include  "Components/Component.h"


class GameObject;
class Shader;
class Texture;

/*
事前にロードされたスプライトをセットし、
*/

class SpriteComponent : public Component {
public:

	#pragma region スプライト管理クラス

	class SpriteManager {
	public:
		SpriteManager();
		~SpriteManager() = default;

		void Load(const std::string& fileName);
		void Unload();

		Texture* GetTexture(const std::string& textureName);
	private:
		std::unordered_map<std::string, Texture*> _textures;

	};

	#pragma endregion

	SpriteComponent(GameObject* parent, int drawLayer = 150);
	~SpriteComponent();

	virtual void Draw(Shader* shader);

	virtual void SetTexture(const std::string& textureName);

	virtual bool IsTileMap() const { return false; };

	/*ゲッターセッター*/

	int TexWidth() const { return _texWidth; };
	int TexHeight() const { return _texHeight; };
	int DrawLayer() const { return _drawLayer; };
	static SpriteManager& S_SpriteManager() { return s_SpriteManager; };

	void FlipX(bool flag) { _flipX = flag; };
	void FlipY(bool flag) { _flipY = flag; };

protected:
	
	static SpriteManager s_SpriteManager;
	
	Texture* _texture;
	int _drawLayer; //描写順序、昇順に描写する
	int _texWidth;
	int _texHeight;

	bool _flipX;
	bool _flipY;

};