#include "GameObjects/GameObject.h"

class FollowCameraComponent;

class Camera : public GameObject {
public:
	Camera(Game* game);
	~Camera() override;

	void InputObject(Input* input) override;
	void UpdateObject(Frame* frame) override;

	Matrix4 GetViewMatrix() const;
	float Zoom() const;
private:
	FollowCameraComponent* _camera;
	float _zoom;
};