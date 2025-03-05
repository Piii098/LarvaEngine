#pragma once

class Frame;
class AIComponent;

class AIState {
public:
	AIState(AIComponent* parent);

	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual const char* GetName() const = 0;
private:
	AIComponent* _parent;
};