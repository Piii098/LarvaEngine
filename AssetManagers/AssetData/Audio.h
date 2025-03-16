#pragma once
#include <string>

class Audio {
public:
	Audio();
	~Audio();

	bool Load(const std::string& eventPath);
	void Unload();
	
	const std::string& GetEventPath() const { return _eventPath; }

private:
	std::string _eventPath;
};