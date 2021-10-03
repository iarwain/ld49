/**
 * @file Object.cpp
 * @date 2-Oct-2021
 */

#include "Object.h"

void Object::OnCreate()
{
    orxConfig_SetBool("IsObject", orxTRUE);
    bDead = bUnstable = orxFALSE;
}

void Object::OnDelete()
{
}

void Object::Update(const orxCLOCK_INFO &_rstInfo)
{
}

orxBOOL Object::OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload)
{
    if(!orxString_Compare(_rstPayload.zParamName, "unstable"))
    {
        _rstPayload.fValue = bUnstable ? orxFLOAT_1 : orxFLOAT_0;
    }
    return orxTRUE;
}
