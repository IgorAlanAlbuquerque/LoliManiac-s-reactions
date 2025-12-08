#pragma once
#include "ElementalReactionsAPI.h"

namespace Freeze {
    void InitFreezeSpell();
    void RegisterFrostFreezeReaction(ERF_API_V1* api, ERF_ElementHandle frostElem);
}
