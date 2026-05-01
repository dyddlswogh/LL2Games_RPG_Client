#pragma once
#include "CommonInclude.h"

//payload.push_back(std::to_string(Attacker->GetId()));
//payload.push_back(std::to_string(SkillID));
//payload.push_back(std::to_string(result.size()));
//payload.push_back(std::to_string(r.monster_instance_id));
//payload.push_back(std::to_string(r.damage));
//payload.push_back(std::to_string(r.cur_hp));
//payload.push_back(std::to_string(r.max_hp));
//payload.push_back(r.dead ? "1" : "0");


struct AttackResult
{
	int playerId;
	int skillId;
	int monster_instance_id;
	int damage;
	int cur_hp;
	int max_hp;
	bool isDead;
};