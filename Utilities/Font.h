#pragma once
#include <string>
#include "Math.h"
#include <string>

class Texture;

class Font {
public:
	Font();
	~Font();

	bool Load(const std;:string& fileName);
	void Unload();
	Texture* RenderText(const std::string& text, const Vector3& color = Color::White, int pointSize = 30);
private:
};