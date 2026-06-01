#pragma once

#include "CommonInclude.h"

struct OtherPlayerMove
{
	std::string playerId;
	float xPos;
	float yPos;
	float speed;
};

struct ExpUpdateResult
{
    int64_t gainedExp = 0;
    int newLevel = 1;
    int64_t curExp = 0;
    int64_t needExp = 0;
    bool levelUp = false;
};