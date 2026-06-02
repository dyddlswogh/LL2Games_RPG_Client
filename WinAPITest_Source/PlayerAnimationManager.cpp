#include "PlayerAnimationManager.h"
#include "EquipeTypes.h"
#include "playerInfo.h"
#include "stbResourceManager.h"
#include "Util.h"
#include <fstream>

#define PLAYER_ANIM_PATH "WinAPITest_Source/Data/Character/"

#define M_RESOURCEMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
namespace fs = std::filesystem;


bool PlayerAnimationManager::Init()
{
    if (!PreLoadAll()) return false;

    return true;
}

bool PlayerAnimationManager::PreLoadAll()
{
    for (const auto& entry : fs::recursive_directory_iterator(PLAYER_ANIM_PATH))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != ".json")
            continue;

        PlayerAnimationSet animSet{};

        if (!LoadJsonFile(entry.path().string(), animSet))
            return false;

        m_animationSets.emplace(animSet.key, animSet);
    }

    return true;
}

bool PlayerAnimationManager::LoadJsonFile(const std::string& path, PlayerAnimationSet& animData)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json j;
    try {
        file >> j;
    }
    catch (const nlohmann::json::parse_error&) {
        return false;
    }
    if (j.is_null()) return false;

    animData.key.jobType = PlayerTypeUtil::StringToJobType(j.at("job").get<std::string>());
    animData.key.weaponType = Weapon::StringToWeaponType(j.value("weapon", "Unknown"));
  
    DebugMsg = "jobType : " + j.at("job").get<std::string>() + "weapon : " + j.value("weapon", "Unknown") + "\n";
    OutputDebugStringA(DebugMsg.c_str());
    const auto& anims = j.at("animations");

    // get은 필수 값 없어서는 안되는 값
    // value는 없어도 기본값으로 동작 가능할 때 사용
    for (auto& [animName, animJson] : anims.items())
    {
        PlayerAnimationInfo info;

        info.animName = animName;
        info.path = animJson.at("path").get<std::string>();
        info.framePrefix = animJson.at("frame_prefix").get<std::string>();
        info.frameCount = animJson.value("frame_count", 1);
        info.delayMs = animJson.value("delay_ms", 0.1f);

        if (animJson.contains("frame_offsets"))
        {
            for (const auto& offsetJson : animJson["frame_offsets"])
            {
                stb::math::Vector2 frameOffset;
                frameOffset.x = offsetJson.value("x", 0.0f);
                frameOffset.y = offsetJson.value("y", 0.0f);

                info.frameOffsets.push_back(frameOffset);
            }
        }

        animData.animations.emplace_back(info);
    }

    const auto& render = j.at("render");
    animData.renderInfo.width = render.value("width", 0);
    animData.renderInfo.height = render.value("height", 0);

    const auto& origin = render.at("origin");
    animData.renderInfo.origin.x = origin.value("x", 0.0f);
    animData.renderInfo.origin.y = origin.value("y", 0.0f);

    return true;
}

bool PlayerAnimationManager::SetupPlayerAnimations(stb::Player* player, JobType jobtype, WeaponType weaponType)
{
    if (player == nullptr)
        return false;

    stb::Animator* animator = player->GetComponent<stb::Animator>();
    if (animator == nullptr)
        animator = player->AddComponent<stb::Animator>();

    const PlayerAnimationSet* animSet = FindAnimationSet(jobtype, weaponType);
    if (animSet == nullptr)
        return false;

    for (const PlayerAnimationInfo& info : animSet->animations)
    {
        std::vector<stb::Texture*> frames;
        for (int i = 0; i < info.frameCount; ++i)
        {
            std::wstring key =
                utils::StringToWString(info.path + "/" + info.framePrefix + std::to_string(i));

            std::wstring DebugMsg = key + L"\n";
            OutputDebugStringW(DebugMsg.c_str());

            stb::Texture* tex = M_RESOURCEMANAGER->Find<stb::Texture>(key);
            if (tex != nullptr)
                frames.emplace_back(tex);
        }
        std::string DebugMsg = info.animName + "\n";
        OutputDebugStringA(DebugMsg.c_str());

        DebugMsg = "frame size :" + std::to_string(frames.size()) + "\n";
        OutputDebugStringA(DebugMsg.c_str());

        animator->CreateFrameAnimation(
            utils::StringToWString(info.animName),
            frames,
            animSet->renderInfo.origin,
            info.frameOffsets,
            info.delayMs
        );

        stb::Animator::EventNames eventNames;

        eventNames.startEventName = utils::StringToWString(info.animationEvent.start);
        eventNames.completeEventName = utils::StringToWString(info.animationEvent.complete);
        eventNames.endEventName = utils::StringToWString(info.animationEvent.end);

        animator->SetAnimationEventNames(utils::StringToWString(info.animName), eventNames);
    }

    return true;
}

const PlayerAnimationSet* PlayerAnimationManager::FindAnimationSet(JobType jobtype, WeaponType weaponType) const
{
    PlayerAnimationKey key{};

    key.jobType = jobtype;
    key.weaponType = weaponType;
    auto it = m_animationSets.find(key);

    if (it == m_animationSets.end())
    {
        std::string temp;
        temp = "Can't find jobType : " + std::to_string(static_cast<int>(jobtype)) + " WeaponType : " + std::to_string(static_cast<int>(weaponType)) + "\n";
        OutputDebugStringA(temp.c_str());
        return nullptr;
    }

    return &(it->second);
}
