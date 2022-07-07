#include "pch.h"

#include <bitset>
#include <memory.h>
#include <math.h>
using namespace UTILITY_NAME;


unsigned int CRC_TABLE[] = {
		0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};

//隔行扫描
static const unsigned ADAM7_IX[7] = { 0, 4, 0, 2, 0, 1, 0 }; /*x start values*/
static const unsigned ADAM7_IY[7] = { 0, 0, 4, 0, 2, 0, 1 }; /*y start values*/
static const unsigned ADAM7_DX[7] = { 8, 8, 4, 4, 2, 2, 1 }; /*x delta values*/
static const unsigned ADAM7_DY[7] = { 8, 8, 8, 4, 4, 2, 2 }; /*y delta values*/

static void Adam7_getpassvalues(unsigned passw[7], unsigned passh[7]/*, size_t filter_passstart[8],
	size_t padded_passstart[8], size_t passstart[8]*/, unsigned w, unsigned h, unsigned bpp)
{
	/*the passstart values have 8 values: the 8th one indicates the byte after the end of the 7th (= last) pass*/
	unsigned i;

	/*calculate width and height in pixels of each pass*/
	for (i = 0; i != 7; ++i)
	{
		passw[i] = (w + ADAM7_DX[i] - ADAM7_IX[i] - 1) / ADAM7_DX[i];
		passh[i] = (h + ADAM7_DY[i] - ADAM7_IY[i] - 1) / ADAM7_DY[i];
		if (passw[i] == 0) passh[i] = 0;
		if (passh[i] == 0) passw[i] = 0;
	}

	//filter_passstart[0] = padded_passstart[0] = passstart[0] = 0;
	//for (i = 0; i != 7; ++i)
	//{
	//	/*if passw[i] is 0, it's 0 bytes, not 1 (no filtertype-byte)*/
	//	filter_passstart[i + 1] = filter_passstart[i]
	//		+ ((passw[i] && passh[i]) ? passh[i] * (1 + (passw[i] * bpp + 7) / 8) : 0);
	//	/*bits padded if needed to fill full byte at end of each scanline*/
	//	padded_passstart[i + 1] = padded_passstart[i] + passh[i] * ((passw[i] * bpp + 7) / 8);
	//	/*only padded at end of reduced image*/
	//	passstart[i + 1] = passstart[i] + (passh[i] * passw[i] * bpp + 7) / 8;
	//}
}

unsigned int MakeCRC(unsigned int crc,unsigned char a)
{
	crc = (crc >> 4) ^ CRC_TABLE[(crc & 15) ^ (a & 15)];
	crc = (crc >> 4) ^ CRC_TABLE[(crc & 15) ^ (a >> 4)];
	return crc;
}
unsigned int MakeCRC(unsigned int crc, const unsigned char* data, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		crc = MakeCRC(crc, data[i]);
	}
	return crc;
}
unsigned int MakeCRC(const unsigned char* data, int nSize)
{
	unsigned int crc = 0xFFFFFFFF;
	return MakeCRC(crc, data, nSize);
}


GsPNGPalette::GsPNGPalette()
{
	Size = 0;
	WithAlpha = false;
}


void GsPNG::Put8(unsigned char a)
{
	OnChunkData(&a, 1);
	m_CRC = MakeCRC(m_CRC, a);
}

void GsPNG::Put32(unsigned int v)
{
	Put8((v >> 24) & 0xFF);
	Put8((v >> 16) & 0xFF);
	Put8((v >> 8) & 0xFF);
	Put8(v & 0xFF);
}
void GsPNG::BeginChunk(const char* id, int len)
{
	Put32(len);
	m_CRC = 0xFFFFFFFF;
	Put8(id[0]);
	Put8(id[1]);
	Put8(id[2]);
	Put8(id[3]);
}

void GsPNG::EndChunk()
{
	Put32(~m_CRC);
}


void GsPNG::OnWrite(const unsigned char* data, int nLen)
{
	BeginChunk("IDAT", nLen);
	for (int i = 0; i < nLen; i++)
		Put8(data[i]);
	EndChunk();
			
}
void GsPNG::OnChunkData(const unsigned char* data, int nSize)
{
	OnPNGData((unsigned char*)data, nSize);
}
	
