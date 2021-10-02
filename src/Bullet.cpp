/**
 * @file Bullet.cpp
 * @date 2-Oct-2021
 */

#include "Bullet.h"

void Bullet::OnCreate()
{
    orxConfig_SetBool("IsBullet", orxTRUE);
}

void Bullet::OnDelete()
{
}

void Bullet::Update(const orxCLOCK_INFO &_rstInfo)
{
}
