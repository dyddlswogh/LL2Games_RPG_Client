#include "ItemDataManager.h"
#include <fstream>

#define ITEM_PATH "WinAPITest_Source/Data/Items/"
namespace fs = std::filesystem;


bool ItemDataManager::PreLoadAll()
{
    for (const auto& entry : fs::recursive_directory_iterator(ITEM_PATH))
    {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;

        // 파일명에서 id 추출 (예: 2000000.json)
        int item_id = 0;
        try {
            item_id = std::stoi(entry.path().stem().string());
        }
        catch (...) {
            continue;
        }

        if (m_itemDatas.find(item_id) != m_itemDatas.end())
            continue;

        ItemData* itemData{};
        if (!LoadJsonFile(entry.path().string(), *itemData))
        {
            delete itemData;
            return false;
        }
        m_itemDatas.emplace(item_id, itemData);
    }
  
    return true;
   
}

bool ItemDataManager::LoadJsonFile(const std::string& path, ItemData& itemData)
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

    itemData.itemId = j.at("item_id").get<int>();
    itemData.name = j.at("name").get<std::string>();
    itemData.type = Item::SetItemType(j.at("type").get<std::string>());
    itemData.stackable = j.at("stackable").get<bool>();
    itemData.maxStack = j.at("max_stack").get<int>();

    const auto& ue = j.at("tooltip").at(0);
    
    itemData.hpRestore = ue.at("hp_restore").get<int>();
    itemData.mpRestore = ue.at("mp_restore").get<int>();
   
    return true;
}

const ItemData* ItemDataManager::FindItemData(int itemId) const
{
    auto itemData = m_itemDatas.find(itemId);
    if (itemData == m_itemDatas.end())
    {
        return nullptr;
    }
    return itemData->second;
}
