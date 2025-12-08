#include "ElementalReactionsAPI.h"
#include "Elements.h"
#include "PCH.h"
#include "single/FreezeFrost.h"

#ifndef DLLEXPORT
    #include "REL/Relocation.h"
#endif
#ifndef DLLEXPORT
    #define DLLEXPORT __declspec(dllexport)
#endif

static ERF_API_V1* g_erf = nullptr;

static ERF_API_V1* AcquireERF() {
    if (g_erf) {
        return g_erf;
    }

    g_erf = ERF_GetAPI(ERF_API_VERSION);

    if (!g_erf) {
        spdlog::error("[Loli] Failed to acquire Elemental Reactions API.");
    }

    return g_erf;
}

static void RegisterAll() {
    auto* api = AcquireERF();
    if (!api) {
        return;
    }

    bool usedBarrier = false;
    if (api->BeginBatchRegistration) {
        usedBarrier = api->BeginBatchRegistration();
    }

    Freeze::InitFreezeSpell();

    Elements::BaseElements elems = Elements::RegisterElements(api);

    Freeze::RegisterFrostFreezeReaction(api, elems.frost);

    if (usedBarrier && api->EndBatchRegistration) {
        api->EndBatchRegistration();
    }
}

static void OnSKSEMessage(SKSE::MessagingInterface::Message* msg) {
    if (!msg) return;

    if (msg->type == SKSE::MessagingInterface::kPostLoad) {
        AcquireERF();
    }
    if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
        if (!g_erf) AcquireERF();
        RegisterAll();
    }
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    spdlog::set_level(spdlog::level::info);

    if (auto* m = SKSE::GetMessagingInterface()) {
        m->RegisterListener(OnSKSEMessage);
        return true;
    }
}