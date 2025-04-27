#pragma
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include <string>
#include <GL/glew.h>
#include "LarvaEngine/Core/Utilities/Math.h"

class Font;
class Texture;
class Shader;

class TextComponent : public SpriteComponent {
public:
	TextComponent(GameObject& parent,int bufferLayer, std::string fontName);
	~TextComponent() override;

	void CreateTextTexture(std::string text, Vector3 color, int pointSize);
	//void Render(Shader& shader) override;

	std::string GetText() const { return _text; };
	std::string GetFontName() const { return _fontName; };
	int GetPointSize() const { return _pointSize; };
	Vector3 GetTextColor() const { return _textColor; };

private:
	std::string _text;
	std::string _fontName;
	int _pointSize;
	Font* _font;
	Vector3 _textColor;

};

// 備考: テクスチャ自体はFontクラスで管理しているため、TextComponentはテクスチャのポインタを持つだけでよい