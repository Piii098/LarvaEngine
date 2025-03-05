#pragma once
#include <string>


class Texture {
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int Width() const { return _width; };
	int Height() const { return _height; };

private:
	unsigned int _textureID;
	int _width;
	int _height;
};