// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "video_core/amdgpu/liverpool.h"

namespace Shader {
struct Info;
}

namespace Vulkan {
std::string GenerateCopyShaderSource(const Shader::Info& info);

bool ExecuteShaderHLE(const Shader::Info& info, const AmdGpu::Liverpool::Regs& regs,
                      const AmdGpu::Liverpool::ComputeProgram& cs_program, Rasterizer& rasterizer,
                      vk::ShaderModule& shader_module);

} // namespace Vulkan
