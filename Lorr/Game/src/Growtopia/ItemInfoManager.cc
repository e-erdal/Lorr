#include "ItemInfoManager.hh"

static void Unscramble( std::string &str, int id, const std::string &key )
{
    ZoneScoped;

    size_t curId = id % key.length();

    for ( size_t i = 0; i < str.length(); i++ )
    {
        str[i] ^= key[curId++];
        if ( curId >= key.length() ) curId = 0;
    }
}

bool ItemInfo::Pack( uint32_t iItemDBVersion, Lorr::BufferStream &buffer )
{
    ZoneScoped;

    buffer.Insert( ID );
    buffer.Insert( Flags );
    buffer.Insert( Type );
    buffer.Insert( Material );

    buffer.Insert<uint16_t>( Name.length() );
    if ( iItemDBVersion > 2 )
    {
        std::string crypted = Name;
        Unscramble( crypted, ID, "PBG892FXX982ABC*" );
        buffer.InsertString( crypted );
    }
    else
        buffer.InsertString( Name );

    buffer.Insert<uint16_t>( Texture.size() );
    buffer.InsertString( Texture );
    buffer.Insert( TextureHash );
    buffer.Insert( VisualEffect );
    buffer.Insert( Cook );
    buffer.Insert( PosX );
    buffer.Insert( PosY );

    buffer.Insert( Storage );
    buffer.Insert( Layer );
    buffer.Insert( CollisionType );
    buffer.Insert( HitsToDestroy );
    buffer.Insert( HealTime );
    buffer.Insert( BodyPart );
    buffer.Insert( Rarity );
    buffer.Insert( MaxCount );

    buffer.Insert<uint16_t>( ExtraString.size() );
    buffer.InsertString( ExtraString );
    buffer.Insert( ExtraStringHash );

    buffer.Insert( AnimMsOrLeashID );
    if ( iItemDBVersion > 3 )
    {
        buffer.Insert<uint16_t>( BattleName.size() );
        buffer.InsertString( BattleName );
        buffer.Insert<uint16_t>( BattlePrefix.size() );
        buffer.InsertString( BattlePrefix );
        buffer.Insert<uint16_t>( BattleSuffix.size() );
        buffer.InsertString( BattleSuffix );
        if ( iItemDBVersion > 4 )
        {
            buffer.Insert<uint16_t>( BattleAbility.size() );
            buffer.InsertString( BattleAbility );
        }
    }

    buffer.Insert( SeedBase );
    buffer.Insert( SeedOverlay );
    buffer.Insert( TreeBase );
    buffer.Insert( TreeLeaves );
    buffer.Insert( SeedColor );
    buffer.Insert( OverlayColor );

    buffer.Insert<int>( 0 );

    buffer.Insert( ReadyIn );

    if ( iItemDBVersion > 6 )
    {
        buffer.Insert( AnimFlags );
        buffer.Insert<uint16_t>( IdleAnim.size() );
        buffer.InsertString( IdleAnim );
    }

    if ( iItemDBVersion > 7 )
    {
        buffer.Insert<uint16_t>( ExtraTexture.size() );
        buffer.InsertString( ExtraTexture );
        buffer.Insert<uint16_t>( ActiveAnim.size() );
        buffer.InsertString( ActiveAnim );
        buffer.Insert( OverlayObject );
    }

    if ( iItemDBVersion > 8 )
    {
        buffer.Insert( Flags2 );
        buffer.Insert<char>( 60 );
    }

    if ( iItemDBVersion > 9 )
    {
        buffer.Insert( Range );
        buffer.Insert( MaxStorage );
    }

    if ( iItemDBVersion > 10 )
    {
        buffer.InsertString( CustomPunchInfo );
    }

    if ( iItemDBVersion > 11 )
    {
        buffer.Insert<char>( 13 );
    }

    if ( iItemDBVersion > 12 )
    {
        buffer.Insert<char>( 4 );
    }

    return true;
}

