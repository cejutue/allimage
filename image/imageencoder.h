#pragma once
#include <memorymanager.h>
#include <image.h>
#include <iostream>
#include <fstream> 
#include <vector>
 
 
#include "jpeg/jpge.h"
#include "jpeg/jpgd.h"
 
//影像IO
class ImageIO
{
public:
	virtual ~ImageIO(){}
	virtual unsigned int  Write(const unsigned char* pBlob,int nLen) = 0;
	virtual unsigned int Read(unsigned char* pBlob,int nLen) = 0;
	virtual bool Rewind()= 0;
	virtual bool Eof() = 0;
	UTILITY_NAME::GsImageEncodeType ImageType();
};
//读写文件的io实现
class ImageFile:public ImageIO
{
	std::fstream m_fStream;
	std::fstream::pos_type m_nLen;
public:
	ImageFile(const char* file,bool bIn = false);
	virtual unsigned int  Write(const unsigned char* pBlob,int nLen);
	virtual unsigned int Read(unsigned char* pBlob,int nLen);
	virtual bool Eof();
	virtual bool Rewind();
	bool IsOpen();
};
//读写内存中的数据的io实现
class ImageBuffer:public ImageIO
{
	UTILITY_NAME::GsByteBuffer* m_pBuffer;
	unsigned int  m_Index;
public:
	ImageBuffer(UTILITY_NAME::GsByteBuffer* pBuffer);
	virtual unsigned int Write(const unsigned char* pBlob,int nLen);
	virtual unsigned int Read(unsigned char* pBlob,int nLen);
	virtual bool Eof();
	virtual bool Rewind();
};
//内存中的只读io实现
class ReadImageBuffer:public ImageIO
{
	const unsigned char* m_Buff;
	unsigned int   m_nLen;
	unsigned int  m_Index;
public:
	ReadImageBuffer(const unsigned char* buff,int nLen);
	virtual unsigned int Write(const unsigned char* pBlob,int nLen);
	virtual unsigned int Read(unsigned char* pBlob,int nLen);
	virtual bool Eof();
	virtual bool Rewind();
};

//影像编码器。
class ImageEncoder
{
protected:
	ImageIO* m_ImageIO;
public:
	virtual ~ImageEncoder(){}
	virtual bool Begin(int w,int h,int nStride,int bpp) = 0;
	virtual bool WriteRow(const unsigned char* pRow,int nLen,int nRowCount =1) = 0;
	virtual void End() =0;
};

//32位颜色放问题。
class RGBAVisitor
{
public:
	virtual void Alloc(int w,int h) = 0;
	virtual unsigned char* Row(int i) =0;
};

class MemRGBAVisitor :public RGBAVisitor
{
	UTILITY_NAME::GsGrowByteBuffer* m_pBuffer;
	int* m_W, * m_H;
public:
	MemRGBAVisitor(UTILITY_NAME::GsGrowByteBuffer* pBuffer, int* w, int* h);
	virtual void Alloc(int w, int h);

	virtual unsigned char* Row(int i);
};

//影像解码器
class ImageDecoder
{
protected:
	char	m_szLastError[256];
	ImageIO* m_ImageIO;
	RGBAVisitor* m_pImageBuffer;
	int m_bPP; 
protected:
	ImageDecoder();
	virtual bool Begin(int w,int h,int nStride,int bpp);
	virtual bool WriteRow(const unsigned char* pRow,int nLen,int nRowIndex,unsigned char* pPalette,int bitdepth);
	virtual bool WriteRow(const unsigned char* pRow,int nLen,int nRowIndex);
	virtual void End();
public:
	void ImageBuffer(RGBAVisitor* pImageBuffer){m_pImageBuffer = pImageBuffer;}
};


 
//jpeg格式编码器实现。
class JpegImageEncoder:public ImageEncoder,jpge::output_stream
{ 
	std::vector<unsigned char> m_ScanLine;
	jpge::jpeg_encoder m_Encoder;
	jpge::params m_param;
    virtual bool put_buf(const void* Pbuf, int len);
public:
	JpegImageEncoder(ImageIO* io);

