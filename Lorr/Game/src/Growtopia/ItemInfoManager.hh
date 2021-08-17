//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Core/Stream/BufferStream.hh"

struct ItemInfo
{
    uint8_t Type;
    uint8_t Material;
    uint8_t VisualEffect;
    uint8_t Storage;
    uint8_t PosX;
    uint8_t PosY;
    uint8_t Layer;
    uint8_t CollisionType;
    uint8_t HitsToDestroy;
    uint8_t BodyPart;
    uint8_t MaxCount;
    uint8_t SeedBase;
    uint8_t SeedOverlay;
    uint8_t TreeBase;
    uint8_t TreeLeaves;

    uint16_t Flags;
    uint16_t Rarity;

    uint32_t ID;
    uint32_t ExtraStringHash;
    uint32_t AnimMsOrLeashID;
    uint32_t TextureHash;
    uint32_t Cook;
    uint32_t HealTime;
    uint32_t SeedColor;
    uint32_t OverlayColor;
    uint32_t ReadyIn;
    uint32_t AnimFlags;
    uint32_t Flags2;
    uint32_t Range;
    uint32_t MaxStorage;
    uint64_t OverlayObject;

    std::string Name;
    std::string Texture;
    std::string ExtraString;
    std::string BattleName;
    std::string BattlePrefix;
    std::string BattleSuffix;
    std::string BattleAbility;
    std::string IdleAnim;
    std::string ExtraTexture;
    std::string ActiveAnim;
    std::string CustomPunchInfo;

    bool Pack( uint32_t iItemDBVersion, Lorr::BufferStream &buffer );
    bool Unpack( uint32_t iItemDBVersion, Lorr::BufferStream &buffer );
};

class ItemInfoManager
{
public:
    ItemInfoManager(){};
    ~ItemInfoManager(){};

    ItemInfo *GetItem( uint16_t uID );

    // Network stuff
    bool Pack( Lorr::BufferStream &buffer );
    bool Unpack( Lorr::BufferStream &buffer );

    std::vector<ItemInfo> m_vItems;

private:
    uint32_t m_iVersion = 0;
};