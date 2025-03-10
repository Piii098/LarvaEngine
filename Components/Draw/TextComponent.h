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

	void CreateTextTexture(std::string text, Vector3 color, int pointSize);
	void RenderText(Shader* shader, const Vector2Int& position, float scale, Vector3 color);
	//void Render(Shader* shader) override;

private:
	Font* _font;
	Texture* _textTexture;
};