/**
 * @file Player.cpp
 * @date 2-Oct-2021
 */

#include "Player.h"
#include "Arena.h"

void orxFASTCALL Attract(const orxCLOCK_INFO *_pstClockInfo, void *_pContext)
{
    Player *poPlayer = (Player *)_pContext;

    if(!poPlayer->bDead)
    {
        poPlayer->PushConfigSection();
        const orxSTRING zSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetString("Input"));
        orxFLOAT fDelay = orxConfig_GetFloat("AttractDelay");
        if(orxMath_GetRandomFloat(orxFLOAT_0, orxFLOAT_1) <= orxConfig_GetFloat("AttackChance"))
        {
            orxInput_SetValue(orxConfig_GetString("AttackList"), orxFLOAT_1);
            fDelay += orxConfig_GetFloat("AttackDelay");
        }
        else
        {
            orxInput_SetValue(orxConfig_GetString("MoveList"), orxFLOAT_1);
        }
        orxClock_AddGlobalTimer(Attract, fDelay, 1, _pContext);
        orxInput_SelectSet(zSet);
        poPlayer->PopConfigSection();
    }
}

void Player::Die()
{
    if(!bDead)
    {
        Object::Die();
        bDead   = orxTRUE;
        fEnergy = fMaxEnergy;
        IncreaseEnergy();
        SetAnim("Die");
        orxCOLOR stColor;
        GetColor(stColor);
        stColor.fAlpha *= 0.75f;
        SetColor(stColor);
    }
}

void Player::IncreaseEnergy()
{
    if(!bDead)
    {
        PushConfigSection();
        if(orxConfig_GetBool("IsUnstable"))
        {
            fEnergy = orxMIN(orxFLOAT_0, fEnergy + fEnergyRate);
        }
        else
        {
            fEnergy = orxMIN(fMaxEnergy, fEnergy + fEnergyRate);
        }
        PopConfigSection();
    }
}

void Player::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    Object::OnCreate();
    orxConfig_SetBool("IsPlayer", orxTRUE);
    s32X        = -1;
    s32Y        = -1;
    u32ID       = orxU32_UNDEFINED;
    bIsAttract  = orxConfig_GetBool("IsAttract");
    fEnergy     = fMaxEnergy = orxConfig_GetFloat("Energy");
    fEnergyRate = orxConfig_GetFloat("EnergyRate");
    IncreaseEnergy();

    // Register with arena
    Arena *poArena = roGame.GetObject<Arena>(orxConfig_GetU64("Arena"));
    if(poArena)
    {
        orxVECTOR vPos;
        orxConfig_GetVector("InitPos", &vPos);
        u32ID       = poArena->RegisterPlayer(*this, orxF2S(vPos.fX), orxF2S(vPos.fY));
        u64ArenaID  = poArena->GetGUID();

        if(bIsAttract)
        {
            orxClock_AddGlobalTimer(Attract, orxConfig_GetFloat("AttractDelay"), 1, this);
        }
    }
    else
    {
        SetLifeTime(orxFLOAT_0);
    }
}

void Player::OnDelete()
{
    // Remove attract mode
    orxClock_RemoveGlobalTimer(Attract, -orxFLOAT_1, this);
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        ld49 &roGame = ld49::GetInstance();

        Arena *poArena = roGame.GetObject<Arena>(u64ArenaID);

        if(poArena && !poArena->bIsGameOver)
        {
            // Push config section
            PushConfigSection();

            // Select input set
            const orxSTRING zSet = orxInput_GetCurrentSet();
            orxInput_SelectSet(orxConfig_GetString("Input"));

            // Move
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
            orxFLOAT  fAttack   = (((fEnergy < orxFLOAT_1) && (fEnergy >= -fMaxEnergy + orxFLOAT_1))
                                && (orxInput_HasBeenActivated("AttackLeft") || orxInput_HasBeenActivated("AttackRight") || orxInput_HasBeenActivated("AttackUp") || orxInput_HasBeenActivated("AttackDown")))
                                  ? orxMath_GetRandomFloat(orxFLOAT_0, orxFLOAT_1)
                                  : 2.0f;
            orxBOOL   bTargeted = orxFALSE;
            orxS32    s32BulletX= s32X, s32BulletY = s32Y, s32Distance = 1;
            if(fAttack <= orxFLOAT_1)
            {
                Player *poPlayer    = poArena->GetPlayer();
                if(poPlayer == this)
                {
                    poPlayer = poArena->GetPlayer();
                }
                if(poPlayer != this)
                {
                    s32Distance = orxMath_GetRandomS32(2, 3);
                    bTargeted   = orxTRUE;
                    fAttack     = 2.0f;
                }
                s32BulletX = poPlayer->s32X;
                s32BulletY = poPlayer->s32Y;
            }
            if((((fEnergy >= orxFLOAT_1) || bTargeted) && orxInput_HasBeenActivated("AttackLeft")) || (fAttack <= 0.25f))
            {
                poArena->ShootBullet(u32ID, s32BulletX - s32Distance, s32BulletY - s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 3, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX - s32Distance, s32BulletY, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 4, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX - s32Distance, s32BulletY + s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 5, &vDirection));
                fEnergy -= orxFLOAT_1;
            }
            else if((((fEnergy >= orxFLOAT_1) || bTargeted) && orxInput_HasBeenActivated("AttackRight")) || (fAttack <= 0.5f))
            {
                poArena->ShootBullet(u32ID, s32BulletX + s32Distance, s32BulletY - s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 1, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX + s32Distance, s32BulletY, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 0, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX + s32Distance, s32BulletY + s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 7, &vDirection));
                fEnergy -= orxFLOAT_1;
            }
            else if((((fEnergy >= orxFLOAT_1) || bTargeted) && orxInput_HasBeenActivated("AttackUp")) || (fAttack <= 0.75f))
            {
                poArena->ShootBullet(u32ID, s32BulletX - s32Distance, s32BulletY - s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 3, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX, s32BulletY - s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 2, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX + s32Distance, s32BulletY - s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 1, &vDirection));
                fEnergy -= orxFLOAT_1;
            }
            else if((((fEnergy >= orxFLOAT_1) || bTargeted) && orxInput_HasBeenActivated("AttackDown")) || (fAttack <= 1.0f))
            {
                poArena->ShootBullet(u32ID, s32BulletX - s32Distance, s32BulletY + s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 5, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX, s32BulletY + s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 6, &vDirection));
                poArena->ShootBullet(u32ID, s32BulletX + s32Distance, s32BulletY + s32Distance, (fEnergy < orxFLOAT_1) ? *orxConfig_GetVector("Direction", &vDirection) : *orxConfig_GetListVector("Direction", 7, &vDirection));
                fEnergy -= orxFLOAT_1;
            }

            // Set anim (energy indicator)
            if(fEnergy == fMaxEnergy)
            {
                SetAnim("100%");
            }
            else if(fEnergy >= 0.5f * fMaxEnergy)
            {
                SetAnim("75%");
            }
            else if (fEnergy >= orxFLOAT_0)
            {
                SetAnim("50%");
            }
            else if (fEnergy >= -0.5f * fMaxEnergy)
            {
                SetAnim("25%");
            }
            else
            {
                SetAnim("0%");
            }

            // Update status
            bUnstable = (fEnergy < orxFLOAT_1) ? orxTRUE : orxFALSE;

            // Deselect input set
            orxInput_SelectSet(zSet);

            // Pop config section
            PopConfigSection();
        }
    }
}
