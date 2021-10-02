/**
 * @file Arena.cpp
 * @date 2-Oct-2021
 */

#include "Arena.h"

orxU32 Arena::RegisterPlayer(Player &_roPlayer)
{
    orxU32 u32Result;

    PushConfigSection();

    orxCHAR acBuffer[32];
    const orxSTRING zBuffer = acBuffer;
    orxString_NPrint(acBuffer, sizeof(acBuffer) - 1, "0x%016llX", _roPlayer.GetGUID());
    orxConfig_AppendListString("PlayerList", &zBuffer, 1);
    u32Result = orxConfig_GetListCount("PlayerList");

    orxObject_SetParent(_roPlayer.GetOrxObject(), GetOrxObject());

    PopConfigSection();

    return u32Result;
}

Player *Arena::GetPlayer(orxU32 _u32ID) const
{
    PushConfigSection();

    auto poResult = ld49::GetInstance().GetObject<Player>(orxConfig_GetListU64("PlayerList", _u32ID - 1));

    PopConfigSection();

    return poResult;
}

void Arena::MovePlayer(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y)
{
    orxU32 x, y;

    // Wrap
    x = (_s32X + orxF2S(vGridSize.fX)) % orxF2S(vGridSize.fX);
    y = (_s32Y + orxF2S(vGridSize.fY)) % orxF2S(vGridSize.fY);

    // Update player
    Player* poPlayer = GetPlayer(_u32ID);
    orxObject_SetParent(poPlayer->GetOrxObject(), poGrid[x + y * orxF2U(vGridSize.fX)].poTile->GetOrxObject());
    poPlayer->s32X = x;
    poPlayer->s32Y = y;
}

void Arena::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsArena", orxTRUE);
    orxConfig_ClearValue("PlayerList");

    // Init game
    orxConfig_PushSection("Game");
    orxConfig_SetBool("IsGameOver", orxFALSE);
    orxConfig_PopSection();

    // Init Grid
    orxVECTOR vPos, vScale;
    orxConfig_GetVector("GridSize", &vGridSize);
    orxConfig_GetVector("TileSize", &vTileSize);

    poGrid = (Cell *)orxMemory_Allocate(sizeof(Cell) * orxF2U(vGridSize.fX) * orxF2U(vGridSize.fY), orxMEMORY_TYPE_MAIN);
    orxMemory_Zero(poGrid, sizeof(Cell) * orxF2U(vGridSize.fX) * orxF2U(vGridSize.fY));

    orxVector_Mulf(&vPos, orxVector_Mul(&vPos, orxVector_Mul(&vPos, &vGridSize, &vTileSize), &GetScale(vScale)), -0.5f);
    SetPosition(vPos);

    orxVector_SetAll(&vPos, orxFLOAT_0);
    for(orxU32 j = 0; j < orxF2U(vGridSize.fY); j++)
    {
        for(orxU32 i = 0; i < orxF2U(vGridSize.fX); i++)
        {
            Cell *poCell    = poGrid + (i + (j * orxF2U(vGridSize.fX)));
            poCell->poTile  = roGame.CreateObject("Tile");
            poCell->u32ID   = 0;
            orxObject_SetParent(poCell->poTile->GetOrxObject(), GetOrxObject());
            poCell->poTile->SetPosition(vPos);
            vPos.fX        += vTileSize.fX;
        }
        vPos.fX             = orxFLOAT_0;
        vPos.fY            += vTileSize.fY;
    }
}

void Arena::OnDelete()
{
    orxMemory_Free(poGrid);
}

void Arena::Update(const orxCLOCK_INFO &_rstInfo)
{
}
