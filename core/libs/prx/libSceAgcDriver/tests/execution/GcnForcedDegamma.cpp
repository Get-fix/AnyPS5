#include "prx/libSceAgcDriver/Execution/include/Driver.hpp"
#include "prx/libSceAgcDriver/Execution/include/VulkanDevice.hpp"
#include "prx/libSceAgcDriver/Graphics/include/GuestSamplerResource.hpp"
#include "prx/libSceAgcDriver/Graphics/include/Sampler.hpp"
#include "prx/libSceAgcDriver/Graphics/include/TextureFormat.hpp"
#include "VulkanTestDevice.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace AgcDriver {

struct VulkanDeviceTestAccess {
    static Graphics::Context ContextOf(const VulkanDevice& device) { return device.graphicsContext(); }
};

}

namespace {

using AgcDriver::Graphics::Require;

std::uint32_t GuestFormatOf(VkFormat wanted) {
    for (std::uint32_t format = 1; format < 256; ++format) {
        try {
            if (AgcDriver::Graphics::ResolveTextureFormat(format) == wanted) return format;
        } catch (const std::exception&) {
        }
    }
    throw std::runtime_error("no guest texture format resolves to the wanted Vulkan format");
}

std::shared_ptr<AgcDriver::Graphics::Sampler> ForcingSampler(const AgcDriver::Graphics::Context& context) {
    AgcDriver::Graphics::GuestSamplerResource resource{};
    resource.magFilter = VK_FILTER_LINEAR;
    resource.minFilter = VK_FILTER_LINEAR;
    resource.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    resource.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    resource.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    resource.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    resource.maxAnisotropy = 1.0f;
    resource.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    resource.forceDegamma = true;
    return std::make_shared<AgcDriver::Graphics::Sampler>(context, resource);
}

}

int main() {
    try {
        const auto device = OpenVulkanTestDevice();
        if (!device) return VulkanTestSkipped;
        const auto context = AgcDriver::VulkanDeviceTestAccess::ContextOf(*device);
        const std::array samplers{ForcingSampler(context)};
        const auto unorm = GuestFormatOf(VK_FORMAT_R8G8B8A8_UNORM);
        const auto floating = GuestFormatOf(VK_FORMAT_R16G16B16A16_SFLOAT);
        using AgcDriver::Graphics::ForcesDegammaOn;
        Require(ForcesDegammaOn(unorm, 1u, samplers) && ForcesDegammaOn(floating, 1u, samplers), "an AGC sampler forcing degamma no longer forces it on every format");
        Require(!ForcesDegammaOn(unorm, 0u, samplers), "a sampler outside the mask forced degamma");
        AgcDriverUseGcnShaders_nid_postfix();
        Require(ForcesDegammaOn(unorm, 1u, samplers), "a GCN sampler forcing degamma did not force it on an 8-bit UNORM format");
        Require(!ForcesDegammaOn(floating, 1u, samplers), "a GCN sampler forcing degamma forced it on a float format, which has no sRGB form");
        std::puts("a GCN sampler forces degamma only on formats with an sRGB form; an AGC sampler on every format");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
