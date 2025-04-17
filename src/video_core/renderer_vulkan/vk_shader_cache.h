// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace Vulkan {

class ShaderCache {
public:
    explicit ShaderCache(const std::string& cache_dir);
    std::vector<uint32_t> LoadShader(const std::string& shader_source,
                                     vk::ShaderStageFlagBits stage);
    void SaveShader(const std::string& shader_source, vk::ShaderStageFlagBits stage,
                    const std::vector<uint32_t>& spirv);

    std::vector<uint32_t> LoadShader(const std::string& shader_source,
                                     vk::ShaderStageFlagBits stage,
                                     const std::vector<std::string>& defines);
    void SaveShader(const std::string& shader_source, vk::ShaderStageFlagBits stage,
                    const std::vector<std::string>& defines, const std::vector<uint32_t>& spirv);

private:
    std::string cache_directory;
    std::string GenerateShaderHash(const std::string& shader_source, vk::ShaderStageFlagBits stage,
                                   const std::vector<std::string>* defines = nullptr) const;
    std::string GetCachePath(const std::string& hash) const;

public:
    ShaderCache() = default;
};

} // namespace Vulkan
