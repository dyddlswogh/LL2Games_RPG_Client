#pragma once

struct AttackResult
{
    int playerId;
    int skillId;
    int monster_instance_id;
    int damage;
    int cur_hp;
    int max_hp;
    int isDead;
};