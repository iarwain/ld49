/**
 * @file Player.cpp
 * @date 2-Oct-2021
 */

#include "Player.h"
#include "Arena.h"

void Player::Die()
{
    bDead = orxTRUE;
    orxCOLOR stColor;
    GetColor(stColor);
    // orxColor_FromRGBToHSV(&stColor, &stColor);
    // stColor.vHSV.fS *= 0.6f;
    // stColor.vHSV.fV *= 0.3f;
    // orxColor_FromHSVToRGB(&stColor, &stColor);
    stColor.fAlpha *= 0.3f;
    SetColor(stColor);
}

void Player::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsPlayer", orxTRUE);
    s32X    = -1;
    s32Y    = -1;
    u32ID   = orxU32_UNDEFINED;
    bDead   = orxFALSE;

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
        orxConfig_GetVector("Direction", &vDirection);
        if(orxInput_HasBeenActivated("AttackLeft"))
        {
            //orxVECTOR vDirection = {-orxFLOAT_1, orxFLOAT_0, orxFLOAT_0};
            poArena->ShootBullet(u32ID, s32X - 1, s32Y, vDirection);
        }
        if(orxInput_HasBeenActivated("AttackRight"))
        {
            //orxVECTOR vDirection = {orxFLOAT_1, orxFLOAT_0, orxFLOAT_0};
            poArena->ShootBullet(u32ID, s32X + 1, s32Y, vDirection);
        }
        if(orxInput_HasBeenActivated("AttackUp"))
        {
            //orxVECTOR vDirection = {orxFLOAT_0, -orxFLOAT_1, orxFLOAT_0};
            poArena->ShootBullet(u32ID, s32X, s32Y - 1, vDirection);
        }
        if(orxInput_HasBeenActivated("AttackDown"))
        {
            //orxVECTOR vDirection = {orxFLOAT_0, orxFLOAT_1, orxFLOAT_0};
            poArena->ShootBullet(u32ID, s32X, s32Y + 1, vDirection);
        }

        // Deselect input set
        orxInput_SelectSet(zSet);

        // Pop config section
        PopConfigSection();
    }
}
