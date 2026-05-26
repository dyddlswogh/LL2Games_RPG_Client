#include "CombatPacketHandler.h"
#include "PacketParser.h"
#include "stbNetworkManager.h"
#include "CombatSystem_Info.h"
#include "MonsterManager.h"
#include "PlayerManager.h"

#define M_MONSTERMANAGER stb::SingletonBase<MonsterManager>::getInstance()
#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()

void CombatPacketHandler::HandlerMosterDamage(const ParsedPacket& pkt)
{
}

void CombatPacketHandler::HandleAttackResult(const ParsedPacket& pkt)
{
    /*
        payload.push_back(std::to_string(Attacker->GetId()));
	    payload.push_back(std::to_string(SkillID));
	    payload.push_back(std::to_string(result.size()));
        payload.push_back(std::to_string(r.monster_instance_id));
        payload.push_back(std::to_string(r.damage));
        payload.push_back(std::to_string(r.cur_hp));
        payload.push_back(std::to_string(r.max_hp));
        payload.push_back(r.dead ? "1" : "0");

        struct MonsterHitResult {
        int monster_instance_id;
        int damage;
        int cur_hp;
        int max_hp;
        bool dead;
};

    */

    try
    {
        size_t offset = 0;
        const char* data = pkt.payload.c_str();
        size_t payloadSize = pkt.payload.size();
        std::string errMsg;

        int monster_size = 0;
        int isDead = 0;

        AttackResult attackResult{};


        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, monster_size, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.playerId, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.skillId, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        for (size_t i = 0; i < monster_size; i++)
        {
          
            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.monster_instance_id, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.damage, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.cur_hp, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, attackResult.max_hp, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, isDead, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            attackResult.isDead = (isDead != 0);


            M_MONSTERMANAGER->ApplyAttackResult(attackResult);
            M_PLAYERMANAGER->PlayAttackAnimation(attackResult.playerId, attackResult.skillId);

        }
    }
    catch (std::exception& e)
    {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }
    catch (...)
    {
        OutputDebugStringA("예상치 못한 에러입니다.\n\n");
    }

}

void CombatPacketHandler::SendBasicAttack(int dir)
{
    std::vector<std::string> data;

    data.push_back(std::to_string(dir));

    //std::string DebugMsg = "Use_Count :" + std::to_string(inventoryitemInfo->useCount) + "\n";

    //OutputDebugStringA(DebugMsg.c_str());

    stb::NetworkManager::getInstance()->SendPacket(PKT_PLAYER_BASIC_ATTACK, data);
    OutputDebugStringA("[PKT_PLAYER_ATTACK 전송 완료]\n\n");
}

void CombatPacketHandler::SendUseSkill(int skillId, int dir)
{
    std::vector<std::string> data;

    data.push_back(std::to_string(skillId));
    data.push_back(std::to_string(dir));

    //std::string DebugMsg = "Use_Count :" + std::to_string(inventoryitemInfo->useCount) + "\n";

    //OutputDebugStringA(DebugMsg.c_str());

    stb::NetworkManager::getInstance()->SendPacket(PKT_PLAYER_ATTACK, data);
    OutputDebugStringA("[PKT_PLAYER_ATTACK 전송 완료]\n\n");

}
