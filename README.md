# allimage
some image encode/decode code(KTX2,WEBP,tif,jpg,png,SVG  and any more)

you can add  all file in  your project.

build before  yo msut  setting:

CMAKE:
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

VS:
# must define some values
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

#include folder
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

