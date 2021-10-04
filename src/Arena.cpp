/**
 * @file Arena.cpp
 * @date 2-Oct-2021
 */

#include "Arena.h"
#include "Bullet.h"

orxBOOL Arena::CheckPosition(orxS32 &_rs32X, orxS32 &_rs32Y) const
{
    orxBOOL bResult = orxTRUE;

    PushConfigSection();

    // Wrap?
    if(orxConfig_GetBool("WrapAround"))
    {
        _rs32X  = (_rs32X + orxF2S(vGridSize.fX)) % orxF2S(vGridSize.fX);
        _rs32Y  = (_rs32Y + orxF2S(vGridSize.fY)) % orxF2S(vGridSize.fY);
    }
    else
    {
        orxS32 s32X, s32Y;

        s32X    = orxCLAMP(_rs32X, 0, orxF2S(vGridSize.fX) - 1);
        s32Y    = orxCLAMP(_rs32Y, 0, orxF2S(vGridSize.fY) - 1);
        bResult = (s32X != _rs32X) || (s32Y != _rs32Y) ? orxFALSE : orxTRUE;
        _rs32X  = s32X;
        _rs32Y  = s32Y;
    }

    PopConfigSection();

    // Done!
    return bResult;
}

orxU32 Arena::RegisterPlayer(Player &_roPlayer, orxS32 _s32X, orxS32 _s32Y)
{
    orxU32 u32Result;

    PushConfigSection();

    orxCHAR acBuffer[32];
    const orxSTRING zBuffer = acBuffer;
    orxString_NPrint(acBuffer, sizeof(acBuffer) - 1, "0x%016llX", _roPlayer.GetGUID());
    orxConfig_AppendListString("PlayerList", &zBuffer, 1);
    u32Result = orxConfig_GetListCount("PlayerList");
    orxObject_SetParent(_roPlayer.GetOrxObject(), GetOrxObject());
    orxObject_SetOwner(_roPlayer.GetOrxObject(), GetOrxObject());
    MovePlayer(u32Result, _s32X, _s32Y);
    u32TickCount = 0;

    PopConfigSection();

    return u32Result;
}

Player *Arena::GetPlayer(orxU32 _u32ID) const
{
    PushConfigSection();

    auto poResult = ld49::GetInstance().GetObject<Player>((_u32ID != orxU32_UNDEFINED) ? orxConfig_GetListU64("PlayerList", _u32ID - 1) : orxConfig_GetU64("PlayerList"));

    PopConfigSection();

    return poResult;
}

void Arena::MovePlayer(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y)
{
    // Update player
    Player* poPlayer = GetPlayer(_u32ID);
    if((poPlayer->s32X >= 0) && (poPlayer->s32Y >= 0))
    {
        orxASSERT(poGrid[poPlayer->s32X + poPlayer->s32Y * orxF2U(vGridSize.fX)].u32Count > 0);
        poGrid[poPlayer->s32X + poPlayer->s32Y * orxF2U(vGridSize.fX)].u32Count--;
    }
    CheckPosition(_s32X, _s32Y);
    orxObject_SetParent(poPlayer->GetOrxObject(), poGrid[_s32X + _s32Y * orxF2U(vGridSize.fX)].poTile->GetOrxObject());
    poGrid[_s32X + _s32Y * orxF2U(vGridSize.fX)].u32Count++;
    poPlayer->s32X = _s32X;
    poPlayer->s32Y = _s32Y;
    u32TickCount++;
}

orxBOOL Arena::MoveBullet(Bullet &_roBullet)
{
    orxBOOL bResult = orxTRUE;

    if(!_roBullet.bDead)
    {
        orxS32 s32X, s32Y, s32OldX, s32OldY;

        orxASSERT(poGrid[_roBullet.s32X + _roBullet.s32Y * orxF2U(vGridSize.fX)].u32Count > 0);
        poGrid[_roBullet.s32X + _roBullet.s32Y * orxF2U(vGridSize.fX)].u32Count--;

        s32OldX = _roBullet.s32X;
        s32OldY = _roBullet.s32Y;
        s32X = _roBullet.s32X = _roBullet.s32X + orxF2S(_roBullet.vDirection.fX);
        s32Y = _roBullet.s32Y = _roBullet.s32Y + orxF2S(_roBullet.vDirection.fY);

        // Can move?
        if(CheckPosition(_roBullet.s32X, _roBullet.s32Y))
        {
            orxObject_SetParent(_roBullet.GetOrxObject(), poGrid[_roBullet.s32X + _roBullet.s32Y * orxF2U(vGridSize.fX)].poTile->GetOrxObject());
        }
        else
        {
            orxVECTOR vDirection;
            vDirection.fX = (s32X < 0) ? orxFLOAT_1 : (s32X == orxF2S(vGridSize.fX)) ? -orxFLOAT_1 : _roBullet.vDirection.fX;
            vDirection.fY = (s32Y < 0) ? orxFLOAT_1 : (s32Y == orxF2S(vGridSize.fY)) ? -orxFLOAT_1 : _roBullet.vDirection.fY;
            vDirection.fZ = orxFLOAT_0;
            _roBullet.SetDirection(vDirection);
            _roBullet.s32X = s32OldX;
            _roBullet.s32Y = s32OldY;
        }

        poGrid[_roBullet.s32X + _roBullet.s32Y * orxF2U(vGridSize.fX)].u32Count++;
    }

    // Done!
    return bResult;
}

