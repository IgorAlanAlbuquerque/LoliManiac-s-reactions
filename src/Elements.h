#pragma once
#include "ElementalReactionsAPI.h"
namespace Elements {
    struct BaseElements {
        ERF_ElementHandle fire{};
        ERF_ElementHandle frost{};
        ERF_ElementHandle shock{};
    };

    BaseElements RegisterElements(ERF_API_V1* api);
}