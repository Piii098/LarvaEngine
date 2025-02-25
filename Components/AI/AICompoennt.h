#pragma once
#include <string>
#include <unordered_map>
#include "Components/Component.h"

class Frame;
class AIState;

class AIComponent : public Component {
public:
	AIComponent(GameObject* parent);
	~AIComponent() override;

	void Update(Frame* frame) override;
	void ChangeState(const std::string& name);

	void RegisterState(AIState* state);
private:
	std::unordered_map<std::string, AIState*> _stateMap;
	AIState* _currentState;

};