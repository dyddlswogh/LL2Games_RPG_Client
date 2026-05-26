#pragma once
#include <string>
#include "stbMath.h"
struct AnimationInfo
{
	std::string anim_name;
	std::string frame_prefix;
	std::string path;
	int frame_count;
	int delay_ms;
};

struct RenderInfo
{
	int width;
	int height;
	stb::math::Vector2 origin = stb::math::Vector2::Zero;
};