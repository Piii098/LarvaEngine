#include "Examples/3D/Sample/SampleMainScene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Renderer/3D/Mesh.h"
#include "LarvaEngine/Core/Game.h"
#include "Examples/3D/Sample/Sample3DSubScene.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/Model.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Components/Draw/MeshComponent.h"
#include "LarvaEngine/Components/Draw/ModelComponent.h"
#include "LarvaEngine/Components/Physics/Rigidbody3DComponent.h"

Sample3D::SampleMainScene::SampleMainScene(SceneManager& manager)
	: MainScene(manager)
{
}

Sample3D::SampleMainScene::~SampleMainScene()
{
}

void Sample3D::SampleMainScene::LoadData()
{
	// _manager.GetGame().GetMeshManager().Load("cube", "Assets/Mesh/Cube.gpmesh");
	_manager.GetGame().GetModelManager().Load("cube", "Assets/Model/03_table.fbx");
	_manager.GetGame().GetModelManager().Load("Gun", "Assets/Model/Gun.fbx");
	_manager.GetGame().GetTextureManager().Load("AKAGE", "Assets/Textures/AKAGE.png");
}

void Sample3D::SampleMainScene::LoadObjects()
{
	GameObject& cube = CreateGameObject<GameObject>();
	ModelComponent& model = cube.CreateComponent<ModelComponent>();
	Rigidbody3DComponent& rigidbody = cube.CreateComponent<Rigidbody3DComponent>();
	cube.Position(500.f, 0.f, 100.f);
	// cube.Rotation(Quaternion(Vector3(1.f,0.f,0.f),Math::ToRadians(90.0f)));
	// cube.Scale(3.f);
	model.SetModel("cube");
	rigidbody.Mass(1.f);
	rigidbody.Velocity(Vector3(10.f, 0.f, 0.f));
	// ChangeSubScene<Sample3D::Sample3DSubScene>(cube);
}