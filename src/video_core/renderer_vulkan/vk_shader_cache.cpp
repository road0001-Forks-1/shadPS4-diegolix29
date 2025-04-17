// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include "vk_shader_cache.h"

namespace Vulkan {

ShaderCache::ShaderCache(const std::string& cache_dir) : cache_directory(cache_dir) {
    std::filesystem::create_directories(cache_directory);
}

std::string ShaderCache::GenerateShaderHash(const std::string& shader_source,
                                            vk::ShaderStageFlagBits stage,
                                            const std::vector<std::string>* defines) const {
    std::ostringstream oss;
    oss << shader_source;
    oss << static_cast<uint32_t>(stage);

    if (defines) {
        std::vector<std::string> sorted_defines = *defines;
        std::sort(sorted_defines.begin(), sorted_defines.end());
        for (const auto& def : sorted_defines) {
            oss << def;
        }
    }

    std::hash<std::string> hasher;
    return std::to_string(hasher(oss.str()));
}

std::string ShaderCache::GetCachePath(const std::string& hash) const {
    return cache_directory + "/" + hash + ".spv";
}

std::vector<uint32_t> ShaderCache::LoadShader(const std::string& shader_source,
                                              vk::ShaderStageFlagBits stage) {
    return LoadShader(shader_source, stage, {});
}

void ShaderCache::SaveShader(const std::string& shader_source, vk::ShaderStageFlagBits stage,
                             const std::vector<uint32_t>& spirv) {
    SaveShader(shader_source, stage, {}, spirv);
}

std::vector<uint32_t> ShaderCache::LoadShader(const std::string& shader_source,
                                              vk::ShaderStageFlagBits stage,
                                              const std::vector<std::string>& defines) {
    const std::string path = GetCachePath(GenerateShaderHash(shader_source, stage, &defines));
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        return {};

    std::vector<uint32_t> spirv(file.tellg() / sizeof(uint32_t));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
    return spirv;
}

void ShaderCache::SaveShader(const std::string& shader_source, vk::ShaderStageFlagBits stage,
                             const std::vector<std::string>& defines,
                             const std::vector<uint32_t>& spirv) {
    const std::string path = GetCachePath(GenerateShaderHash(shader_source, stage, &defines));
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
}

} // namespace Vulkan
