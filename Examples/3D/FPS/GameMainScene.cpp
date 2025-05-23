#include "Examples/3D/FPS/GameMainScene.h"
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
#include "Examples/3D/FPS/Player.h"
#include "Examples/3D/FPS/HUDUIScene.h"

Example3DFPS::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager){
}

Example3DFPS::GameMainScene::~GameMainScene()
{
}

void Example3DFPS::GameMainScene::LoadData()
{
	// _manager.GetGame().GetMeshManager().Load("cube", "Assets/Mesh/Cube.gpmesh");
	//_manager.GetGame().GetModelManager().Load("table", "Assets/Model/03_table.fbx");
	_manager.GetGame().GetModelManager().Load("Gun", "Assets/Model/Gun.fbx");
	_manager.GetGame().GetModelManager().Load("Stage", "Assets/Model/Stage.fbx");
	_manager.GetGame().GetModelManager().Load("Chara", "Assets/Model/Charac.fbx");
	//_manager.GetGame().GetTextureManager().Load("AKAGE", "Assets/Textures/AKAGE.png");
}

void Example3DFPS::GameMainScene::LoadObjects()
{

	GameObject& table = CreateGameObject<GameObject>();
	ModelComponent& model = table.CreateComponent<ModelComponent>();
	table.Position(10.f, 0.f, 0.f);
	model.SetModel("Gun");

	GameObject& chara = CreateGameObject<GameObject>();
	ModelComponent& charaModel = chara.CreateComponent<ModelComponent>();
	chara.Rotation(Quaternion(Vector3(0.f, 0.f, 1.f), Math::ToRadians(180.0f)));
	chara.Position(400.f, 0.f, 200.f);
	charaModel.SetModel("Chara");

	GameObject& stage = CreateGameObject<GameObject>();
	stage.Position(0.f, 0.f, -50.f);
	ModelComponent& stageModel = stage.CreateComponent<ModelComponent>();
	stageModel.SetModel("Stage");

	_camera = &CreateGameObject<Player>();

	CreateUIScene<Example3DFPS::HUDUIScene>();
}