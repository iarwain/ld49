/**
 * @file Player.cpp
 * @date 2-Oct-2021
 */

#include "Player.h"
#include "Arena.h"

void Player::Die()
{
    if(!bDead)
    {
        bDead = orxTRUE;
        orxCOLOR stColor;
        GetColor(stColor);
        stColor.fAlpha *= 0.3f;
        SetColor(stColor);
    }
}

void Player::IncreaseEnergy()
{
    if(!bDead)
    {
        u32Energy = orxMIN(u32MaxEnergy, u32Energy + 1);
    }
}

void Player::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsPlayer", orxTRUE);
    s32X        = -1;
    s32Y        = -1;
    u32ID       = orxU32_UNDEFINED;
    bDead       = orxFALSE;
    u32Energy   = u32MaxEnergy = orxConfig_GetU32("Energy");

    // Enable its inputs
    orxInput_EnableSet(orxConfig_GetString("Input"), orxTRUE);

    // Register with arena
    orxVECTOR vPos;
    orxConfig_GetVector("InitPos", &vPos);
    u32ID = roGame.GetNextObject<Arena>()->RegisterPlayer(*this, orxF2S(vPos.fX), orxF2S(vPos.fY));
}

void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        ld49 &roGame = ld49::GetInstance();

        Arena *poArena = roGame.GetNextObject<Arena>();

        // Push config section
        PushConfigSection();

        // Select input set
        const orxSTRING zSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetString("Input"));

        // Move
        const orxSTRING azMoveList[4] =
        {
            "MoveRight", "MoveLeft", "MoveDown", "MoveUp"
        };
        orxVECTOR vMove =
        {
            (orxInput_HasBeenActivated("MoveRight") ? orxFLOAT_1 : orxFLOAT_0) - (orxInput_HasBeenActivated("MoveLeft") ? orxFLOAT_1 : orxFLOAT_0),
            (orxInput_HasBeenActivated("MoveDown") ? orxFLOAT_1 : orxFLOAT_0) - (orxInput_HasBeenActivated("MoveUp") ? orxFLOAT_1 : orxFLOAT_0), orxFLOAT_0
        };
        if(!orxVector_IsNull(&vMove))
        {
            poArena->MovePlayer(u32ID, s32X + orxF2S(vMove.fX), s32Y + orxF2S(vMove.fY));
        }

        // Attack
        orxVECTOR vDirection;
        orxBOOL bUnstable = orxConfig_GetBool("Unstable");
        if((u32Energy > 0) && orxInput_HasBeenActivated("AttackLeft"))
        {
            poArena->ShootBullet(u32ID, s32X - 1, s32Y - 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 3, &vDirection));
            poArena->ShootBullet(u32ID, s32X - 1, s32Y, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 4, &vDirection));
            poArena->ShootBullet(u32ID, s32X - 1, s32Y + 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 5, &vDirection));
            u32Energy--;
        }
        if((u32Energy > 0) && orxInput_HasBeenActivated("AttackRight"))
        {
            poArena->ShootBullet(u32ID, s32X + 1, s32Y - 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 1, &vDirection));
            poArena->ShootBullet(u32ID, s32X + 1, s32Y, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 0, &vDirection));
            poArena->ShootBullet(u32ID, s32X + 1, s32Y + 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 7, &vDirection));
            u32Energy--;
        }
        if((u32Energy > 0) && orxInput_HasBeenActivated("AttackUp"))
        {
            poArena->ShootBullet(u32ID, s32X - 1, s32Y - 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 3, &vDirection));
            poArena->ShootBullet(u32ID, s32X, s32Y - 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 2, &vDirection));
            poArena->ShootBullet(u32ID, s32X + 1, s32Y - 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 1, &vDirection));
            u32Energy--;
        }
        if((u32Energy > 0) && orxInput_HasBeenActivated("AttackDown"))
        {
            poArena->ShootBullet(u32ID, s32X - 1, s32Y + 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 5, &vDirection));
            poArena->ShootBullet(u32ID, s32X, s32Y + 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 6, &vDirection));
            poArena->ShootBullet(u32ID, s32X + 1, s32Y + 1, bUnstable ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 7, &vDirection));
            u32Energy--;
        }

        // Deselect input set
        orxInput_SelectSet(zSet);

        // Pop config section
        PopConfigSection();
    }
}