bool GsPNG::DirectCopyType()
{
	if (m_Type >= eGrayScale1 && m_Type <= eRGB8) return true;
	if (m_Type == eRGB16) return true;
	if (m_Type >= ePalette1 && m_Type <= ePalette8) return true;
	if (m_Type == eRGBA8) return true;
	if (m_Type == eRGBA16) return true;
	return false;
}
GsPNG::GsPNG()
{
	m_Row = 0;
	m_W = 0;
	m_H = 0;
	m_ScanCount = 1;
	m_ReadRGBA8RowCount = 0;
	m_Depth = 0;
	m_pGamma = nullptr;
	m_pChrm = nullptr;
	m_pBackground = nullptr;
	m_pPhys = nullptr;
}
GsPNG::GsPNG(int w, int h, GsPNGColorType type) :m_W(w), m_H(h)
{ 
	m_Type = type;
	m_Row = 0;
	m_pGamma = nullptr;
	m_pChrm = nullptr;
	m_pBackground = nullptr;
	m_pPhys = nullptr;
}
GsPNG::~GsPNG()
{
	if (m_pGamma)
		delete m_pGamma;
	if (m_pChrm)
		delete m_pChrm;
	if (m_pBackground)
		delete m_pBackground;
	if (m_pPhys)
		delete m_pPhys;
}
void GsPNG::Error(ErrorCode code)
{
	m_Error = code;
	m_ErrorMessage.clear();
}
void GsPNG::Error(const char* msg)
{
	m_ErrorMessage.clear();
	m_Error = eGeneralError;
	if (msg) m_ErrorMessage = msg;
}
void GsPNG::Succeed()
{

	m_Error = eGeneralError;
	m_ErrorMessage.clear();
}
GsPNG::ErrorCode GsPNG::LastErrorCode()
{
	return m_Error;
}
const char* GsPNG::LastErrorMessage()
{
	if (m_ErrorMessage.empty())
		return NULL;
	return m_ErrorMessage.c_str();
}


int GsPNG::Stride(GsPNGColorType type, int width)
{
	switch (type)
	{
	case eGrayScale1:
		return ceil(width / 8.0);
	case eGrayScale2:
		return ceil(width / 4.0);
	case eGrayScale4:
		return ceil(width  / 2.0);
	case eGrayScale8:
		return width;
	case eGrayScale16:
		return width * 2;
	
	case eRGB8:
	case eBGR8:
		return width * 3;
	case eRGB16:
	case eBGR16:
		return width * 6;
	case ePalette1:
		return ceil(width / 8.0);
	case ePalette2:
		return ceil(width / 4.0);
	case ePalette4:
		return ceil(width / 2.0);
	case ePalette8:
		return width;
	case eAlphaGrayScale8:
		return width * 2;
	case eAlphaGrayScale16:
		return width * 4;
	case eARGB8:
	case eRGBA8:
	case eABGR8:
	case eBGRA8:
		return width * 4;
	case eARGB16:
	case eRGBA16:
	case eABGR16:
	case eBGRA16:
		return width * 8;
	}
	return 0;
}
int GsPNG::Height()
{
	return m_H;
}
int GsPNG::Width()
{
	return m_W;
}

int GsPNG::Stride()
{
	return Stride(m_Type, m_W);
}
GsPNGColorType  GsPNG::ColorType()
{
	return m_Type;
}
//调色板
GsPNGPalette& GsPNG::Palette()
{
	return m_Palette;
}
		

bool GsPNG::BeginSave()
{
	//如果调色板为空颜色类型为调色板则无法开始。
	if (m_Palette.Size == 0)
	{
		if (m_Type >= ePalette1 && m_Type <= ePalette8)
			return false;
	}
	while (m_Buffer.BufferSize() <= Stride())
		m_Buffer.Allocate(m_Buffer.BufferSize() * 2);
	
	int n = OnPNGData((unsigned char*)"\211PNG\r\n\032\n", 8);
	if (n < 0)
		return false;
	BeginChunk( "IHDR", 13);
	Put32(m_W);
	Put32(m_H);
			
	/*PNG image				|type	|Colour type			|Allowed bit depths	Interpretation
	Greyscale				|0		|1, 2, 4, 8, 16			|Each pixel is a greyscale sample
	Truecolour				|2		|8, 16					|Each pixel is an R, G, B triple
	Indexed - colour		|3		|1, 2, 4, 8				|Each pixel is a palette index; a PLTE chunk shall appear.
	Greyscale with alpha	|4		|8, 16					|Each pixel is a greyscale sample followed by an alpha sample.
	Truecolour with alpha	|6		|8, 16					|Each pixel is an R, G, B triple followed by an alpha sample.
	*/
	int type = 0;
	int depth = 0;
	switch (m_Type)
	{
	case eGrayScale1:
		type = 0;
		depth = 1;
		break;
	case eGrayScale2:
		type = 0;
		depth =2;
		break;
	case eGrayScale4:
		type = 0;
		depth = 4;
		break;
	case eGrayScale8:
		type = 0;
		depth = 8;
		break;
	case eGrayScale16:
		type = 0;
		depth = 16;
		break;
	case eRGB8:
	case eBGR8:
		type = 2;
		depth = 8;
		break;
	case eRGB16:
	case eBGR16:
		type = 2;
		depth = 16;
		break;
	case ePalette1:
		type = 3;
		depth = 1;
		break;
				 
	case ePalette2:
		type = 3;
		depth = 2;
		break;
	case ePalette4:
		type = 3;
		depth = 4;
		break;
	case ePalette8:
		type = 3;
		depth = 8;
		break;
	case eARGB8:
	case eRGBA8:
	case eABGR8:
	case eBGRA8:
		type = 6;
		depth = 8;
		break;
	case eARGB16:
	case eRGBA16:
	case eABGR16:
	case eBGRA16:
		type = 6;
		depth = 16;
		break;

	}
	Put8(depth); // bit depth
	Put8(type); // RGBA
	Put8(0); // compression (deflate)
	Put8(0); // filter (standard)
	Put8(0); // interlace off
	EndChunk();
	
	//如果是调色板颜色。
	if (type == 3)
	{
		if (m_Palette.Size > 256)
			m_Palette.Size = 256;
		BeginChunk("PLTE", m_Palette.Size * 3);
		unsigned char* pal = (unsigned char*)m_Palette.RGBAColor;
		for (int i = 0; i < m_Palette.Size; i+=4)
		{
			Put8(pal[i * 4]);    //R
			Put8(pal[i * 4] + 1);//G
			Put8(pal[i * 4] + 2);//B
		}
		EndChunk();

		//如果调色板有Alpha通道的话
		if (m_Palette.WithAlpha)
		{
			BeginChunk("tRNS", m_Palette.Size);
			unsigned char* pal = (unsigned char*)m_Palette.RGBAColor;
			for (int i = 0; i < m_Palette.Size; i += 4)
			{
				Put8(pal[i * 4] + 3);//A
			}

			EndChunk();
		}
	}

	//开始数据块的
	BeginDeflate(eCompressionLevel9);
			
	m_Cache.Allocate(0);
	unsigned char b = 0;
	m_Cache.Append(&b,1);
	return true;
}
int GsPNG::Row()
{
	return m_Row;
}
void GsPNG::SaveRow(const unsigned char* row)
{
	m_Row++;
	m_Cache.Allocate(Stride() + 1);
	
	CopyRow(row, m_Cache.Ptr() + 1,Stride());
	Write(m_Cache.Ptr(), m_Cache.BufferSize());
}
#pragma region COLOR 

