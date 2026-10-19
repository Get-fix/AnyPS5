#include "prx/libc/include/general/VabiMacros.hpp"
#include "SceTypes.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

extern "C" {
int APS5_VABI sceNpCmpOnlineId(const NpOnlineId* first, const NpOnlineId* second);
int APS5_VABI sceNpMatching2Initialize(const void* param);
int APS5_VABI sceNpMatching2Terminate(void);
int APS5_VABI sceNpMatching2AbortContextStart(void);
int APS5_VABI sceNpMatching2GrantRoomOwner(void);
int APS5_VABI sceNpMatching2SignalingGetConnectionStatus(void);
int APS5_VABI sceNpMatching2SignalingGetPingInfo(void);
int APS5_VABI sceNpSignalingInitialize(std::int64_t pool_size, int thread_priority, int cpu_affinity_mask, std::int64_t thread_stack_size);
int APS5_VABI sceNpSignalingTerminate(void);
int APS5_VABI sceNpSignalingDeactivateConnection(void);
int APS5_VABI sceNpLookupCreateTitleCtx(const NpId* self_np_id);
int APS5_VABI sceNpLookupAbortRequest(void);
int APS5_VABI sceNpLookupCreateAsyncRequest(void);
int APS5_VABI sceNpLookupPollAsync(void);
}

namespace {

constexpr int NpInvalidArgument = static_cast<int>(0x80550003u);
constexpr int NpSignedOut = static_cast<int>(0x80550006u);
constexpr int NpUtilNotMatch = static_cast<int>(0x80550609u);
constexpr int CommunityInsufficientArgument = static_cast<int>(0x8055070Cu);
constexpr int Matching2AlreadyInitialized = static_cast<int>(0x80550C02u);
constexpr int Matching2NotInitialized = static_cast<int>(0x80550C03u);
constexpr int Matching2InvalidArgument = static_cast<int>(0x80550C0Au);
constexpr int SignalingNotInitialized = static_cast<int>(0x80552701u);
constexpr int SignalingAlreadyInitialized = static_cast<int>(0x80552702u);

void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "NpPs4Imports: %s\n", message);
        std::abort();
    }
}

template <typename TCall>
void RequireNotImplemented(TCall call, const std::string& name) {
    try {
        call();
    } catch (const std::runtime_error& error) {
        Require(std::string(error.what()) == name + " not implemented", "a declared function threw another error");
        return;
    }
    Require(false, "a declared function returned");
}

NpOnlineId OnlineId(const char* name) {
    NpOnlineId id{};
    std::memcpy(id.data, name, std::strlen(name));
    return id;
}

void CheckOnlineIds() {
    auto first = OnlineId("player-one");
    auto same = OnlineId("player-one");
    const auto other = OnlineId("player-two");
    same.data[16] = 'x';
    same.pad[0] = 1;
    Require(sceNpCmpOnlineId(&first, &same) == 0, "online ids that differ only past 16 characters do not match");
    Require(sceNpCmpOnlineId(&first, &other) == NpUtilNotMatch, "different online ids match");
    const auto full = OnlineId("abcdefghijklmnop");
    auto fullOther = full;
    fullOther.data[15] = 'q';
    Require(sceNpCmpOnlineId(&full, &fullOther) == NpUtilNotMatch, "the sixteenth character is not compared");
    Require(sceNpCmpOnlineId(nullptr, &first) == NpInvalidArgument && sceNpCmpOnlineId(&first, nullptr) == NpInvalidArgument, "a null online id was compared");
}

void CheckMatching2() {
    unsigned char param[0x30]{};
    const auto setSize = [&](std::uint64_t size) { std::memcpy(param + 0x20, &size, sizeof(size)); };
    Require(sceNpMatching2Terminate() == Matching2NotInitialized, "NpMatching2 terminated before initialization");
    Require(sceNpMatching2Initialize(nullptr) == Matching2InvalidArgument, "a null NpMatching2 parameter was accepted");
    setSize(0x20u);
    Require(sceNpMatching2Initialize(param) == Matching2InvalidArgument, "an NpMatching2 parameter of size 0x20 was accepted");
    setSize(0x28u);
    Require(sceNpMatching2Initialize(param) == 0, "an NpMatching2 parameter of size 0x28 was refused");
    Require(sceNpMatching2Initialize(param) == Matching2AlreadyInitialized, "NpMatching2 initialized twice");
    Require(sceNpMatching2Terminate() == 0, "NpMatching2 did not terminate");
    setSize(0x30u);
    Require(sceNpMatching2Initialize(param) == 0, "an NpMatching2 parameter with an SSL pool was refused");
    Require(sceNpMatching2Terminate() == 0, "NpMatching2 did not terminate after the SSL form");
    RequireNotImplemented([] { sceNpMatching2AbortContextStart(); }, "sceNpMatching2AbortContextStart");
    RequireNotImplemented([] { sceNpMatching2GrantRoomOwner(); }, "sceNpMatching2GrantRoomOwner");
    RequireNotImplemented([] { sceNpMatching2SignalingGetConnectionStatus(); }, "sceNpMatching2SignalingGetConnectionStatus");
    RequireNotImplemented([] { sceNpMatching2SignalingGetPingInfo(); }, "sceNpMatching2SignalingGetPingInfo");
}

void CheckSignaling() {
    Require(sceNpSignalingTerminate() == SignalingNotInitialized, "NpSignaling terminated before initialization");
    Require(sceNpSignalingInitialize(0, 0, 0, 0) == 0, "NpSignaling did not initialize");
    Require(sceNpSignalingInitialize(0x20000, 700, 0, 0x4000) == SignalingAlreadyInitialized, "NpSignaling initialized twice");
    Require(sceNpSignalingTerminate() == 0, "NpSignaling did not terminate");
    Require(sceNpSignalingTerminate() == SignalingNotInitialized, "NpSignaling terminated twice");
    RequireNotImplemented([] { sceNpSignalingDeactivateConnection(); }, "sceNpSignalingDeactivateConnection");
}

void CheckLookup() {
    NpId npId{};
    npId.online_id = OnlineId("player-one");
    Require(sceNpLookupCreateTitleCtx(&npId) == NpSignedOut, "a lookup context was created for a signed-out user");
    Require(sceNpLookupCreateTitleCtx(nullptr) == CommunityInsufficientArgument, "a null NP id was accepted");
    RequireNotImplemented([] { sceNpLookupAbortRequest(); }, "sceNpLookupAbortRequest");
    RequireNotImplemented([] { sceNpLookupCreateAsyncRequest(); }, "sceNpLookupCreateAsyncRequest");
    RequireNotImplemented([] { sceNpLookupPollAsync(); }, "sceNpLookupPollAsync");
}

}

int main() {
    CheckOnlineIds();
    CheckMatching2();
    CheckSignaling();
    CheckLookup();
    return 0;
}
