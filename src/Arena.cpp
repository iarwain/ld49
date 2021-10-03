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
    MovePlayer(u32Result, _s32X, _s32Y);
    u32TickCount = 0;

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
    // Update player
    Player* poPlayer = GetPlayer(_u32ID);
    CheckPosition(_s32X, _s32Y);
    orxObject_SetParent(poPlayer->GetOrxObject(), poGrid[_s32X + _s32Y * orxF2U(vGridSize.fX)].poTile->GetOrxObject());
    poPlayer->s32X = _s32X;
    poPlayer->s32Y = _s32Y;
    u32TickCount++;
}

orxBOOL Arena::MoveBullet(Bullet &_roBullet)
{
    orxBOOL bResult;
    orxS32  s32X, s32Y;

    poGrid[_roBullet.s32X + _roBullet.s32Y * orxF2U(vGridSize.fX)].u32ID = 0;

    s32X = _roBullet.s32X + orxF2S(_roBullet.vDirection.fX);
    s32Y = _roBullet.s32Y + orxF2S(_roBullet.vDirection.fY);

    // Update player
    if(bResult = CheckPosition(s32X, s32Y))
    {
        orxObject_SetParent(_roBullet.GetOrxObject(), poGrid[s32X + s32Y * orxF2U(vGridSize.fX)].poTile->GetOrxObject());
        _roBullet.s32X = s32X;
        _roBullet.s32Y = s32Y;

        poGrid[s32X + s32Y * orxF2U(vGridSize.fX)].u32ID = _roBullet.u32ID;
    }

    // Done!
    return bResult;
}

void Arena::ShootBullet(orxU32 _u32ID, orxS32 _s32X, orxS32 _s32Y, const orxVECTOR &_rvDirection)
{
    ld49 &roGame = ld49::GetInstance();

    if(CheckPosition(_s32X, _s32Y))
    {
        orxConfig_PushSection("Bullet");
        orxConfig_SetVector("Direction", &_rvDirection);
        orxConfig_PopSection();
        Bullet *poBullet = roGame.CreateObject<Bullet>("Bullet");
        Cell &roCell = poGrid[_s32X + _s32Y * orxF2U(vGridSize.fX)];
        orxObject_SetParent(poBullet->GetOrxObject(), roCell.poTile->GetOrxObject());
        poBullet->u32ID = roCell.u32ID = _u32ID;
        poBullet->s32X = _s32X;
        poBullet->s32Y = _s32Y;
        orxCOLOR stColor;
        poBullet->SetColor(GetPlayer(_u32ID)->GetColor(stColor));
    }
}

void Arena::OnCreate()
{
    ld49 &roGame = ld49::GetInstance();

    // Init variables
    orxConfig_SetBool("IsArena", orxTRUE);
    orxConfig_SetBool("WrapAround", orxTRUE); // For grid size-independent initial placement of players
    orxConfig_ClearValue("PlayerList");
    u32TickCount    = 0;

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
    ld49 &roGame = ld49::GetInstance();

    PushConfigSection();

    // Update wrap around
    orxConfig_SetString("WrapAround", "@Game");

    // Should tick?
    if(u32TickCount)
    {
        // Update bullets (can't use GetNextObject<Bullet> as it triggers a compiler bug in a different part of the code (!) with MSVS2019)
        for(ScrollObject *poObject = roGame.GetNextObject();
            poObject;
            poObject = roGame.GetNextObject(poObject))
        {
            poObject->PushConfigSection();
            if(orxConfig_GetBool("IsBullet"))
            {
                if(!MoveBullet(*ScrollCast<Bullet *>(poObject)))
                {
                    poObject->SetLifeTime(orxFLOAT_0);
                }
            }
            poObject->PopConfigSection();
        }

        //! TODO: Check collisions

        u32TickCount = 0;
    }

    if(u32TickCount)
    {
        u32TickCount = 0;
    }

    PopConfigSection();
}