	void Quality(int nQuality);

	~JpegImageEncoder();
	virtual bool Begin(int w,int h,int nStride,int bpp);
	virtual bool WriteRow(const unsigned char* pRow,int nLen,int nRowCount =1);
	virtual void End();
};


class JpegImageDecoder:public ImageDecoder,jpgd::jpeg_decoder_stream
{
	virtual int read(jpgd::uint8 *pBuf, int max_bytes_to_read, bool *pEOF_flag);
public:
	JpegImageDecoder(ImageIO* pIO);
	virtual bool Decode();
	virtual bool Decode(UTILITY_NAME::GsImageHeaderInfo* info);
};

class BmpImageDecoder:public ImageDecoder
{
	// 用于记录解码的方式。如果info!=null 则只解析头
	UTILITY_NAME::GsImageHeaderInfo* m_info = nullptr;
public:
	BmpImageDecoder(ImageIO* pIO);
	virtual bool Decode();
	virtual bool Decode(UTILITY_NAME::GsImageHeaderInfo* info);
};

//png格式影像编码器。
class PNGImageEncoder:public ImageEncoder
{  
	bool m_IsLittleEndian;
public:
	PNGImageEncoder(ImageIO* io);
	~PNGImageEncoder();
	virtual bool Begin(int w,int h,int nStride,int bpp);
	virtual bool WriteRow(const unsigned char* pRow,int nLen,int nRowCount =1);
	virtual void End();

};
 
class PNGImageDecoder:public ImageDecoder
{
	 
public:
	PNGImageDecoder(ImageIO* pIO);
};


class STBImageDecoder :public ImageDecoder
{
	// 用于记录解码的方式。如果info!=null 则只解析头
	UTILITY_NAME::GsImageHeaderInfo* m_info = nullptr;
	ImageIO* m_io;
public:
	STBImageDecoder(ImageIO* io);
	bool Decode(int& w,int& h,int& bits, UTILITY_NAME::GsGrowByteBuffer* buff);
	virtual bool Decode(UTILITY_NAME::GsImageHeaderInfo* info);
};
class TIFFImageDecoder :public ImageDecoder
{ 
	// 用于记录解码的方式。如果info!=null 则只解析头
	UTILITY_NAME::GsImageHeaderInfo* m_info = nullptr;
	UTILITY_NAME::GsInputStreamPtr m_ptrStream;
public:
	TIFFImageDecoder(UTILITY_NAME::GsInputStream* pStream);
	bool Decode(int& w, int& h, int& bits, UTILITY_NAME::GsGrowByteBuffer* buff);

	virtual bool Decode(UTILITY_NAME::GsImageHeaderInfo* info);
};
#include "webp/src/webp/encode.h"

class WEBPImageEncoder : public ImageEncoder
{	
	WebPPicture m_Picture;
	WebPConfig m_Config;
	UTILITY_NAME::GsRGBAType m_inputType;
public:
	WEBPImageEncoder(ImageIO* io);


	~WEBPImageEncoder();

	void Quality(int nQuality);
	void Speed(int speed);
	void RGBAType(UTILITY_NAME::GsRGBAType inputType);

	// 通过 ImageEncoder 继承
	virtual bool Begin(int w, int h, int nStride, int bpp) override;
	virtual bool WriteRow(const unsigned char * pRow, int nLen, int nRowCount = 1) override;
	virtual void End() override;
};

class WEBPImageDecoder : public ImageDecoder
{
	unsigned char* m_Buff = 0;
	unsigned int   m_nLen = 0;	
	bool m_IsBuffOwn = false;
public :
	WEBPImageDecoder(const char *file);
	WEBPImageDecoder(const unsigned char* blob,int nLen );
	virtual ~WEBPImageDecoder();
	virtual bool Decode(int& w, int& h, int& bits, UTILITY_NAME::GsGrowByteBuffer* buff);

	virtual bool Decode(UTILITY_NAME::GsImageHeaderInfo* info);
};