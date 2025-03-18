#include "Components/Draw/SpriteComponent.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Utilities/Math.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "AssetManagers/AssetManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタ:デストラクタ

SpriteComponent::SpriteComponent(GameObject* parent, int bufferLayer, int drawLayer)
    : Component(parent)
    , _texture(nullptr)
    , _texWidth(0.f)
    , _texHeight(0.f)
    , _texOffset(Vector2::Zero)
    , _texScale(Vector2(1.f, 1.f))
    , _selfLightColor(Vector3(1.f, 1.f, 1.f))
    , _selfLightIntensity(0.f)
    , _flipX(false)
    , _flipY(false)
    , _positionOffset(Vector2::Zero)
    , _color(Vector3(1.f, 1.f, 1.f))
    , _horizontalAlign(HorizontalAlign::Center)
    , _drawLayer(drawLayer) {
    SetBufferLayer(bufferLayer);
    _textureManager = _parent->GetScene()->GetManager()->GetGame()->GetTextureManager();
	GetParent()->GetScene()->AddSprite(this);
}


SpriteComponent::~SpriteComponent() {
    delete _texture;
    _texture = nullptr;
    GetParent()->GetScene()->RemoveSprite(this);
}

#pragma endregion

void SpriteComponent::Render(Shader* shader) {

    if (_texture) {
        float scaleX = _flipX ? -1.f : 1.f;
        float scaleY = _flipY ? -1.f : 1.f;

        // テクスチャの実際の幅（スケールを適用した値）
        float actualWidth = static_cast<float>(_texWidth) * _texScale.x;

        // 水平方向の位置調整オフセット値を計算
        float horizontalOffset = 0.0f;

        switch (_horizontalAlign) {
        case HorizontalAlign::Left:
            // 左揃えの場合、テクスチャの半分の幅を右に移動
            horizontalOffset = actualWidth * 0.5f;
            break;

        case HorizontalAlign::Right:
            // 右揃えの場合、テクスチャの半分の幅を左に移動
            horizontalOffset = -actualWidth * 0.5f;
            break;

        case HorizontalAlign::Center:
        default:
            // 中央揃えの場合、位置調整は不要
            horizontalOffset = 0.0f;
            break;
        }

        // スケールマトリックスの作成
        Matrix4 scaleMat = Matrix4::CreateScale(
            scaleX * actualWidth,
            scaleY * static_cast<float>(_texHeight) * _texScale.y,
            1.0f);

        // 平行移動マトリックスの作成（水平方向の揃えを考慮）
        Matrix4 translationMat = Matrix4::CreateTranslation(
            Vector3(_positionOffset.x + horizontalOffset, _positionOffset.y, 0.0f));

        // ワールド変換マトリックスの計算
        Matrix4 world = scaleMat * translationMat * _parent->WorldTransform();

        // 以下は既存のコード
        shader->SetMatrixUniform("uWorldTransform", world);
        shader->SetVector2Uniform("uTexOffset", _texOffset);
        shader->SetVector2Uniform("uTexScale", _texScale);
        shader->SetVector3Uniform("uColor", _color);

        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::SetTexture(const std::string& textureName) {
    _texture = _textureManager->Get(textureName);
    if (_texture) {
        _texWidth = _texture->Width();
        _texHeight = _texture->Height();
    }
    else {
        SDL_Log("Failed to load texture: %s", textureName.c_str());
    }
}