template<class T, const int A, const int B, const int C>
struct Color3
{
	Color3<T, A, B, C>& operator =(const Color3<T, A,B,C>& rhs)
	{
		Byte[0] = rhs.Byte[A];
		Byte[1] = rhs.Byte[B];
		Byte[2] = rhs.Byte[C];
		return *this;
	}

	T Byte[3];
};

template<class T,const int A, const int B, const int C, const int D>
struct Color4
{
	Color4<T, A, B, C, D>& operator =(const Color4<T, A,B,C,D>& rhs)
	{
		Byte[0] = rhs.Byte[A];
		Byte[1] = rhs.Byte[B];
		Byte[2] = rhs.Byte[C];
		Byte[3] = rhs.Byte[D];
		return *this;
	}
	template<class U,const int X,const int Y,const int Z>
	Color4<T, A, B, C, D>& operator  =(const Color3<U, X,Y, Z>& rhs)
	{
		Byte[0] = rhs.Byte[X];
		Byte[1] = rhs.Byte[Y];
		Byte[2] = rhs.Byte[Z];
		return *this;
	}

	T Byte[4];
};


template<class T>
void TemplateCopy(const unsigned char* from, unsigned char* to, int nStride)
{
	const T* f = (const T*)from;
	T* t = (T*)to;
	std::copy(f, f + nStride / sizeof(T), t);
}

template<class F, class T>
void TemplateCopy(const unsigned char* from, unsigned char* to, int nStride)
{
	const F* f = (const F*)from;
	T* t = (T*)to;
	std::copy(f, f + nStride / sizeof(F), t);
}

#pragma endregion

void GsPNG::CopyRow(const unsigned char* row, unsigned char* cache, int nStride)
{
	//如果直接copy
	if (DirectCopyType())
	{
		memcpy(cache, row, nStride);
		return;
	}
	switch (m_Type)
	{
	case eBGR8:
		TemplateCopy<Color3<unsigned char, 2,1,0> >(row, cache, nStride);
		break;
	case eBGR16:
		TemplateCopy<Color3<unsigned short, 2,1,0> >(row, cache, nStride);
		break;
	case eARGB8:
		TemplateCopy<Color4<unsigned char, 1,2,3,0> >(row, cache, nStride);
		break;
	case eABGR8:
		TemplateCopy<Color4<unsigned char,3,2,1,0> >(row, cache, nStride);
		break;
	case eBGRA8:
		TemplateCopy<Color4<unsigned char,2,1,0,3> >(row, cache, nStride);
		break;
	case eARGB16:
		TemplateCopy<Color4<unsigned short,1,2,3,0>  >(row, cache, nStride);
		break;
	case eABGR16:
		TemplateCopy<Color4<unsigned short, 3,2,1,0> >(row, cache, nStride);
		break;
	case eBGRA16:
		TemplateCopy<Color4<unsigned short,2,1,0,3> >(row, cache, nStride);
		break;
	}
	 
} 
 
