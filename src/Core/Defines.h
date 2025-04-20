#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

//shorter names for easier use
template<typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr ref<T> makeRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args) ...);
}

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr scope<T> makeScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args) ...);
}

template<typename T>
using list = std::vector<T>;

template<typename T, typename U>
using map = std::unordered_map<T, U>;