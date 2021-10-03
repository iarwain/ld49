/**
 * @file Bullet.cpp
 * @date 2-Oct-2021
 */

#include "Bullet.h"

void Bullet::Die()
{
    if(!bDead)
    {
        Object::Die();
        SetLifeTime(orxFLOAT_0);
        orxObject_SetParent(orxObject_CreateFromConfig("Explosion"), orxObject_GetParent(GetOrxObject()));
    }
}

void Bullet::SetDirection(const orxVECTOR &_rvDirection)
{
    vDirection.fX = (_rvDirection.fX < orxFLOAT_0) ? -orxFLOAT_1 : (_rvDirection.fX > orxFLOAT_0) ? orxFLOAT_1 : orxFLOAT_0;
    vDirection.fY = (_rvDirection.fY < orxFLOAT_0) ? -orxFLOAT_1 : (_rvDirection.fY > orxFLOAT_0) ? orxFLOAT_1 : orxFLOAT_0;
    vDirection.fZ = orxFLOAT_0;
    orxConfig_SetVector("DummyDirection", &vDirection);
    SetAnim(orxConfig_GetString("DummyDirection"), orxTRUE, orxTRUE);
}

void Bullet::OnCreate()
{
    orxConfig_SetBool("IsBullet", orxTRUE);
    orxConfig_GetVector("Direction", &vDirection);
    bActive     = orxFALSE;
    u32ID       = orxU32_UNDEFINED;
    s32X        = s32Y = 0;
    u64ArenaID  = 0;
}

void Bullet::OnDelete()
{
}

void Bullet::Update(const orxCLOCK_INFO &_rstInfo)
{
}
