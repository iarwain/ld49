/**
 * @file Player.cpp
 * @date 2-Oct-2021
 */

#include "Player.h"
#include "Arena.h"

void Player::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsPlayer", orxTRUE);

    // Enable its inputs
    orxInput_EnableSet(orxConfig_GetString("Input"), orxTRUE);

    // Register with arena
    u32ID = roGame.GetNextObject<Arena>()->RegisterPlayer(*this);
}


void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Push config section
    PushConfigSection();

    if(!orxConfig_GetBool("IsGameOver"))
    {
        const orxSTRING azMoveList[4] =
        {
            "MoveRight", "MoveLeft", "MoveDown", "MoveUp"
        };

        // Select input set
        const orxSTRING zSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetString("Input"));

        //! TODO: Logic goes here

        // Deselect input set
        orxInput_SelectSet(zSet);

        // Pop config section
        PopConfigSection();
    }
}
