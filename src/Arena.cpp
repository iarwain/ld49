/**
 * @file Arena.cpp
 * @date 2-Oct-2021
 */

#include "Arena.h"

void Arena::OnCreate()
{
    // Init variables
    orxConfig_SetBool("IsArena", orxTRUE);

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
