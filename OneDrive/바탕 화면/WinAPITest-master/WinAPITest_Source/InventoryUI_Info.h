#pragma once

enum class InventoryType
{
    Equip,
    Consume,
    Etc,
    Setup,
    Cash,
    Cosmetic,
    EnumEnd
};

enum class InventoryButtonType
{
    Expand,
    Reduce,
    Close,
    EnumEnd
};

enum class UIButtonState
{
    Normal,
    Hover,
    Pressed
};

struct InventorySlotUI
{
    int slotIndex;

    float x;
    float y;
    float width;
    float height;
    bool isEnabled;

    int itemId;
    int itemCount;
};


struct InventoryButton
{
    InventoryButtonType type;
    UIButtonState state;

    RECT size;

    stb::Texture* nomalImg;
    stb::Texture* hoverImg;
    stb::Texture* pressedImg;

    bool isVisble;
    bool isEnable;
};

struct InventoryTabVisual
{
    stb::Texture* normal = nullptr;
    stb::Texture* selected = nullptr;
    RECT rect{};
};

struct InventoryTabButton
{
    InventoryType type{};
    InventoryTabVisual normalView;
    InventoryTabVisual fullView;
};
