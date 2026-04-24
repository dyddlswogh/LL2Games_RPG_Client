#pragma once
#include "CommonInclude.h"
#include "math.h"

struct BaseStat
{
	int str;
	int dex;
	int intel;
	int luck;
};

struct DerivedStat
{
	int maxHp;
	int maxMp;
};

struct PlayerIdentity
{
	int charId = 0;
	int accountId = 0;
};

struct PlayerProfile
{
	std::string name;
	int level = 0;
	int rootJob = 0;
	int job = 0;
};

struct PlayerLocation
{
	int mapId = 0;
	stb::math::Vector2 pos;
};

enum class PlayerState
{
	IDLE,
	MOVE,
	JUMP,
	ATTACK,
	STUNNED,
	DEAD
};