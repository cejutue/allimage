/* Copyright 2019-2020 The Khronos Group Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

/***************************** Do not edit.  *****************************
             Automatically generated by makevkswitch.pl.
 *************************************************************************/
case VK_FORMAT_R4G4_UNORM_PACK8: {
    int channels[] = {1,0}; int bits[] = {4,4};
    return createDFDPacked(0, 2, bits, channels, s_UNORM);
}
case VK_FORMAT_R4G4B4A4_UNORM_PACK16: {
    int channels[] = {3,2,1,0}; int bits[] = {4,4,4,4};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_B4G4R4A4_UNORM_PACK16: {
    int channels[] = {3,0,1,2}; int bits[] = {4,4,4,4};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_R5G6B5_UNORM_PACK16: {
    int channels[] = {2,1,0}; int bits[] = {5,6,5};
    return createDFDPacked(0, 3, bits, channels, s_UNORM);
}
case VK_FORMAT_B5G6R5_UNORM_PACK16: {
    int channels[] = {0,1,2}; int bits[] = {5,6,5};
    return createDFDPacked(0, 3, bits, channels, s_UNORM);
}
case VK_FORMAT_R5G5B5A1_UNORM_PACK16: {
    int channels[] = {3,2,1,0}; int bits[] = {1,5,5,5};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_B5G5R5A1_UNORM_PACK16: {
    int channels[] = {3,0,1,2}; int bits[] = {1,5,5,5};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_A1R5G5B5_UNORM_PACK16: {
    int channels[] = {2,1,0,3}; int bits[] = {5,5,5,1};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_R8_UNORM: return createDFDUnpacked(0, 1, 1, 0, s_UNORM);
case VK_FORMAT_R8_SNORM: return createDFDUnpacked(0, 1, 1, 0, s_SNORM);
case VK_FORMAT_R8_USCALED: return createDFDUnpacked(0, 1, 1, 0, s_USCALED);
case VK_FORMAT_R8_SSCALED: return createDFDUnpacked(0, 1, 1, 0, s_SSCALED);
case VK_FORMAT_R8_UINT: return createDFDUnpacked(0, 1, 1, 0, s_UINT);
case VK_FORMAT_R8_SINT: return createDFDUnpacked(0, 1, 1, 0, s_SINT);
case VK_FORMAT_R8_SRGB: return createDFDUnpacked(0, 1, 1, 0, s_SRGB);
case VK_FORMAT_R8G8_UNORM: return createDFDUnpacked(0, 2, 1, 0, s_UNORM);
case VK_FORMAT_R8G8_SNORM: return createDFDUnpacked(0, 2, 1, 0, s_SNORM);
case VK_FORMAT_R8G8_USCALED: return createDFDUnpacked(0, 2, 1, 0, s_USCALED);
case VK_FORMAT_R8G8_SSCALED: return createDFDUnpacked(0, 2, 1, 0, s_SSCALED);
case VK_FORMAT_R8G8_UINT: return createDFDUnpacked(0, 2, 1, 0, s_UINT);
case VK_FORMAT_R8G8_SINT: return createDFDUnpacked(0, 2, 1, 0, s_SINT);
case VK_FORMAT_R8G8_SRGB: return createDFDUnpacked(0, 2, 1, 0, s_SRGB);
case VK_FORMAT_R8G8B8_UNORM: return createDFDUnpacked(0, 3, 1, 0, s_UNORM);
case VK_FORMAT_R8G8B8_SNORM: return createDFDUnpacked(0, 3, 1, 0, s_SNORM);
case VK_FORMAT_R8G8B8_USCALED: return createDFDUnpacked(0, 3, 1, 0, s_USCALED);
case VK_FORMAT_R8G8B8_SSCALED: return createDFDUnpacked(0, 3, 1, 0, s_SSCALED);
case VK_FORMAT_R8G8B8_UINT: return createDFDUnpacked(0, 3, 1, 0, s_UINT);
case VK_FORMAT_R8G8B8_SINT: return createDFDUnpacked(0, 3, 1, 0, s_SINT);
case VK_FORMAT_R8G8B8_SRGB: return createDFDUnpacked(0, 3, 1, 0, s_SRGB);
case VK_FORMAT_B8G8R8_UNORM: return createDFDUnpacked(0, 3, 1, 1, s_UNORM);
case VK_FORMAT_B8G8R8_SNORM: return createDFDUnpacked(0, 3, 1, 1, s_SNORM);
case VK_FORMAT_B8G8R8_USCALED: return createDFDUnpacked(0, 3, 1, 1, s_USCALED);
case VK_FORMAT_B8G8R8_SSCALED: return createDFDUnpacked(0, 3, 1, 1, s_SSCALED);
case VK_FORMAT_B8G8R8_UINT: return createDFDUnpacked(0, 3, 1, 1, s_UINT);
case VK_FORMAT_B8G8R8_SINT: return createDFDUnpacked(0, 3, 1, 1, s_SINT);
case VK_FORMAT_B8G8R8_SRGB: return createDFDUnpacked(0, 3, 1, 1, s_SRGB);
case VK_FORMAT_R8G8B8A8_UNORM: return createDFDUnpacked(0, 4, 1, 0, s_UNORM);
case VK_FORMAT_R8G8B8A8_SNORM: return createDFDUnpacked(0, 4, 1, 0, s_SNORM);
case VK_FORMAT_R8G8B8A8_USCALED: return createDFDUnpacked(0, 4, 1, 0, s_USCALED);
case VK_FORMAT_R8G8B8A8_SSCALED: return createDFDUnpacked(0, 4, 1, 0, s_SSCALED);
case VK_FORMAT_R8G8B8A8_UINT: return createDFDUnpacked(0, 4, 1, 0, s_UINT);
case VK_FORMAT_R8G8B8A8_SINT: return createDFDUnpacked(0, 4, 1, 0, s_SINT);
case VK_FORMAT_R8G8B8A8_SRGB: return createDFDUnpacked(0, 4, 1, 0, s_SRGB);
case VK_FORMAT_B8G8R8A8_UNORM: return createDFDUnpacked(0, 4, 1, 1, s_UNORM);
case VK_FORMAT_B8G8R8A8_SNORM: return createDFDUnpacked(0, 4, 1, 1, s_SNORM);
case VK_FORMAT_B8G8R8A8_USCALED: return createDFDUnpacked(0, 4, 1, 1, s_USCALED);
case VK_FORMAT_B8G8R8A8_SSCALED: return createDFDUnpacked(0, 4, 1, 1, s_SSCALED);
case VK_FORMAT_B8G8R8A8_UINT: return createDFDUnpacked(0, 4, 1, 1, s_UINT);
case VK_FORMAT_B8G8R8A8_SINT: return createDFDUnpacked(0, 4, 1, 1, s_SINT);
case VK_FORMAT_B8G8R8A8_SRGB: return createDFDUnpacked(0, 4, 1, 1, s_SRGB);
case VK_FORMAT_A8B8G8R8_UNORM_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_A8B8G8R8_SNORM_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_SNORM);
}
case VK_FORMAT_A8B8G8R8_USCALED_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_USCALED);
}
case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_SSCALED);
}
case VK_FORMAT_A8B8G8R8_UINT_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_UINT);
}
case VK_FORMAT_A8B8G8R8_SINT_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_SINT);
}
case VK_FORMAT_A8B8G8R8_SRGB_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {8,8,8,8};
    return createDFDPacked(0, 4, bits, channels, s_SRGB);
}
case VK_FORMAT_A2R10G10B10_UNORM_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_A2R10G10B10_SNORM_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SNORM);
}
case VK_FORMAT_A2R10G10B10_USCALED_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_USCALED);
}
case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SSCALED);
}
case VK_FORMAT_A2R10G10B10_UINT_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_UINT);
}
case VK_FORMAT_A2R10G10B10_SINT_PACK32: {
    int channels[] = {2,1,0,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SINT);
}
case VK_FORMAT_A2B10G10R10_UNORM_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_A2B10G10R10_SNORM_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SNORM);
}
case VK_FORMAT_A2B10G10R10_USCALED_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_USCALED);
}
case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SSCALED);
}
case VK_FORMAT_A2B10G10R10_UINT_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_UINT);
}
case VK_FORMAT_A2B10G10R10_SINT_PACK32: {
    int channels[] = {0,1,2,3}; int bits[] = {10,10,10,2};
    return createDFDPacked(0, 4, bits, channels, s_SINT);
}
case VK_FORMAT_R16_UNORM: return createDFDUnpacked(0, 1, 2, 0, s_UNORM);
case VK_FORMAT_R16_SNORM: return createDFDUnpacked(0, 1, 2, 0, s_SNORM);
case VK_FORMAT_R16_USCALED: return createDFDUnpacked(0, 1, 2, 0, s_USCALED);
case VK_FORMAT_R16_SSCALED: return createDFDUnpacked(0, 1, 2, 0, s_SSCALED);
case VK_FORMAT_R16_UINT: return createDFDUnpacked(0, 1, 2, 0, s_UINT);
case VK_FORMAT_R16_SINT: return createDFDUnpacked(0, 1, 2, 0, s_SINT);
case VK_FORMAT_R16_SFLOAT: return createDFDUnpacked(0, 1, 2, 0, s_SFLOAT);
case VK_FORMAT_R16G16_UNORM: return createDFDUnpacked(0, 2, 2, 0, s_UNORM);
case VK_FORMAT_R16G16_SNORM: return createDFDUnpacked(0, 2, 2, 0, s_SNORM);
case VK_FORMAT_R16G16_USCALED: return createDFDUnpacked(0, 2, 2, 0, s_USCALED);
case VK_FORMAT_R16G16_SSCALED: return createDFDUnpacked(0, 2, 2, 0, s_SSCALED);
case VK_FORMAT_R16G16_UINT: return createDFDUnpacked(0, 2, 2, 0, s_UINT);
case VK_FORMAT_R16G16_SINT: return createDFDUnpacked(0, 2, 2, 0, s_SINT);
case VK_FORMAT_R16G16_SFLOAT: return createDFDUnpacked(0, 2, 2, 0, s_SFLOAT);
case VK_FORMAT_R16G16B16_UNORM: return createDFDUnpacked(0, 3, 2, 0, s_UNORM);
case VK_FORMAT_R16G16B16_SNORM: return createDFDUnpacked(0, 3, 2, 0, s_SNORM);
case VK_FORMAT_R16G16B16_USCALED: return createDFDUnpacked(0, 3, 2, 0, s_USCALED);
case VK_FORMAT_R16G16B16_SSCALED: return createDFDUnpacked(0, 3, 2, 0, s_SSCALED);
case VK_FORMAT_R16G16B16_UINT: return createDFDUnpacked(0, 3, 2, 0, s_UINT);
case VK_FORMAT_R16G16B16_SINT: return createDFDUnpacked(0, 3, 2, 0, s_SINT);
case VK_FORMAT_R16G16B16_SFLOAT: return createDFDUnpacked(0, 3, 2, 0, s_SFLOAT);
case VK_FORMAT_R16G16B16A16_UNORM: return createDFDUnpacked(0, 4, 2, 0, s_UNORM);
case VK_FORMAT_R16G16B16A16_SNORM: return createDFDUnpacked(0, 4, 2, 0, s_SNORM);
case VK_FORMAT_R16G16B16A16_USCALED: return createDFDUnpacked(0, 4, 2, 0, s_USCALED);
case VK_FORMAT_R16G16B16A16_SSCALED: return createDFDUnpacked(0, 4, 2, 0, s_SSCALED);
case VK_FORMAT_R16G16B16A16_UINT: return createDFDUnpacked(0, 4, 2, 0, s_UINT);
case VK_FORMAT_R16G16B16A16_SINT: return createDFDUnpacked(0, 4, 2, 0, s_SINT);
case VK_FORMAT_R16G16B16A16_SFLOAT: return createDFDUnpacked(0, 4, 2, 0, s_SFLOAT);
case VK_FORMAT_R32_UINT: return createDFDUnpacked(0, 1, 4, 0, s_UINT);
case VK_FORMAT_R32_SINT: return createDFDUnpacked(0, 1, 4, 0, s_SINT);
case VK_FORMAT_R32_SFLOAT: return createDFDUnpacked(0, 1, 4, 0, s_SFLOAT);
case VK_FORMAT_R32G32_UINT: return createDFDUnpacked(0, 2, 4, 0, s_UINT);
case VK_FORMAT_R32G32_SINT: return createDFDUnpacked(0, 2, 4, 0, s_SINT);
case VK_FORMAT_R32G32_SFLOAT: return createDFDUnpacked(0, 2, 4, 0, s_SFLOAT);
case VK_FORMAT_R32G32B32_UINT: return createDFDUnpacked(0, 3, 4, 0, s_UINT);
case VK_FORMAT_R32G32B32_SINT: return createDFDUnpacked(0, 3, 4, 0, s_SINT);
case VK_FORMAT_R32G32B32_SFLOAT: return createDFDUnpacked(0, 3, 4, 0, s_SFLOAT);
case VK_FORMAT_R32G32B32A32_UINT: return createDFDUnpacked(0, 4, 4, 0, s_UINT);
case VK_FORMAT_R32G32B32A32_SINT: return createDFDUnpacked(0, 4, 4, 0, s_SINT);
case VK_FORMAT_R32G32B32A32_SFLOAT: return createDFDUnpacked(0, 4, 4, 0, s_SFLOAT);
case VK_FORMAT_R64_UINT: return createDFDUnpacked(0, 1, 8, 0, s_UINT);
case VK_FORMAT_R64_SINT: return createDFDUnpacked(0, 1, 8, 0, s_SINT);
case VK_FORMAT_R64_SFLOAT: return createDFDUnpacked(0, 1, 8, 0, s_SFLOAT);
case VK_FORMAT_R64G64_UINT: return createDFDUnpacked(0, 2, 8, 0, s_UINT);
case VK_FORMAT_R64G64_SINT: return createDFDUnpacked(0, 2, 8, 0, s_SINT);
case VK_FORMAT_R64G64_SFLOAT: return createDFDUnpacked(0, 2, 8, 0, s_SFLOAT);
case VK_FORMAT_R64G64B64_UINT: return createDFDUnpacked(0, 3, 8, 0, s_UINT);
case VK_FORMAT_R64G64B64_SINT: return createDFDUnpacked(0, 3, 8, 0, s_SINT);
case VK_FORMAT_R64G64B64_SFLOAT: return createDFDUnpacked(0, 3, 8, 0, s_SFLOAT);
case VK_FORMAT_R64G64B64A64_UINT: return createDFDUnpacked(0, 4, 8, 0, s_UINT);
case VK_FORMAT_R64G64B64A64_SINT: return createDFDUnpacked(0, 4, 8, 0, s_SINT);
case VK_FORMAT_R64G64B64A64_SFLOAT: return createDFDUnpacked(0, 4, 8, 0, s_SFLOAT);
case VK_FORMAT_B10G11R11_UFLOAT_PACK32: {
    int channels[] = {0,1,2}; int bits[] = {11,11,10};
    return createDFDPacked(0, 3, bits, channels, s_UFLOAT);
}
case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: {
    int bits[] = {0}; int channels[] = {0};
    return createDFDPacked(0, 6, bits, channels, s_UFLOAT);
}
case VK_FORMAT_D16_UNORM: return createDFDDepthStencil(16,0,2);
case VK_FORMAT_X8_D24_UNORM_PACK32: return createDFDDepthStencil(24,0,4);
case VK_FORMAT_D32_SFLOAT: return createDFDDepthStencil(32,0,4);
case VK_FORMAT_S8_UINT: return createDFDDepthStencil(0,8,1);
case VK_FORMAT_D16_UNORM_S8_UINT: return createDFDDepthStencil(16,8,3);
case VK_FORMAT_D24_UNORM_S8_UINT: return createDFDDepthStencil(24,8,4);
case VK_FORMAT_D32_SFLOAT_S8_UINT: return createDFDDepthStencil(32,8,5);
case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return createDFDCompressed(c_BC1_RGB, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return createDFDCompressed(c_BC1_RGB, 4, 4, 1, s_SRGB);
case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return createDFDCompressed(c_BC1_RGBA, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return createDFDCompressed(c_BC1_RGBA, 4, 4, 1, s_SRGB);
case VK_FORMAT_BC2_UNORM_BLOCK: return createDFDCompressed(c_BC2, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC2_SRGB_BLOCK: return createDFDCompressed(c_BC2, 4, 4, 1, s_SRGB);
case VK_FORMAT_BC3_UNORM_BLOCK: return createDFDCompressed(c_BC3, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC3_SRGB_BLOCK: return createDFDCompressed(c_BC3, 4, 4, 1, s_SRGB);
case VK_FORMAT_BC4_UNORM_BLOCK: return createDFDCompressed(c_BC4, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC4_SNORM_BLOCK: return createDFDCompressed(c_BC4, 4, 4, 1, s_SNORM);
case VK_FORMAT_BC5_UNORM_BLOCK: return createDFDCompressed(c_BC5, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC5_SNORM_BLOCK: return createDFDCompressed(c_BC5, 4, 4, 1, s_SNORM);
case VK_FORMAT_BC6H_UFLOAT_BLOCK: return createDFDCompressed(c_BC6H, 4, 4, 1, s_UFLOAT);
case VK_FORMAT_BC6H_SFLOAT_BLOCK: return createDFDCompressed(c_BC6H, 4, 4, 1, s_SFLOAT);
case VK_FORMAT_BC7_UNORM_BLOCK: return createDFDCompressed(c_BC7, 4, 4, 1, s_UNORM);
case VK_FORMAT_BC7_SRGB_BLOCK: return createDFDCompressed(c_BC7, 4, 4, 1, s_SRGB);
case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8, 4, 4, 1, s_UNORM);
case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8, 4, 4, 1, s_SRGB);
case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8A1, 4, 4, 1, s_UNORM);
case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8A1, 4, 4, 1, s_SRGB);
case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8A8, 4, 4, 1, s_UNORM);
case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return createDFDCompressed(c_ETC2_R8G8B8A8, 4, 4, 1, s_SRGB);
case VK_FORMAT_EAC_R11_UNORM_BLOCK: return createDFDCompressed(c_EAC_R11, 4, 4, 1, s_UNORM);
case VK_FORMAT_EAC_R11_SNORM_BLOCK: return createDFDCompressed(c_EAC_R11, 4, 4, 1, s_SNORM);
case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return createDFDCompressed(c_EAC_R11G11, 4, 4, 1, s_UNORM);
case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return createDFDCompressed(c_EAC_R11G11, 4, 4, 1, s_SNORM);
case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 4, 4, 1, s_UNORM);
case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 4, 4, 1, s_SRGB);
case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 5, 4, 1, s_UNORM);
case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 5, 4, 1, s_SRGB);
case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 5, 5, 1, s_UNORM);
case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 5, 5, 1, s_SRGB);
case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 6, 5, 1, s_UNORM);
case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 6, 5, 1, s_SRGB);
case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 6, 6, 1, s_UNORM);
case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 6, 6, 1, s_SRGB);
case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 8, 5, 1, s_UNORM);
case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 8, 5, 1, s_SRGB);
case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 8, 6, 1, s_UNORM);
case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 8, 6, 1, s_SRGB);
case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 8, 8, 1, s_UNORM);
case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 8, 8, 1, s_SRGB);
case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 10, 5, 1, s_UNORM);
case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 10, 5, 1, s_SRGB);
case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 10, 6, 1, s_UNORM);
case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 10, 6, 1, s_SRGB);
case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 10, 8, 1, s_UNORM);
case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 10, 8, 1, s_SRGB);
case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 10, 10, 1, s_UNORM);
case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 10, 10, 1, s_SRGB);
case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 12, 10, 1, s_UNORM);
case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 12, 10, 1, s_SRGB);
case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return createDFDCompressed(c_ASTC, 12, 12, 1, s_UNORM);
case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return createDFDCompressed(c_ASTC, 12, 12, 1, s_SRGB);
case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return createDFDCompressed(c_PVRTC, 8, 4, 1, s_UNORM);
case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return createDFDCompressed(c_PVRTC, 4, 4, 1, s_UNORM);
case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return createDFDCompressed(c_PVRTC2, 8, 4, 1, s_UNORM);
case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return createDFDCompressed(c_PVRTC2, 4, 4, 1, s_UNORM);
case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return createDFDCompressed(c_PVRTC, 8, 4, 1, s_SRGB);
case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return createDFDCompressed(c_PVRTC, 4, 4, 1, s_SRGB);
case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return createDFDCompressed(c_PVRTC2, 8, 4, 1, s_SRGB);
case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return createDFDCompressed(c_PVRTC2, 4, 4, 1, s_SRGB);
case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 1, s_SFLOAT);
case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 4, 1, s_SFLOAT);
case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 1, s_SFLOAT);
case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 5, 1, s_SFLOAT);
case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 1, s_SFLOAT);
case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 8, 5, 1, s_SFLOAT);
case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 8, 6, 1, s_SFLOAT);
case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 8, 8, 1, s_SFLOAT);
case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 10, 5, 1, s_SFLOAT);
case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 10, 6, 1, s_SFLOAT);
case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 10, 8, 1, s_SFLOAT);
case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 10, 10, 1, s_SFLOAT);
case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 12, 10, 1, s_SFLOAT);
case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 12, 12, 1, s_SFLOAT);
case VK_FORMAT_ASTC_3x3x3_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 3, 3, 3, s_UNORM);
case VK_FORMAT_ASTC_3x3x3_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 3, 3, 3, s_SRGB);
case VK_FORMAT_ASTC_3x3x3_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 3, 3, 3, s_SFLOAT);
case VK_FORMAT_ASTC_4x3x3_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 3, 3, s_UNORM);
case VK_FORMAT_ASTC_4x3x3_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 3, 3, s_SRGB);
case VK_FORMAT_ASTC_4x3x3_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 3, 3, s_SFLOAT);
case VK_FORMAT_ASTC_4x4x3_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 3, s_UNORM);
case VK_FORMAT_ASTC_4x4x3_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 3, s_SRGB);
case VK_FORMAT_ASTC_4x4x3_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 3, s_SFLOAT);
case VK_FORMAT_ASTC_4x4x4_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 4, s_UNORM);
case VK_FORMAT_ASTC_4x4x4_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 4, s_SRGB);
case VK_FORMAT_ASTC_4x4x4_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 4, 4, 4, s_SFLOAT);
case VK_FORMAT_ASTC_5x4x4_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 4, 4, s_UNORM);
case VK_FORMAT_ASTC_5x4x4_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 4, 4, s_SRGB);
case VK_FORMAT_ASTC_5x4x4_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 4, 4, s_SFLOAT);
case VK_FORMAT_ASTC_5x5x4_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 4, s_UNORM);
case VK_FORMAT_ASTC_5x5x4_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 4, s_SRGB);
case VK_FORMAT_ASTC_5x5x4_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 4, s_SFLOAT);
case VK_FORMAT_ASTC_5x5x5_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 5, s_UNORM);
case VK_FORMAT_ASTC_5x5x5_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 5, s_SRGB);
case VK_FORMAT_ASTC_5x5x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 5, 5, 5, s_SFLOAT);
case VK_FORMAT_ASTC_6x5x5_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 5, 5, s_UNORM);
case VK_FORMAT_ASTC_6x5x5_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 5, 5, s_SRGB);
case VK_FORMAT_ASTC_6x5x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 5, 5, s_SFLOAT);
case VK_FORMAT_ASTC_6x6x5_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 5, s_UNORM);
case VK_FORMAT_ASTC_6x6x5_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 5, s_SRGB);
case VK_FORMAT_ASTC_6x6x5_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 5, s_SFLOAT);
case VK_FORMAT_ASTC_6x6x6_UNORM_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 6, s_UNORM);
case VK_FORMAT_ASTC_6x6x6_SRGB_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 6, s_SRGB);
case VK_FORMAT_ASTC_6x6x6_SFLOAT_BLOCK_EXT: return createDFDCompressed(c_ASTC, 6, 6, 6, s_SFLOAT);
case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT: {
    int channels[] = {2,1,0,3}; int bits[] = {4,4,4,4};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT: {
    int channels[] = {0,1,2,3}; int bits[] = {4,4,4,4};
    return createDFDPacked(0, 4, bits, channels, s_UNORM);
}
