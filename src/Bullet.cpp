/**
 * @file Bullet.cpp
 * @date 2-Oct-2021
 */

#include "Bullet.h"

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
}

void Bullet::OnDelete()
{
}

void Bullet::Update(const orxCLOCK_INFO &_rstInfo)
{
}
