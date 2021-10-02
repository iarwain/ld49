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
    u32Result = orxConfig_GetListCount("PlayerList") - 1;

    orxObject_SetParent(_roPlayer.GetOrxObject(), GetOrxObject());

    PopConfigSection();

    return u32Result;
}

Player *Arena::GetPlayer(orxU32 _u32ID) const
{
    PushConfigSection();

    auto poResult = ld49::GetInstance().GetObject<Player>(orxConfig_GetListU64("PlayerList", _u32ID));

    PopConfigSection();

    return poResult;
}

void Arena::OnCreate()
{
    // Init variables
    orxConfig_SetBool("IsArena", orxTRUE);
    orxConfig_ClearValue("PlayerList");

    // Inits game
    orxConfig_PushSection("Game");
    orxConfig_SetBool("IsGameOver", orxFALSE);
    orxConfig_PopSection();
}

void Arena::OnDelete()
{
}

void Arena::Update(const orxCLOCK_INFO &_rstInfo)
{
}
