# LarvaEngine

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

### 3. ビルド
```bash
git clone https://github.com/username/LarvaEngine.git
cd LarvaEngine
# 上記ライブラリをセットアップ後
# Visual Studioでビルド