#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>

class Game;
class LightComponent;

class Renderer {
public:

	virtual ~Renderer() = default;

	/// @brief レンダラーの初期化
	virtual bool Initialize(const std::string& windowName) = 0;

	/// @brief レンダラーの描写処理

	virtual void Render() = 0;
	
	void SetWindowName(const std::string& name) {
		_windowName = name;
	}

	void SetWindowSize(int width, int height) {
		_windowWidth = width;
		_windowHeight = height;
	}

	void SetLowResSize(int width, int height) {
		_lowResWidth = width;
		_lowResHeight = height;
	}

	virtual void AddLight(LightComponent* light) {};
	virtual void RemoveLight(LightComponent* light) {};

	SDL_Window* GetWindow() const {
		return _window;
	}

protected:


	Renderer(Game& game) : _game(game) {
		// Constructor implementation
	}

	SDL_Window* _window = nullptr;
	SDL_GLContext _context = nullptr;

	std::string _windowName;
	int _windowWidth = -1;
	int _windowHeight = -1;
	int _lowResWidth = -1;
	int _lowResHeight = -1;

	Game& _game;
};