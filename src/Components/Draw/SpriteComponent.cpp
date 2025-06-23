#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Renderer/2D/Renderer2D.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Physics/Collision2D.h"

#pragma region コンストラクタ:デストラクタ

SpriteComponent::SpriteComponent(GameObject& parent, int bufferLayer, int drawLayer)
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
    , _alpha(1.f)
    , _horizontalAlign(HorizontalAlign::Center)
    , _verticalAlign(VerticalAlign::Middle)
    , _drawLayer(drawLayer)
    , _textureManager(_parent.GetScene().GetManager().GetGame().GetTextureManager()) {
    SetBufferLayer(bufferLayer);
    _parent.GetScene().AddSprite(this);
}


SpriteComponent::~SpriteComponent() {
    _parent.GetScene().RemoveSprite(this);
}

#pragma endregion

void SpriteComponent::Render(Shader& shader) const {

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

        float actualHeight = static_cast<float>(_texHeight) * _texScale.y;
        // 垂直方向の位置調整オフセット値を計算
        float verticalOffset = 0.0f;
        switch (_verticalAlign) {
        case VerticalAlign::Top:
            // 上揃えの場合、テクスチャの半分の高さを下に移動
            verticalOffset = -actualHeight * 0.5f;
            break;
        case VerticalAlign::Bottom:
            // 下揃えの場合、テクスチャの半分の高さを上に移動
            verticalOffset = +actualHeight * 0.5f;
            break;
        case VerticalAlign::Middle:
        default:
            break;
        }

        // スケールマトリックスの作成
        Matrix4 scaleMat = Matrix4::CreateScale(
            scaleX * actualWidth,
            scaleY * actualHeight,
            1.0f);

        // 平行移動マトリックスの作成（水平方向の揃えを考慮）
        Matrix4 translationMat = Matrix4::CreateTranslation(
            Vector3(_positionOffset.x + horizontalOffset, _positionOffset.y + verticalOffset, 0.0f));

        // ワールド変換マトリックスの計算
        Matrix4 world = scaleMat * translationMat * _parent.WorldTransform();

        // 以下は既存のコード
        shader.SetMatrixUniform("uWorldTransform", world);
        shader.SetVector2Uniform("uTexOffset", _texOffset);
        shader.SetVector2Uniform("uTexScale", _texScale);
        shader.SetVector3Uniform("uColor", _color);
        shader.SetFloatUniform("uAlpha", _alpha);

        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::SetTexture(const std::string& textureName) {
    _texture = _textureManager.Get(textureName);
    if (_texture) {
        _texWidth = _texture->Width();
        _texHeight = _texture->Height();
    }
    else {
        SDL_Log("Failed to load texture from SpriteComponent: %s", textureName.c_str());
    }
}

void SpriteComponent::GetAABB(AABB2D& outAABB) const {
    int halfWidth = static_cast<int>(_texWidth * _texScale.x / 2);
    int halfHeight = static_cast<int>(_texHeight * _texScale.y / 2);

    outAABB._min = Vector2(-halfWidth, -halfHeight);
    outAABB._max = Vector2(halfWidth, halfHeight);
}

AABB2D SpriteComponent::GetAABB() const {
    AABB2D outAABB(Vector2::Zero, Vector2::Zero);
    GetAABB(outAABB);
    return outAABB;
}