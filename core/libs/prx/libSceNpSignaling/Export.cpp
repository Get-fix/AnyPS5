#include <cstdint>
#include <mutex>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

static constexpr int SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED = static_cast<int>(0x80552701);
static constexpr int SCE_NP_SIGNALING_ERROR_ALREADY_INITIALIZED = static_cast<int>(0x80552702);

namespace {

std::mutex signalingMutex;
bool signalingInitialized = false;

}

extern "C" {

int APS5_VABI sceNpSignalingActivateConnection() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingCreateContext() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingDeleteContext() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingGetConnectionInfo() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingInitialize(std::int64_t pool_size, int thread_priority, int cpu_affinity_mask, std::int64_t thread_stack_size) {
    (void)pool_size;
    (void)thread_priority;
    (void)cpu_affinity_mask;
    (void)thread_stack_size;
    std::lock_guard lock(signalingMutex);
    if (signalingInitialized) return SCE_NP_SIGNALING_ERROR_ALREADY_INITIALIZED;
    signalingInitialized = true;
    return 0;
}

int APS5_VABI sceNpSignalingSetContextOption() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingTerminate(void) {
    std::lock_guard lock(signalingMutex);
    if (!signalingInitialized) return SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
    signalingInitialized = false;
    return 0;
}

int APS5_VABI sceNpSignalingTerminateConnection() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNpSignalingDeactivateConnection(void) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

}
