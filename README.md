# LarvaEngine

**2D/3D対応 ゲームエンジン開発プロジェクト**

## 📋 作品概要

### プロジェクト内容
C++とSDL3をベースとした、2Dと3Dの両方に対応したゲームエンジンです。コンポーネントベースアーキテクチャを採用し、またシーン管理システムを実装しています。

### 担当箇所・実装機能

#### **コアシステム設計**
- **コンポーネントベースアーキテクチャ**: GameObject-Componentパターンの実装
- **シーン管理システム**: MainScene/SubScene/UISceneの階層構造
- **アセット管理**: テクスチャ、モデル、オーディオの効率的な管理システム

#### **レンダリングシステム**
- **2D/3D統合レンダラー**: 低解像度レンダリングとアップスケール
- **マルチレイヤー描画**: パララックス効果による多層背景システム
- **ポストプロセシング**: ブルームエフェクト、**被写界深度（DoF）**
- **ライティングシステム**: アンビエントライト、ポイントライト対応

#### **物理エンジン**
- **2D物理演算**: 独自実装による衝突検出システム
- **固定タイムステップ**: 安定したフレームレート制御

#### **オーディオシステム**
- **FMOD統合**: 高品質な3Dオーディオ実装

### 主な工夫点

#### **1. 被写界深度（Depth of Field）の実装**
- **リアルタイム深度計算**: Zバッファを活用した焦点距離制御
- **ガウシアンブラー**: 複数パスによる高品質なボケ効果
- **フォーカス制御**: カメラからの距離に基づく動的フォーカス調整

#### **2. アーキテクチャ設計**
- **拡張性重視**: 新機能追加が容易なコンポーネント設計
- **メモリ効率**: スマートポインタによるリソース管理
- **型安全**: テンプレートを活用したアセットマネージャー

#### **3. クロスプラットフォーム対応**
- **SDL3活用**: Windows以外への移植容易性
- **標準C++17**: モダンC++機能の積極的活用

### 技術スタック
- **言語**: C++17
- **グラフィックス**: SDL3 + OpenGL + GLEW
- **オーディオ**: FMOD Studio API
- **3Dモデル**: Assimp
- **ビルド**: Visual Studio 2022

## セットアップ手順

### 1. 必要なライブラリをダウンロード
- [SDL3](https://www.libsdl.org/download-3.0.php) 

- [FMOD Studio API](https://www.fmod.com/download) 

- [Assimp](https://github.com/assimp/assimp/releases)

- [GLEW](http://glew.sourceforge.net/)

### 2. DLLファイルの配置
実行ファイルと同じディレクトリに以下を配置：
- SDL3.dll
- SDL3_ttf.dll  
- fmod.dll
- fmodstudio.dll
- glew32.dll
- zlibd1.dll
- minizip.dll
- pugixml.dll
- poly2tri.dll

## 技術詳細

### 被写界深度（Depth of Field）実装

**焦点距離による動的ブラー効果**

```cpp
// DoF.frag - 深度ベースブラーシェーダー
float depth = texture(depthTexture, texCoord).r;
float focusDistance = 10.0; // 焦点距離
float focusRange = 5.0;     // 焦点範囲

// 深度から焦点距離を計算
float blur = abs(depth - focusDistance) / focusRange;
blur = clamp(blur, 0.0, 1.0);

// ガウシアンブラーを段階的に適用
vec3 color = texture(colorTexture, texCoord).rgb;
if(blur > 0.1) {
    color = applyGaussianBlur(colorTexture, texCoord, blur * maxBlurRadius);
}
```

**主な特徴:**
- リアルタイム深度計算による自然なボケ効果
- 複数パスガウシアンブラーによる高品質な結果
- 焦点距離の動的調整が可能

### コンポーネントシステム

```cpp
class GameObject {
    template<typename T>
    T* AddComponent() {
        auto comp = std::make_unique<T>(*this);
        T* rawPtr = comp.get();
        _components.emplace_back(std::move(comp));
        return rawPtr;
    }
};

// 使用例
auto player = scene.CreateGameObject();
auto sprite = player->AddComponent<SpriteComponent>();
auto physics = player->AddComponent<Rigidbody2DComponent>();
```

### シーン管理アーキテクチャ

**階層的シーン構造**
- **MainScene**: ゲームオブジェクトの生成や、SubScene、UISceneの管理
- **SubScene**: 具体的なゲームモード実装
- **UIScene**: ユーザーインターフェース管理

```cpp
class GameMainScene : public MainScene {
public:
    void LoadData() override {
        // ゲームアセットの読み込み
        GetGame().GetTextureManager().Load("player", "Assets/player.png");
    }
    
    void LoadObjects() override {
        // ゲームオブジェクトの生成
        _player = CreateGameObject<Player>();
        _camera = CreateGameObject<Camera>();
    }
};
```

## サンプル・デモ

### 実装例一覧

1. **3D被写界深度デモ**
   - `Examples/3D/Sample/` - 基本的な3Dレンダリング
   - DoFエフェクトのリアルタイム調整
   - フォーカス切り替えデモ

2. **2Dスクロールゲーム**
   - `Examples/2D/Scroll/` - コンポーネントシステム活用
   - パララックス背景実装
   - プレイヤー入力とアニメーション
   ※現在バージョンではレンダリング上の不具合により実行できません
   　デモムービーは過去バージョンを使用しています

3. **3D FPSサンプル**
   - `Examples/3D/FPS/` - 物理演算統合
   - カメラコントローラー
   - 3Dモデル表示とライティング
