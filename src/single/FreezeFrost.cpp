#include "FreezeFrost.h"

#include "PCH.h"

static RE::SpellItem* g_FreezeSpell = nullptr;

static void OnFrostFreeze(const ERF_ReactionContext& ctx, void*) {
    if (!ctx.target) {
        return;
    }
    if (!g_FreezeSpell) {
        spdlog::warn("[Loli] Frost freeze reaction fired but g_loliFreezeSpell == nullptr.");
        return;
    }

    auto* caster = RE::PlayerCharacter::GetSingleton();
    if (!caster) {
        return;
    }

    auto* casterMagic = caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
    if (!casterMagic) {
        return;
    }

    casterMagic->CastSpellImmediate(g_FreezeSpell, false, ctx.target, 1.0f, false, 0.0f, caster);
}

void Freeze::InitFreezeSpell() {
    auto* dh = RE::TESDataHandler::GetSingleton();
    if (!dh) {
        spdlog::error("[Loli] TESDataHandler not available, cannot resolve freeze spell.");
        return;
    }

    constexpr RE::FormID kFreezeSpellID = 0x00000800;
    const char* kPluginName = "LoliManiacSingleReactions.esp";

    g_FreezeSpell = dh->LookupForm<RE::SpellItem>(kFreezeSpellID, kPluginName);
    if (!g_FreezeSpell) {
        spdlog::error("[Loli] Freeze spell not found in {} (formID {:08X}).", kPluginName, kFreezeSpellID);
    }
}

void Freeze::RegisterFrostFreezeReaction(ERF_API_V1* api, ERF_ElementHandle frostElem) {
    if (!api || !api->RegisterReaction) {
        spdlog::error("[Loli] ERF API or RegisterReaction invalid.");
        return;
    }
    if (!frostElem) {
        spdlog::error("[Loli] Frost element handle is 0, cannot register reaction.");
        return;
    }

    ERF_ElementHandle elemsFrost[] = {frostElem};

    ERF_ReactionDesc_Public r{};
    r.name = "Frost_Freeze_85";
    r.elements = elemsFrost;
    r.elementCount = 1;
    r.ordered = false;
    r.minPctEach = 0.85f;
    r.minSumSelected = 0.0f;
    r.cooldownSeconds = 0.5f;
    r.elementLockoutSeconds = 10.0f;
    r.hudTint = 0x4FB2FF;
    r.cb = &OnFrostFreeze;
    r.user = nullptr;
    r.iconName = "ERF_ICON__erf_core__frost";

    api->RegisterReaction(r);
}
