# allimage
some image encode/decode code
20220709: Log: can read all
support lists:
(png 
tga 
jpg 
gif 
ktx 
dds 
crn 
ktx2
webp
tiff)

use:
1:  you can add  all file in  your project, this need some define and include setting.
2:  build with allimage.vcxproj, less vs2015;
3:  linux use camke build , less cmake 3.12, gcc4.8.5;


## build 

before  yo msut  setting:

# CMAKE:
	ADD_DEFINITIONS(-DKTX_FEATURE_WRITE=1)
	ADD_DEFINITIONS(-DKHRONOS_STATIC)
	ADD_DEFINITIONS(-DLIBKTX)
	ADD_DEFINITIONS(-DPNG_NO_CONFIG_H)
	ADD_DEFINITIONS(-DASTCENC_NEON=0)
	ADD_DEFINITIONS(-DASTCENC_SSE=0)
	ADD_DEFINITIONS(-DASTCENC_AVX=0)
	ADD_DEFINITIONS(-DASTCENC_POPCNT=0)
	ADD_DEFINITIONS(-DASTCENC_F16C=0)
	ADD_DEFINITIONS(-DBASISU_SUPPORT_OPENCL=0)
	
	INCLUDE_DIRECTORIES(                  
	     ${CMAKE_CURRENT_SOURCE_DIR}
	    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/
	    ${CMAKE_CURRENT_SOURCE_DIR}/miniz/
		${CMAKE_CURRENT_SOURCE_DIR}/image/inc/
		${CMAKE_CURRENT_SOURCE_DIR}/image/libjpeg
		${CMAKE_CURRENT_SOURCE_DIR}/image/webp/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/other_include/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/dfdutils/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/basisu/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/utils/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/basisu/transcoder/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/astc-encoder/
		${CMAKE_CURRENT_SOURCE_DIR}/image/ktx/astc-encoder/Source/
	     )

# VS:
	#must define some values
	KTX_FEATURE_WRITE=1
	KHRONOS_STATIC
	LIBKTX
	WIN32
	PNG_NO_CONFIG_H
	ASTCENC_NEON=0
	ASTCENC_SSE=41
	ASTCENC_AVX=2
	ASTCENC_POPCNT=1
	ASTCENC_F16C=1
	BASISD_SUPPORT_FXT1=0
	BASISU_NO_ITERATOR_DEBUG_LEVEL
	KTX_FEATURE_KTX1
	KTX_FEATURE_KTX2
	KTX_FEATURE_WRITE
	BASISD_SUPPORT_KTX2_ZSTD=1
	BASISD_SUPPORT_KTX2=1
	BASISU_SUPPORT_SSE=1
	BASISU_SUPPORT_OPENCL=0
	
	#include
	:
	./image/ktx
	../include
	.
	./zlib
	./image/inc
	./image/tif/port
	./image/tif/win32
	./image/libjpeg
	./image/webp
	./image/ktx/other_include
	./image/ktx/dfdutils
	./image/ktx/basisu
	./image/ktx/utils
	./image/ktx/basisu/transcoder
	./image/ktx/astc-encoder
	./image/ktx/astc-encoder/Source



# Test code

```c++
#include <iostream>
#include "aimage.h"

#ifdef _WIN32  
#ifdef _DEBUG
#pragma comment(lib,"allimaged.lib")
#elif
#pragma comment(lib,"allimage.lib")
#endif // _DEBUG
#endif // WIN32


int main()
{
    AImage* imgpng =  AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001.png");
    AImage* imgtga = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001.tga");
    AImage* imgjpg = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\world.jpg");
    AImage* imggif = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\transgif.gif");
    AImage* imgktx = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.ktx");
    AImage* imgdds = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.dds");
    AImage* imgcrn = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.crn");
    AImage* imgktx2 = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\0318_img0.ktx2");
    AImage* imgwebp = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\baby_tux.webp"); 

    imgpng->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.png", AImageEncodeType::ePNG);
    imgtga->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.tga", AImageEncodeType::eTGA);
    imgjpg->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\worlda.jpg", AImageEncodeType::eJPG);
    imggif->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\transgifa.gif", AImageEncodeType::eGIF);
    imgktx->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.ktx", AImageEncodeType::eKTX);
    imgdds->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.dds", AImageEncodeType::eDDS);
    imgcrn->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.crn", AImageEncodeType::eCRN);
    imgktx2->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\0318_img0a.ktx2", AImageEncodeType::eKTX2);
    imgwebp->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\baby_tuxa.webp", AImageEncodeType::eWEBP);
}


```