static unsigned char paethPredictor(short a, short b, short c)
{
	short pa = abs(b - c);
	short pb = abs(a - c);
	short pc = abs(a + b - c - c);

	if (pc < pa && pc < pb) return (unsigned char)c;
	else if (pb < pa) return (unsigned char)b;
	else return (unsigned char)a;
}
int Bpp(GsPNGColorType type)
{
	switch(type)
	{
		case eGrayScale1:
		return 1;
		case eGrayScale2 :
			return 2;
		case  eGrayScale4 :
			return 4;
		case  eGrayScale8 :
			return 8;
		case  eGrayScale16 :
			return 16;
		case  eRGB8 :
		case  eBGR8 :
			return 24;
		case  eRGB16 :
		case  eBGR16 :
			return 48;
		case  ePalette1 :
			return 1;
		case  ePalette2 :
			return 2;
		case  ePalette4 :
			return 4;
		case  ePalette8 :
			return 8;
		case  eAlphaGrayScale8 :
			return 16;
		case  eAlphaGrayScale16 :
			return 32;
		case  eARGB8 :
		case  eRGBA8 :
		case  eABGR8 :
		case  eBGRA8 :
			return 32;
		case  eARGB16 :
		case  eRGBA16 :
		case  eABGR16 :
		case  eBGRA16 :
			return 64;

	}
	return 0;
}
int ByteWidth(int bpp )
{
	return (  bpp + 7) / 8;
}
//此算法应该只与通道个数相关
void GsPNG::Filtering(FilterTypes etype, unsigned char * row, int length)
{
	int bpp = Bpp(m_Type);
	/*if (bpp != 24 && bpp != 32)
		return;*/
	int bytewidth = ByteWidth(bpp);

	m_CacheBefore.Allocate(length); 
	unsigned char * precon = m_CacheBefore.Ptr();
	if (m_Row == 0)
		precon = NULL;
	if (!precon && etype == eFTPaeth)
		etype = eFTSub;

	unsigned char * recon = row;
	unsigned char * scanline = row;

	switch (etype)
	{
		//原值
		case eFTNone:break;
		case eFTSub:
		for (int i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
		for (int i = bytewidth; i < length; ++i) recon[i] = scanline[i] + recon[i - bytewidth];
		break; 
		
		break;
		case eFTUp: 
			if (precon)
				for (int i = 0; i != length; ++i) recon[i] = scanline[i] + precon[i];
			else
				for (int i = 0; i != length; ++i) recon[i] = scanline[i];
			break;
		case eFTAverage:
			if (precon)
			{
				for (int i = 0; i != bytewidth; ++i) recon[i] = scanline[i] + (precon[i] >> 1);
				for (int i = bytewidth; i < length; ++i) recon[i] = scanline[i] + ((recon[i - bytewidth] + precon[i]) >> 1);
			}
			else
			{
				for (int i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
				for (int i = bytewidth; i < length; ++i) recon[i] = scanline[i] + (recon[i - bytewidth] >> 1);
			}
			break;
		break;
		case eFTPaeth:
			if (precon)
			{
				for (int i = 0; i != bytewidth; ++i)
					recon[i] = (scanline[i] + precon[i]); /*paethPredictor(0, precon[i], 0) is always precon[i]*/
				for (int i = bytewidth; i < length; ++i)
					recon[i] = (scanline[i] + paethPredictor(recon[i - bytewidth], precon[i], precon[i - bytewidth]));
				
			}
			else
			{
				for (int i = 0; i != bytewidth; ++i)
					recon[i] = scanline[i];
				for (int i = bytewidth; i < length; ++i)
					/*paethPredictor(recon[i - bytewidth], 0, 0) is always recon[i - bytewidth]*/
					recon[i] = (scanline[i] + recon[i - bytewidth]);
				
			}
			break;
	}
	//记录一下，下次取BC的值
	m_CacheBefore.Copy(row, length);
}

void GsPNG::FinishSave()
{	
	End();
	BeginChunk("IEND", 0);
	EndChunk();
}
	 
const unsigned char *GsPNG::Read(int& nSize)
{
	m_Cache.Allocate(nSize);
	nSize = OnPNGData(m_Cache.Ptr(), nSize, false);
	m_Cache.Allocate(nSize);
	if (nSize <= 0)
		return NULL;
	return m_Cache.Ptr();
}
bool GsPNG::IsPNGSignature(const unsigned char* head)
{
	static unsigned char sign[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	//比较
	for (int i = 0; i < 8; i++)
		if (head[i] != sign[i])
			return false;
	return true;
}
unsigned int Read32(const unsigned char* buf)
{
	return ((unsigned int)((*(buf)) & 0xff) << 24) +
		((unsigned int)((*(buf + 1)) & 0xff) << 16) +
		((unsigned int)((*(buf + 2)) & 0xff) << 8) +
		((unsigned int)((*(buf + 3)) & 0xff));
}
GsPNGColorType ToColorType(int depth, unsigned char cType)
{
	if (cType == 0)
	{
		switch (depth)
		{
		case 1:
			return eGrayScale1;
		case 2:
			return eGrayScale2;
		case 4:
			return eGrayScale4;
		case 8:
			return eGrayScale8;
		case 16:
			return eGrayScale16;
		}
		return eUnknownPNGColorType;
	}
	
	if (cType == 2)
	{
		switch (depth)
		{
		case 8:
			return eRGB8;
		case 16:
			return eRGB16;
		}
		return eUnknownPNGColorType;
	}
	if (cType == 3)
	{
		switch (depth)
		{
		case 1:
			return ePalette1;
		case 2:
			return ePalette2;
		case 4:
			return ePalette4;
		case 8:
			return ePalette8;
		}
		return eUnknownPNGColorType;
	}

	if (cType == 4)
	{
		switch (depth)
		{
		case 8:
			return eAlphaGrayScale8;
		case 16:
			return eAlphaGrayScale16;
		}
		return eUnknownPNGColorType;
	}
	if (cType == 6)
	{
		switch (depth)
		{ 
		case 8:
			return eRGBA8;
		case 16:
			return eRGBA16;
		}
		return eUnknownPNGColorType;
	}
	return eUnknownPNGColorType;
}
GsPNG::ChunkType GsPNG::ReadChunkHead(unsigned int& crc, unsigned int& length)
{
	int size = 8;
	const unsigned char* pData = Read(size);
	if (!pData || size != 8)
		return eErrorChunk;

	//读取chunk的长度
	length = Read32(pData);
	m_ChunkSize = length;
	//将头加入校验
	crc = MakeCRC(pData + 4, 4);
	const char* name = (const char*)(pData + 4);

	if (strncmp(name, "IHDR",4) == 0 )
		return eIHDR;
	if (strncmp(name, "PLTE", 4) == 0 )
		return ePLTE;
	if (strncmp(name, "IEND", 4) == 0 )
		return ePLTE;
	if (strncmp(name, "IDAT", 4) == 0 )
		return eIDAT;

	if (strncmp(name, "cHRM", 4) == 0 )
		return ecHRM;
	if (strncmp(name, "gAMA", 4) == 0 )
		return egAMA;
	if (strncmp(name, "iCCP", 4) == 0 )
		return eiCCP;
	if (strncmp(name, "sBIT", 4) == 0 )
		return esBIT;
	if (strncmp(name, "sRGB", 4) == 0 )
		return esRGB;
	if (strncmp(name, "bKGD", 4) == 0 )
		return ebKGD;
	if (strncmp(name, "hIST", 4) == 0 )
		return ehIST;
	if (strncmp(name, "tRNS", 4) == 0 )
		return etRNS;
	if (strncmp(name, "pHYs", 4) == 0 )
		return epHYs;
	if (strncmp(name, "pPLT", 4) == 0 )
		return epPLT;
	if (strncmp(name, "tIME", 4) == 0 )
		return etIME;
	if (strncmp(name, "iTXt", 4) == 0 )
		return eiTXt;
	if (strncmp(name, "tEXt", 4) == 0 )
		return etEXt;
	if (strncmp(name, "zTXt",4) == 0 )
		return ezTXt;
	return eErrorChunk;
}
const unsigned char* GsPNG::ReadChunkData(unsigned int crc, unsigned int length, bool &crcOK)
{
	int size = 8;
	//读取头和CRC
	size = length + 4;
	const unsigned char* pData = Read(size);
	if (!pData || size != length + 4)
		return NULL;

	crc = MakeCRC(crc, pData, length);
	
	//校验CRC
	crcOK = (~crc == Read32(pData + length));

	m_ChunkSize -= size - 4;
	return pData;
}

int GsPNG::ScanCount()
{
	return m_ScanCount;
}

bool GsPNG::BeginRead()
{
	int size = 8;
	const unsigned char* pData = Read(size);
	if (!pData || size != 8)
		return Error("length of the data is not enough", false);

	//如果文件头不是PNG的签名则不干活
	if (!IsPNGSignature(pData))
		return Error("this is not a png file format", false);
	

	unsigned int length;
	ChunkType eChunkType = ReadChunkHead(m_CRC, length);
	if (eChunkType != eIHDR)
		return Error("chunk postion is error,IHDR chunk is not the first ",false);
	if (length != 13)
		return Error("length of IHDR chunk  is error", false);

	bool crcOK;
	pData = ReadChunkData(m_CRC, length, crcOK);
	if(!crcOK) return Error("check crc of IHDR  faild", false);
	if(!pData) return Error("read data of IHDR  faild", false);

	/*
	Width				4 bytes
	Height				4 bytes
	Bit depth			1 byte
	Colour type			1 byte
	Compression method	1 byte
	Filter method		1 byte
	Interlace method	1 byte
	*/
	m_W = Read32(pData); pData += 4;
	m_H = Read32(pData); pData += 4;
	m_Depth = *pData++;
	unsigned char cType = *pData++;
	
	m_Type = ToColorType(m_Depth, cType);

	unsigned char compress = *pData++;//压缩
	unsigned char filter = *pData++;//过滤
	unsigned char inter = *pData++;//是否隔行扫描
	if (inter)
	{
		int npp = Bpp(m_Type);
		if (npp < 0)
			return Succeed(false);
		m_ScanCount = 7;
		m_vPassW.resize(7, 0);
		m_vPassH.resize(7, 0);
		//m_vFilterPassStart.resize(8, 0);
		//m_vPaddedPassStart.resize(8, 0);
		//m_vPassStart.resize(8, 0);
		Adam7_getpassvalues(&m_vPassW[0], &m_vPassH[0], /*&m_vFilterPassStart[0], &m_vPaddedPassStart[0], &m_vPassStart[0],*/ m_W, m_H, npp);
		m_Cachel7.Allocate(m_vPassW[6] * npp / m_Depth);
	}

	while (true)
	{
		eChunkType = ReadChunkHead(m_CRC, length);
		if(eChunkType == eErrorChunk)
			return Error("read error chunk type", false);
		if (eChunkType == eIEND)
		{
			m_Cache.Allocate(0);
			return Succeed(true);
		}
		else if (eChunkType == eIDAT)
		{
			//设置zlib的缓冲区是一行的数据长度+1
			CacheSize(Stride() + 1);
			BeginInflate();
			return Succeed(true);
		}
		pData = ReadChunkData(m_CRC, length, crcOK);
		if(!crcOK)  return Error("check crc of chunk  faild", false);

		if(eChunkType == ePLTE)
		{
			m_Palette.Size = length / 3;
			memset(m_Palette.RGBAColor, 0xff, 256 * 4);
			//将颜色值拷贝过来
			for (int i = 0; i < m_Palette.Size; i++)
			{
				memcpy(&m_Palette.RGBAColor[i], pData, 3);
				pData += 3;
			}
			
		}
		else if (eChunkType == etRNS)//透明通道。
		{
			//透明通道必须在调色板之后
			if (m_Palette.Size == 0)
				return Error("tRNS chunk is not after PLTE chunk", false);
			
			m_Palette.WithAlpha = true;
			length = length < m_Palette.Size ? length : m_Palette.Size;
			for (int i = 0; i <length; i++)
			{
				unsigned char* p = (unsigned char*)&m_Palette.RGBAColor[i];
				p[3]=*pData++;

			}
		}
		else if (eChunkType == egAMA)
		{
			m_pGamma = new unsigned int(16777216u * pData[0] + 65536u * pData[1] + 256u * pData[2] + pData[3]);
		}
		else if (eChunkType == ecHRM)
		{
			m_pChrm = new GsChrm;
			m_pChrm->chrm_white_x = 16777216u * pData[0] + 65536u * pData[1] + 256u * pData[2] + pData[3];
			m_pChrm->chrm_white_y = 16777216u * pData[4] + 65536u * pData[5] + 256u * pData[6] + pData[7];
			m_pChrm->chrm_red_x = 16777216u * pData[8] + 65536u * pData[9] + 256u * pData[10] + pData[11];
			m_pChrm->chrm_red_y = 16777216u * pData[12] + 65536u * pData[13] + 256u * pData[14] + pData[15];
			m_pChrm->chrm_green_x = 16777216u * pData[16] + 65536u * pData[17] + 256u * pData[18] + pData[19];
			m_pChrm->chrm_green_y = 16777216u * pData[20] + 65536u * pData[21] + 256u * pData[22] + pData[23];
			m_pChrm->chrm_blue_x = 16777216u * pData[24] + 65536u * pData[25] + 256u * pData[26] + pData[27];
			m_pChrm->chrm_blue_y = 16777216u * pData[28] + 65536u * pData[29] + 256u * pData[30] + pData[31];
		}
		else if (eChunkType == ebKGD)
		{
			m_pBackground = new GsPNGBackground;
			if (cType == 3)
			{
				if (length != 1)
					return Error("bKGD length is not 1", false);
				if (pData[0] >= m_Palette.Size)
					return Error("bKGD[0] is be equal or greater than palette size", false);
				m_pBackground->background_r = m_pBackground->background_g = m_pBackground->background_b = pData[0];
			}
			else if (cType == 0 || cType == 4)
			{
				if (length != 2)
					return Error("bKGD length is not 2", false);
				m_pBackground->background_r = m_pBackground->background_g = m_pBackground->background_b = 256u * pData[0] + pData[1];
			}
			else if (cType == 2 || cType == 6)
			{
				if (length != 6)
					return Error("bKGD length is not 6", false);
				m_pBackground->background_r = 256u * pData[0] + pData[1];
				m_pBackground->background_g = 256u * pData[2] + pData[3];
				m_pBackground->background_b = 256u * pData[4] + pData[5];
			}
		}
		else if (eChunkType == epHYs)
		{
			if (length != 9)
				return Error("pHYs length is not 9", false);
			m_pPhys = new GsPhys;
			m_pPhys->phys_x = 16777216u * pData[0] + 65536u * pData[1] + 256u * pData[2] + pData[3];
			m_pPhys->phys_y = 16777216u * pData[4] + 65536u * pData[5] + 256u * pData[6] + pData[7];
			m_pPhys->phys_unit = pData[8];
		}


	}
	return true;
}
const unsigned char* GsPNG::OnNextInputBuffer(unsigned int& size)
{
	size = 0;
	//如果是第一行
	if (m_ChunkSize >0 )
	{
		size = m_ChunkSize;
		bool crcOK;
		const unsigned char* data = ReadChunkData(m_CRC, size, crcOK);
		if (!crcOK)
		{
			Error("check crc of IDAT chunk faild");
			return NULL;
		}
		return data;
	}
	
	//读取下一个chunk，如果读取的不是idat那么就不干活
	ChunkType eChunkType  = ReadChunkHead(m_CRC, size);
	if (eChunkType != eIDAT)
	{
		size = 0;
		return NULL;
	}
	
	bool crcOK;
	const unsigned char* data = ReadChunkData(m_CRC, size, crcOK);
	if (!crcOK)
	{
		Error("check crc of IDAT chunk faild");
		return NULL;
	}
	return data;
}
unsigned short BitsFromBlob(const unsigned char* head, int nOffset, int nLen)
{
	const unsigned short *h = (const unsigned short *)head;
	int part = 16 / nLen;
	std::bitset<16> *set = (std::bitset<16> *)(h + (nOffset / part));
	int a = (nOffset * 16) / part;
	a = a % 16;

	unsigned short val = 0;
	std::bitset<16> *s = (std::bitset<16> *)&val;
	for (int i = 0; i < nLen; i++)
	{
		s->set(i, set->test(a + i));
	}
	return val;
}
/// \brief 解码一行,以RGBA8的格式输出
bool GsPNG::ReadRGBA8Row(unsigned char* row)
{
	if (m_Type == eRGBA8)
		return ReadRow(row);

	
	m_Cache1.Allocate(Stride());
	if (!ReadRow(m_Cache1.Ptr()))
		return false;
	
	memset(row, 0xff, Width() * 4);
	unsigned char* head = m_Cache1.Ptr();

	//索引色
	if (m_Type >= eGrayScale1 && m_Type <= eGrayScale16)
	{
		int bit = 1 << (m_Type - eGrayScale1);
		double scale = 255.0 / ((1 << bit) - 1);
		for (int i = 0; i < m_W; i++)
		{;
			memset(row, scale * BitsFromBlob(head, i, bit), 3);
			row += 4;
		}
		return true;
	}

	//从RGB到RGBA
	if (m_Type == eRGB8)
	{
		TemplateCopy<Color3<unsigned char, 0, 1, 2>, Color4<unsigned char, 0, 1, 2, 3> >(head, row, m_W * 3);
		return true;
	}

	//从BGR到RGBA
	if (m_Type == eBGR8)
	{
		TemplateCopy< Color3<unsigned char, 2, 1, 0> , Color4<unsigned char, 0, 1, 2, 3> >(head, row, m_W * 3);

		return true;
	}

	//RGB16颜色
	if (m_Type == eRGB16 || m_Type == eBGR16)
	{
		int a = 0, b = 2;
		if (m_Type == eBGR16){ a = 2;b = 0;}

		unsigned short* s = (unsigned short*)head;
		for (int i = 0; i < m_W; i++)
		{
			row[0] = s[a] / 257;
			row[1] = s[1] / 257;
			row[2] = s[b] / 257;
			
			s += 3;
			row += 4;
		}
		return true;
	}

	if (m_Type >= ePalette1 && m_Type <= ePalette8)
	{ 
		int bit = 1 << (m_Type - ePalette1);
		for (int i = 0; i < m_W; i++)
		{
			unsigned short v = BitsFromBlob(head, i, bit);
			memcpy(row, &m_Palette.RGBAColor[v], 4); 
			row += 4;
		}
	}


	if (m_Type == eBGRA8)
	{
		TemplateCopy<Color4<unsigned char, 2, 1, 0, 3> >(head, row, m_W * 4);
		return true;
	}

	if (m_Type == eABGR8)
	{
		TemplateCopy<Color4<unsigned char, 3, 2, 1, 0> >(head, row, m_W * 4);
		return true;
	}

	if (m_Type == eARGB8)
	{
		TemplateCopy<Color4<unsigned char, 1, 2, 3, 0> >(head, row, m_W * 4);
		return true;
	}

	if (m_Type == eAlphaGrayScale8)
	{
		for (int i = 0; i < Width(); ++i, row += 4)
		{
			row[0] = row[1] = row[2] = head[i * 2 + 0];
			row[3] = head[i * 2 + 1];
		}
		return true;
	}
	if (m_Type == eAlphaGrayScale16)
	{
		for (int i = 0; i != Width(); ++i, row += 4) {
			row[0] = row[1] = row[2] = head[i * 4 + 0];
			row[3] = head[i * 4 + 2];
		}
		return true;
	}

	int r = 0, g = 1, b = 2, a = 3;
	switch (m_Type)
	{
	case eARGB16:
		r = 1; g = 2; b = 3; a = 0;
		break;
	case eRGBA16:
		break;
	case eABGR16:
		r = 3; g = 2; b = 1; a = 0;
		break;
	case eBGRA16:
		r = 1; g = 2; b = 3; a = 0;
		break;
	default:
		return false;
	}

	unsigned short* s = (unsigned short*)head;
	for (int i = 0; i < m_W; i++)
	{
		row[0] = s[r] / 257;
		row[1] = s[g] / 257;
		row[2] = s[b] / 257;
		row[3] = s[a] / 257;

		s += 4;
		row += 4;
	}
	return true;
}
bool GsPNG::ReadRow(unsigned char* row)
{
	int nScanCount = m_ReadRGBA8RowCount / m_H, nRow = m_ReadRGBA8RowCount % m_H;
	if (m_ScanCount <= nScanCount)
		return false;
	++m_ReadRGBA8RowCount;
	int nRowLen = 0;
	unsigned char filter;
	if (m_ScanCount == 7)
	{
		size_t pixeloutstart = 0;
		size_t bytewidth = Bpp(m_Type) / m_Depth;
		int y = 0;
		bool bFound = false;
		for (y = 0; y < m_vPassH[nScanCount]; ++y)
		{
			for (int x = 0; x < m_vPassW[nScanCount]; ++x)
			{
				pixeloutstart = ((ADAM7_IY[nScanCount] + y * ADAM7_DY[nScanCount]) * m_W + ADAM7_IX[nScanCount] + x * ADAM7_DX[nScanCount]) * bytewidth;
				if (pixeloutstart / Stride() == nRow)
				{
					bFound = true;
					break;
				}
			}
			if (bFound)
				break;
		}
		if (!bFound)
			return true;
		m_Row = y;
		nRowLen = m_vPassW[nScanCount] * bytewidth;
		InflateSome(&filter, 1);
		if (InflateSome(m_Cachel7.Ptr(), nRowLen) != nRowLen)
			return Error("png data may be damaged", false);
		Filtering((FilterTypes)filter, m_Cachel7.Ptr(), nRowLen);
		unsigned char* tmp = m_Cachel7.Ptr();
		for (int x = 0; x < m_vPassW[nScanCount]; ++x)
		{
			pixeloutstart = ((ADAM7_IY[nScanCount] + y * ADAM7_DY[nScanCount]) * m_W + ADAM7_IX[nScanCount] + x * ADAM7_DX[nScanCount]) * bytewidth;
			pixeloutstart %= Stride();
			memcpy((void*)(row + pixeloutstart), tmp, bytewidth);
			tmp += bytewidth;
		}
	}
	else
	{
		if (m_Cache.BufferSize() == 0)
			return Error("png data may be damaged", false);
		if (m_Row >= m_H)
			return Error("read rows too much", false);

		//解压行的第一个字节，这个是filter，未来使用现在直接丢弃。
		InflateSome(&filter, 1);//这里有可能是 0，1，2，4
								//解压一行
		nRowLen = Stride();
		int n = InflateSome(row, nRowLen);
		//读取的长度不足的话证明解压不下去了
		if (n != Stride())
			return Error("png data may be damaged", false);

		Filtering((FilterTypes)filter, row, nRowLen);
		m_Row++;
	}
	
	return true;
}
void GsPNG::FinishRead()
{
	//剩下的都丢弃。

}

/// \brief 从内存块解码
GsMemoryPNG::GsMemoryPNG(const unsigned char* buff, int nSize)
{
	m_buff = buff;
	m_nSize = nSize;
	m_Offset = 0;
	m_pBuffer = NULL;
}
/// \brief 从内存块解码
GsMemoryPNG::GsMemoryPNG(GsByteBuffer* buff)
{
	m_buff = NULL;
	m_nSize = 0;
	m_Offset = 0;
	m_pBuffer = buff;
}

/// \brief 从内存块编码
GsMemoryPNG::GsMemoryPNG(GsByteBuffer* buff, int w, int h, GsPNGColorType type ):GsPNG(w,h,type)
{
	m_buff = NULL;
	m_nSize = 0;
	m_Offset = 0; 
	m_pBuffer = buff;

} 
/// \brief 编码到内存
GsMemoryPNG::GsMemoryPNG(int w, int h, GsPNGColorType type ):GsPNG(w,h,type)
{
	m_buff = NULL;
	m_nSize = 0;
	m_Offset = 0;
	m_pBuffer = &m_Buffer;
}
int GsMemoryPNG::OnPNGData(unsigned char* data, int nLen, bool bWrite)
{
	//如果是写入则直接写入到ByteBuffer中。
	if (bWrite)
	{
		if (!m_pBuffer)
			return 0;
		m_pBuffer->Append(data, nLen);
		return nLen;
	}

	const unsigned char* head = m_buff;
	int size = m_nSize;
	if (m_pBuffer)
	{
		head = m_pBuffer->BufferHead();
		size = m_pBuffer->BufferSize();
	}
	//避免读取超限
	int nRead = GsMath::Min(nLen, size - m_Offset);
	//拷贝数据
	memcpy(data, head + m_Offset, nRead);
	//offset
	m_Offset += nRead;

	return nRead;

}
GsMemoryPNG::~GsMemoryPNG()
{

}

/// \brief 获取编码用的内存
GsByteBuffer* GsMemoryPNG::MemoryBuffer()
{
	return m_pBuffer;
}




//写入字节
int GsFilePNG::OnPNGData(unsigned char* data, int nLen, bool bWrite)
{
	if (!m_F)
		return -1;
	if (bWrite)
	{
		fwrite(data, nLen, 1, m_F);
		return nLen;
	}
	return fread(data, 1, nLen, m_F);
}

GsFilePNG::GsFilePNG(const char* file)
{
	m_F = fopen(file, "rb");

} 
GsFilePNG::GsFilePNG(const char* file, int w, int h,GsPNGColorType type) :GsPNG(w, h, type)
{
	m_F = fopen(file, "wb");
}
GsFilePNG::~GsFilePNG()
{
	if (!m_F)
		return;
	fclose(m_F);
}