void Arena::ShootBullet(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y, const orxVECTOR &_rvDirection)
{
    ld49 &roGame = ld49::GetInstance();

    if(CheckPosition(_s32X, _s32Y))
    {
        Cell &roCell = poGrid[_s32X + _s32Y * orxF2U(vGridSize.fX)];
        orxConfig_PushSection("Bullet");
        orxConfig_SetVector("Direction", &_rvDirection);
        orxConfig_PopSection();
        Bullet *poBullet = roGame.CreateObject<Bullet>("Bullet");
        orxObject_SetParent(poBullet->GetOrxObject(), roCell.poTile->GetOrxObject());
        orxObject_SetOwner(poBullet->GetOrxObject(), GetOrxObject());
        poBullet->u64ArenaID = GetGUID();
        poBullet->bUnstable = GetPlayer(_u32ID)->bUnstable;
        roCell.u32Count++;
        poBullet->u32ID = _u32ID;
        poBullet->s32X = _s32X;
        poBullet->s32Y = _s32Y;
        orxCOLOR stColor;
        poBullet->SetColor(GetPlayer(_u32ID)->GetColor(stColor), orxFALSE);
    }
}

void Arena::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsArena", orxTRUE);
    orxConfig_SetBool("WrapAround", orxTRUE); // For grid size-independent initial placement of players
    orxConfig_ClearValue("PlayerList");
    fTickTime       = orxConfig_GetFloat("TickTime");
    u32TickSize     = orxConfig_GetU32("TickSize");
    u32TickCount    = 0;
    bIsGameOver     = orxFALSE;
    bIsTutorial     = orxConfig_GetBool("IsTutorial");

    // Init game
    orxConfig_PushSection("Game");
    orxConfig_SetBool("IsGameOver", orxFALSE);
    orxConfig_SetU64("Arena", GetGUID());
    orxConfig_PopSection();

    // Init music
    orxOBJECT* pstMusic = orxOBJECT(orxStructure_Get(orxConfig_GetU64("Music")));
    if(pstMusic && !bIsTutorial)
    {
        orxObject_SetLifeTime(pstMusic, orxFLOAT_0);
        orxObject_CreateFromConfig("GameMusic");
    }
    else if(!pstMusic && bIsTutorial)
    {
        orxObject_CreateFromConfig("MenuMusic");
    }

    // Init Grid
    orxVECTOR vCurrentPos, vPos, vScale;
    orxConfig_GetVector("GridSize", &vGridSize);
    orxConfig_GetVector("TileSize", &vTileSize);

    poGrid = (Cell *)orxMemory_Allocate(sizeof(Cell) * orxF2U(vGridSize.fX) * orxF2U(vGridSize.fY), orxMEMORY_TYPE_MAIN);
    orxMemory_Zero(poGrid, sizeof(Cell) * orxF2U(vGridSize.fX) * orxF2U(vGridSize.fY));

    orxVector_Add(&vPos, &GetPosition(vCurrentPos), orxVector_Mulf(&vPos, orxVector_Mul(&vPos, orxVector_Mul(&vPos, &vGridSize, &vTileSize), &GetScale(vScale)), -0.5f));
    SetPosition(vPos);

    orxVector_SetAll(&vPos, orxFLOAT_0);
    for(orxU32 j = 0; j < orxF2U(vGridSize.fY); j++)
    {
        for(orxU32 i = 0; i < orxF2U(vGridSize.fX); i++)
        {
            Cell *poCell    = poGrid + (i + (j * orxF2U(vGridSize.fX)));
            poCell->poTile  = roGame.CreateObject("Tile");
            orxObject_SetOwner(poCell->poTile->GetOrxObject(), GetOrxObject());
            poCell->u32Count= 0;
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
    if(!bIsTutorial)
    {
        orxConfig_PushSection("Game");
        orxOBJECT* pstMusic = orxOBJECT(orxStructure_Get(orxConfig_GetU64("Music")));
        if(pstMusic)
        {
            orxObject_SetLifeTime(pstMusic, orxFLOAT_0);
            orxConfig_PushSection("Game");
            orxConfig_SetU64("Music", 0);
            orxConfig_PopSection();
        }
        orxConfig_PopSection();
    }

    orxMemory_Free(poGrid);
}

void Arena::Update(const orxCLOCK_INFO &_rstInfo)
{
    ld49 &roGame = ld49::GetInstance();

    PushConfigSection();

    if(!bIsGameOver)
    {
        // Update status
        orxConfig_SetString("WrapAround", "@Game");
        fTickTime -= _rstInfo.fDT;

        // Should tick?
        if((fTickTime <= orxFLOAT_0) || ((fTickTime == -orxFLOAT_1) && (u32TickCount >= u32TickSize)))
        {
            // Update bullets (can't use GetNextObject<Bullet> as it triggers a compiler bug in a different part of the code (!) with MSVS2019)
            for(ScrollObject *poObject = roGame.GetNextObject();
                poObject;
                poObject = roGame.GetNextObject(poObject))
            {
                poObject->PushConfigSection();
                if(orxConfig_GetBool("IsBullet"))
                {
                    Bullet *poBullet = ScrollCast<Bullet *>(poObject);
                    if((poBullet->u64ArenaID == GetGUID()) && poBullet->bActive)
                    {
                        MoveBullet(*poBullet);
                    }
                    else
                    {
                        orxCOLOR stColor;
                        poBullet->bActive = orxTRUE;
                        poBullet->SetColor(poBullet->GetColor(stColor));
                    }
                }
                poObject->PopConfigSection();
            }

            // Replenish all players' energy
            for(orxS32 i = 0, iCount = orxConfig_GetListCount("PlayerList"); i < iCount; i++)
            {
                Player *poPlayer = ld49::GetInstance().GetObject<Player>(orxConfig_GetListU64("PlayerList", i));
                poPlayer->IncreaseEnergy();
            }

            u32TickCount    = 0;
            fTickTime      += orxConfig_GetFloat("TickTime");
        }

        // For all cells
        for(orxU32 i = 0, iCount = orxF2U(vGridSize.fX) * orxF2U(vGridSize.fY); i < iCount; i++)
        {
            // Collision?
            Cell& roCell = poGrid[i];
            if(roCell.u32Count > 1)
            {
                // For all children
                for(orxOBJECT *pstObject = orxObject_GetChild(roCell.poTile->GetOrxObject());
                    pstObject;
                    pstObject = orxObject_GetSibling(pstObject))
                {
                    Object *poObject = (Object *)orxObject_GetUserData(pstObject);
                    if(poObject)
                    {
                        poObject->Die();
                    }
                }
                roCell.u32Count = 0;
            }
        }

        // Not tutorial?
        if(!bIsTutorial)
        {
            // For all players
            orxU32 u32AliveCount = 0;
            Player *poWinner = orxNULL;
            for(orxS32 i = 0, iCount = orxConfig_GetListCount("PlayerList"); i < iCount; i++)
            {
                Player *poPlayer = ld49::GetInstance().GetObject<Player>(orxConfig_GetListU64("PlayerList", i));
                if(!poPlayer->bDead)
                {
                    u32AliveCount++;
                    poWinner = poPlayer;
                }
            }
            if(((u32AliveCount == 1) && (orxConfig_GetListCount("PlayerList") > 1))
            || (u32AliveCount == 0))
            {
                orxCHAR acName[64];
                bIsGameOver = orxTRUE;
                orxString_NPrint(acName, sizeof(acName) - 1, "%s", poWinner ? poWinner->GetModelName() : "NO ONE");
                orxConfig_SetString("Winner", acName);
                orxString_UpperCase(acName);
                orxConfig_SetString("WINNER", acName);
                orxObject_SetOwner(roGame.CreateObject("GameOver")->GetOrxObject(), GetOrxObject());
            }
        }
    }
    else
    {
        if(orxInput_HasBeenActivated("Restart"))
        {
            orxInput_SetValue("Reset", orxFLOAT_1);
        }
    }

    PopConfigSection();
}
