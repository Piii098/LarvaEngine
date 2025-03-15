#pragma once
#include <variant>
#include <string>

namespace GameTypes {
    // �Q�[���S�̂Ŏg�p���鋤�ʌ^��`
    using DataValue = std::variant<std::monostate, int, float, bool>;

    // ������ϊ�
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
}