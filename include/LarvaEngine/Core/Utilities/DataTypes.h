#pragma once
#include <variant>
#include <string>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_gamepad.h>

/// @brief ゲーム全体で使用する共通型定義
namespace GameTypes {

	/// @brief  データ型
	/// シーン間のデータのやり取りに使用する
    using DataValue = std::variant<std::monostate, int, float, bool>;

    // 文字列変換
    inline std::string ConvertDataToString(const DataValue& value) {
        return std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "";
            }
            else if constexpr (std::is_same_v<T, int>) {
                return std::to_string(arg);
            }
            else if constexpr (std::is_same_v<T, float>) {
                return std::to_string(arg);
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            }
            return "";
            }, value);
    }

	/// @brief マウスボタン
	/// マウスのボタンを表す(SDLには存在しないため自作)
    enum class SDL_MouseButton {
        SDL_MOUSE_LEFT = 1,
        SDL_MOUSE_RIGHT = 2,
        SDL_MOUSE_MIDDLE = 3,
        SDL_MOUSE_X1 = 4,
        SDL_MOUSE_X2 = 5
    };

	/// @brief ゲームパッドのスティック
	/// ゲームパッドのスティックを表す(SDLには存在しないため自作)
	enum class SDL_GamepadStick {
		SDL_GAMEPAD_LEFT_STICK = 1,
		SDL_GAMEPAD_RIGHT_STICK = 2
	};
    
	/// @brief マウスボタンのマスクを取得
	/// マウスボタンのマスクを取得する
    inline Uint32 GetMouseButtonMask(SDL_MouseButton button) {
        return 1u << (static_cast<int>(button) - 1);
    }

	/// @brief 複数の入力をまとめた型
	/// 入力アクションを使用する際に使用する
    using InputCode = std::variant<std::monostate, SDL_Scancode, SDL_GamepadButton, SDL_MouseButton>;
    

}