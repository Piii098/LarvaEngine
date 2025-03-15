#pragma
#include "Components/Draw/TextureComponent.h"
#include <string>
#include "Utilities/Math.h"

class Font;
class Texture;
class Shader;

class TextComponent : public TextureComponent {
public:
	TextComponent(GameObject* gameObject,int bufferLayer, std::string fontName);
	~TextComponent() override;

	void CreateTextTexture(std::string text, Vector3 color, int pointSize, bool isOutline = false);
	//void Render(Shader* shader) override;

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
	Texture* _textTexture;
	Texture* _outlineTexture;
};