bool ItemInfo::Unpack( uint32_t iItemDBVersion, Lorr::BufferStream &buffer )
{
    ZoneScoped;

    if ( buffer.GetSize() < 10 )
    {
        return false;
    }

    ID = buffer.Get<uint32_t>();
    Flags = buffer.Get<uint16_t>();
    Type = buffer.Get<uint8_t>();
    Material = buffer.Get<uint8_t>();

    auto nameLen = buffer.Get<uint16_t>();
    Name = buffer.GetString( nameLen );
    if ( iItemDBVersion > 2 )
    {
        Unscramble( Name, ID, "PBG892FXX982ABC*" );
    }

    auto textureLen = buffer.Get<uint16_t>();
    Texture = buffer.GetString( textureLen );

    // // LOAD TEXTURE
    // char *szGTPath = 0;
    // uint32_t GTPathLen = 0;
    // bx::getEnv( szGTPath, &GTPathLen, "LOCALAPPDATA" );
    // // GetEngine()->GetTextureManager().CreateTextureFromFile( "file://" + Texture, 0 );
    // // Console::Log( "Localappdata: {}", szGTPath );

    TextureHash = buffer.Get<uint32_t>();
    VisualEffect = buffer.Get<uint8_t>();
    Cook = buffer.Get<uint32_t>();
    PosX = buffer.Get<uint8_t>();
    PosY = buffer.Get<uint8_t>();

    Storage = buffer.Get<uint8_t>();
    Layer = buffer.Get<uint8_t>();
    CollisionType = buffer.Get<uint8_t>();
    HitsToDestroy = buffer.Get<uint8_t>();
    HealTime = buffer.Get<uint32_t>();
    BodyPart = buffer.Get<uint8_t>();
    Rarity = buffer.Get<uint16_t>();
    MaxCount = buffer.Get<uint8_t>();

    auto extraStringLen = buffer.Get<uint16_t>();
    ExtraString = buffer.GetString( extraStringLen );
    ExtraStringHash = buffer.Get<uint32_t>();

    AnimMsOrLeashID = buffer.Get<uint32_t>();
    if ( iItemDBVersion > 3 )
    {
        auto BattleNameLen = buffer.Get<uint16_t>();
        BattleName = buffer.GetString( BattleNameLen );

        auto BattlePrefixLen = buffer.Get<uint16_t>();
        BattlePrefix = buffer.GetString( BattlePrefixLen );

        auto BattleSuffixLen = buffer.Get<uint16_t>();
        BattleSuffix = buffer.GetString( BattleSuffixLen );

        if ( iItemDBVersion > 4 )
        {
            auto BattleAbilityLen = buffer.Get<uint16_t>();
            BattleAbility = buffer.GetString( BattleAbilityLen );
        }
    }

    SeedBase = buffer.Get<uint8_t>();
    SeedOverlay = buffer.Get<uint8_t>();
    TreeBase = buffer.Get<uint8_t>();
    TreeLeaves = buffer.Get<uint8_t>();
    SeedColor = buffer.Get<uint32_t>();
    OverlayColor = buffer.Get<uint32_t>();

    // actually the 2 ingredients for this item (as 2x shorts) are here,
    // but removed in new items.dat versions, so skipping
    buffer.Get<uint32_t>( 4 );

    ReadyIn = buffer.Get<uint32_t>( 4 );
    if ( iItemDBVersion > 6 )
    {
        AnimFlags = buffer.Get<uint32_t>( 4 );

        auto IdleAnimLen = buffer.Get<uint16_t>( 2 );
        IdleAnim = buffer.GetString( IdleAnimLen );
    }

    if ( iItemDBVersion > 7 )
    {
        auto ExtraTextureLen = buffer.Get<uint16_t>( 2 );
        ExtraTexture = buffer.GetString( ExtraTextureLen );

        auto ActiveAnimLen = buffer.Get<uint16_t>( 2 );
        ActiveAnim = buffer.GetString( ActiveAnimLen );

        OverlayObject = buffer.Get<uint64_t>( 8 );
    }

    if ( iItemDBVersion > 8 )
    {
        Flags2 = buffer.Get<uint32_t>( 4 );
        buffer.Get<char>( 60 );
    }

    if ( iItemDBVersion > 9 )
    {
        Range = buffer.Get<uint32_t>( 4 );
        MaxStorage = buffer.Get<uint32_t>( 4 );
    }

    if ( iItemDBVersion > 10 )
    {
        auto CustomPunchInfoLen = buffer.Get<uint16_t>( 2 );
        CustomPunchInfo = buffer.GetString( CustomPunchInfoLen );
    }

    if ( iItemDBVersion > 11 )
    {
        buffer.Get<char>( 13 );
    }

    if ( iItemDBVersion > 12 )
    {
        buffer.Get<char>( 4 );
    }

    return true;
}

ItemInfo *ItemInfoManager::GetItem( uint16_t uID )
{
    ZoneScoped;

    if ( uID > m_vItems.size() ) return 0;
    if ( uID <= 0 ) return 0;

    return &m_vItems[uID - 1];
}

bool ItemInfoManager::Pack( Lorr::BufferStream &buffer )
{
    ZoneScoped;

    buffer.Insert<uint16_t>( m_iVersion );
    buffer.Insert<uint32_t>( m_vItems.size() );

    for ( auto &item : m_vItems )
    {
        item.Pack( m_iVersion, buffer );
    }

    return true;
}

bool ItemInfoManager::Unpack( Lorr::BufferStream &buffer )
{
    ZoneScoped;

    if ( buffer.GetSize() < 6 )
    {
        return false;
    }

    m_iVersion = buffer.Get<uint16_t>();

    auto itemAmount = buffer.Get<uint32_t>();
    m_vItems.resize( itemAmount );

    for ( auto &item : m_vItems )
    {
        if ( !item.Unpack( m_iVersion, buffer ) ) return false;
    }

    return true;
}