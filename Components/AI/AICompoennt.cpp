#include "Components/AI/AICompoennt.h"
#include "Components/AI/AIState.h"
#include "SDL3/SDL.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

AIComponent::AIComponent(GameObject* parent)
	: Component(parent){

}

AIComponent::~AIComponent() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�

void AIComponent::Update(Frame* frame) {

	if (_currentState) {
		_currentState->Update(frame);
	}

}

void AIComponent::ChangeState(const std::string& name) {

	if (_currentState) {
		_currentState->OnExit();
	}

	auto iter = _stateMap.find(name);
	if (iter != _stateMap.end()) {
		_currentState = iter->second;
		_currentState->OnEnter();
	}
	else {
		SDL_Log("The content of AIState '%s' doesn't exsist", name.c_str());
		_currentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state) {
	_stateMap.emplace(state->GetName(), state);
}

#pragma endregion

