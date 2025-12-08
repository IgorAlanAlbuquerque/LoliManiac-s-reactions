#include "Elements.h"

Elements::BaseElements Elements::RegisterElements(ERF_API_V1* api) {
    BaseElements out{};

    if (!api || !api->RegisterElement) {
        return out;
    }

    constexpr std::uint32_t kMagicDamageFire = 0x0001CEAD;
    constexpr std::uint32_t kMagicDamageFrost = 0x0001CEAE;
    constexpr std::uint32_t kMagicDamageShock = 0x0001CEAF;

    {
        ERF_ElementDesc_Public d{};
        d.name = "Fire";
        d.colorRGB = 0xF04A3A;
        d.keywordID = kMagicDamageFire;
        out.fire = api->RegisterElement(d);
    }
    {
        ERF_ElementDesc_Public d{};
        d.name = "Frost";
        d.colorRGB = 0x4FB2FF;
        d.keywordID = kMagicDamageFrost;
        out.frost = api->RegisterElement(d);
    }
    {
        ERF_ElementDesc_Public d{};
        d.name = "Shock";
        d.colorRGB = 0xFFD02A;
        d.keywordID = kMagicDamageShock;
        out.shock = api->RegisterElement(d);
    }

    return out;
}