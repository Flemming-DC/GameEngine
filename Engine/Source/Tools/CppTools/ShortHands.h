#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stduuid/uuid.h>
#include "glm/glm.hpp"

#define UsingShorts \
using uint = unsigned int;\
using vec2 = glm::vec2;\
using vec3 = glm::vec3;\
using vec4 = glm::vec4;\
using mat4 = glm::mat4;\
using uuid = uuids::uuid;\
using string = std::string;\
template<typename T> using vector = std::vector<T>;\
template<typename T> using unique_ptr = std::unique_ptr<T>;\
template<typename T> using optional = std::optional<T>;\
template<typename T1, typename T2> using map = std::map<T1, T2>;\
template<typename T1, typename T2> using unordered_map = std::unordered_map<T1, T2>;\
template<typename T1, typename T2> using map_uo = std::unordered_map<T1, T2>;\
template<typename T1, typename T2> using pair = std::pair<T1, T2>;\
;

/*
using std::vector;\
using std::string;\
using std::unique_ptr;\
using std::optional;\
using std::map;\
using std::unordered_map;\
using std::pair;\

template<typename T> using unique_ptr = std::unique_ptr<T>;\
template<typename T> using optional = std::optional<T>;\
template<typename T1, typename T2> using map = std::map<T1, T2>;\
template<typename T1, typename T2> using unordered_map = std::unordered_map<T1, T2>;\
template<typename T1, typename T2> using pair = std::pair<T1, T2>;\


typedef unsigned int uint;\
template<typename Tkey, typename Tval> using map_o = std::map<Tkey, Tval>;
template<typename Tkey, typename Tval> using uo_map = std::unordered_map<Tkey, Tval>;\

*/

