#include "pch.h"
#include "imageencoder.h"
#include <functional>
#include <memory.h>
#include <mathhelp.h>
#include <filesystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
#include "ktx.h"
#include "vkformat_enum.h"
#include "vk_format.h"
using namespace UTILITY_NAME;
#ifdef ANDROID

#else
//// Public crnlib header.
#include "crnlib.h"
#define CRND_HEADER_FILE_ONLY
//CRN transcoder library.
#include "crn_decomp.h"
//.DDS file format definitions.
#include "dds_defs.h"
#include "crnlib/crn_core.h"
#include "crnlib/crn_console.h"

#include "crnlib/crn_colorized_console.h"

#include "crnlib/crn_find_files.h"
#include "crnlib/crn_file_utils.h"
#include "crnlib/crn_command_line_params.h"

#include "crnlib/crn_dxt.h"
#include "crnlib/crn_cfile_stream.h"
#include "crnlib/crn_texture_conversion.h"
#include "crnlib/crn_texture_comp.h"
#include "crnlib/crn_buffer_stream.h"
#endif // ANDROID


UTILITY_NS
int read_stbi(void *user, char *data, int size)    // fill 'data' with 'size' bytes.  return number of bytes actually read
{
	ImageIO* img = (ImageIO*)user;
	return img->Read((unsigned char*)data, size);
}
void skip_stbi(void *user, int n)                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
{
	ImageIO* img = (ImageIO*)user;
	unsigned char buff[1024];
	while (n > 0)
	{
		int s = GsMath::Min(1024, n);
		img->Read(buff, s);
		n -= s;
	}
}
int eof_stbi(void *user)                        // returns nonzero if we are at end of file/data
{
	ImageIO* img = (ImageIO*)user;
	return img->Eof();
}
UTILITY_ENDNS
GsImageEncodeType ImageIO::ImageType()
{
	// Determine the true image type using link:
    // http://en.wikipedia.org/wiki/List_of_file_signatures
	char buffer[12] = {0};
	if(Read((unsigned char*)buffer,12) < 12) return eUnknownImage;

	if (strncmp(buffer, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) == 0) 
        return ePNG;
    if(buffer[0]=='\xff' && buffer[1]=='\xd8' && buffer[2]=='\xff') 
        return eJPG;
	if (buffer[0] == '\x4D' && buffer[1] == '\x4D')
		return eTIFF;
	if (buffer[0] == '\x49' && buffer[1] == '\x49')
		return eTIFF;

    if(buffer[0]=='B' && buffer[1]=='M') 
        return eBMP;
    if(buffer[0]=='D' && buffer[1]=='N' && buffer[2] == 'G') 
		return eDNG;
	if (buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F')
		return eGIF;
	if (strncmp(buffer, "DDS\x20", 4) == 0)
		return eDDS;
	if (strncmp(buffer, "Hx", 2) == 0)
		return eCRN;

	if (strncmp(buffer, "\xAB\x4B\x54\x58\x20\x31\x31\xBB\x0D\x0A\x1A\x0A", 12) == 0)
		return eKTX;
	if (buffer[8] == 'W' && buffer[9] == 'E' && buffer[10] == 'B' && buffer[11] == 'P')
		return eWEBP;
	if (strncmp(buffer, "\xAB\x4B\x54\x58\x20\x32\x30\xBB\x0D\x0A\x1A\x0A", 12) == 0)
		return eKTX2;
	GsString f = buffer;
	stbi__context ctx;
	memset(&ctx, 0, sizeof(stbi__context));
	ctx.io_user_data = this;
	stbi_io_callbacks cb;
	cb.eof = eof_stbi;
	cb.read = read_stbi;
	cb.skip = skip_stbi;
	Rewind();
	
	stbi__start_callbacks(&ctx, &cb, this);
	if (stbi__tga_test(&ctx))
		return eTGA;

    return eUnknownImage;
}
ImageFile::ImageFile(const char* file,bool bIn)
{
	if(bIn)
	{
		m_fStream.open(file,std::ios::in|std::ios::binary);
		m_fStream.seekg(0,std::ios::end);
		m_nLen = m_fStream.tellg();
		m_fStream.seekg(0,std::ios::beg);
	}
	else
		m_fStream.open(file,std::ios::out|std::ios::binary);

}
bool ImageFile::Rewind()
{
	if (m_fStream.eof())
		m_fStream.clear(std::ios::eofbit);
	m_fStream.seekg(0,std::ios::beg);
	return true;
}
bool ImageFile::IsOpen()
{
	return m_fStream.is_open();
}
bool ImageFile::Eof()
{
	return m_fStream.eof();
}
unsigned int ImageFile::Read(unsigned char* pBlob,int nLen)
{
	std::fstream::pos_type pos  = m_fStream.tellg();
	if(NULL == pBlob)
		m_fStream.seekg(nLen,std::ios::cur);
	else
		m_fStream.read((char*)pBlob,nLen);
	if(m_fStream.eof())
		return m_nLen - pos;
	return m_fStream.tellg() -  pos;
}
unsigned int ImageFile::Write(const unsigned char* pBlob,int nLen)
{
	m_fStream.write((const char*)pBlob,nLen);
	return nLen;
}

ReadImageBuffer::ReadImageBuffer(const unsigned char* buff,int nLen)
{
	m_Buff = buff;
	m_nLen = nLen;
	m_Index = 0;
}
bool ReadImageBuffer::Eof()
{
	return m_Index >= m_nLen;
}
unsigned int ReadImageBuffer::Write(const unsigned char* pBlob,int nLen)
{
	return 0;
}
unsigned int ReadImageBuffer::Read(unsigned char* pBlob,int nLen)
{
	if(m_Index >=m_nLen)
		return 0;
	int nRead = nLen;
	if(m_Index + nRead > m_nLen)
		nRead = m_nLen - m_Index;
	if(pBlob)
		memcpy(pBlob,m_Buff + m_Index,nRead);
	m_Index+=nRead;
	return nRead;
}

bool ReadImageBuffer::Rewind()
{
	m_Index = 0;
	return true;
}


ImageBuffer::ImageBuffer(GsByteBuffer* pBuffer)
{
	m_pBuffer = pBuffer;
	m_Index = 0;
}
bool ImageBuffer::Eof()
{
	return m_Index >= m_pBuffer->BufferSize();
}
unsigned int  ImageBuffer::Write(const unsigned char* pBlob,int nLen)
{ 
	m_pBuffer->Append(pBlob,nLen);
	return nLen;
}

unsigned int ImageBuffer::Read(unsigned char* pBlob,int nLen)
{
	if(m_Index >=m_pBuffer->BufferSize())
		return 0;
	int nRead = nLen;
	if(m_Index + nRead > m_pBuffer->BufferSize())
		nRead = m_pBuffer->BufferSize() - m_Index;
	if(pBlob)
		memcpy(pBlob,m_pBuffer->BufferHead() + m_Index,nRead);
	m_Index+=nRead;
	return nRead;
}

bool ImageBuffer::Rewind()
{
	m_Index = 0;
	return true;
}


ImageDecoder::ImageDecoder()
{
	m_pImageBuffer = NULL;
}
bool ImageDecoder::Begin(int w,int h,int nStride,int bpp)
{ 
	if(!m_pImageBuffer) return false;
	m_bPP = bpp;
	m_pImageBuffer->Alloc(w,h);
	return true;
}
bool ImageDecoder::WriteRow(const unsigned char* pRow,int nLen,int nRowIndex,unsigned char* pPalette,int bitdepth)
{
	if(!m_pImageBuffer) return false;
	unsigned char* pRowHead = m_pImageBuffer->Row(nRowIndex);
	//bitdepth 的值应该为1，2，4，8
	//按照位进行偏移。
	for(int  i = 0;i<nLen * bitdepth ;i+=bitdepth)
	{
		//颜色索引
		unsigned  char b = pRow[i / 8];
		if(bitdepth != 8)
		{
			b=b<<(i % 8);
			b=b>>(8-bitdepth);
		}
		//调色板的颜色
		unsigned char* pPalColor = pPalette + (b * 4);
		memcpy(pRowHead,pPalColor,4);
		pRowHead+=4;
	}
	return false;
}
bool ImageDecoder::WriteRow(const unsigned char* pRow,int nLen,int nRowIndex)
{
	if(!m_pImageBuffer) return false;
	unsigned char* pRowHead = m_pImageBuffer->Row(nRowIndex);
	if(m_bPP == 32)
		memcpy(pRowHead ,pRow,nLen);
	else if(m_bPP == 24)
	{
		for(int i =0;i<nLen;i+=3)
		{
			memcpy(pRowHead,pRow,3);
			pRowHead[3] =255;
			pRowHead+=4;
			pRow+=3;
		}
	}
	return true;
}
void ImageDecoder::End()
{  
}

PNGImageEncoder::PNGImageEncoder(ImageIO* io)
{
	m_ImageIO = io;
	  

}  
 
PNGImageEncoder::~PNGImageEncoder()
{ 
	 
}

JpegImageEncoder::JpegImageEncoder(ImageIO* io)
{
	m_ImageIO = io; 
	m_param.m_quality = 75;
}
void JpegImageEncoder::Quality(int nQuality)
{
	m_param.m_quality = nQuality;
}
JpegImageEncoder::~JpegImageEncoder()
{ 

} 
bool JpegImageEncoder::put_buf(const void* Pbuf, int len)
{
	m_ImageIO->Write((const unsigned char*)Pbuf,len);
	return true;
}
bool JpegImageEncoder::Begin(int w,int h,int nStride,int bpp)
{
	m_Encoder.init(this,w,h,3,m_param);
	m_ScanLine.resize(w * 3);
	return true;
}
bool JpegImageEncoder::WriteRow(const unsigned char* pRow,int nLen,int nRowCount)
{
	for(int i =0;i<nRowCount;i++)
	{
		unsigned char* pRowHead = &m_ScanLine[0];
		for(int i =0;i<m_ScanLine.size() / 3;i++)
		{
			pRowHead[i * 3]		= pRow[i * 4]  * 1.0 * pRow[i * 4 + 3] / 255;			//R
			pRowHead[i * 3 + 1] = pRow[i * 4 + 1]  * 1.0 * pRow[i * 4 + 3] / 255;  //G
			pRowHead[i * 3 + 2] = pRow[i * 4 + 2]  * 1.0 * pRow[i * 4 + 3] / 255;  //B
		}
		if(!m_Encoder.process_scanline(&m_ScanLine[0])) return false;
	}

    return true;
}

void JpegImageEncoder::End()
{ 
	 m_Encoder.process_scanline(NULL);
} 
JpegImageDecoder::JpegImageDecoder(ImageIO* pIO)
{
	m_ImageIO = pIO;
}   
// The read() method is called when the internal input buffer is empty.
// Parameters:
// pBuf - input buffer
// max_bytes_to_read - maximum bytes that can be written to pBuf
// pEOF_flag - set this to true if at end of stream (no more bytes remaining)
// Returns -1 on error, otherwise return the number of bytes actually written to the buffer (which may be 0).
// Notes: This method will be called in a loop until you set *pEOF_flag to true or the internal buffer is full.
int JpegImageDecoder::read(jpgd::uint8 *pBuf, int max_bytes_to_read, bool *pEOF_flag)
{
	int nRet = m_ImageIO->Read(pBuf,max_bytes_to_read);
	if(nRet <=0 || nRet < max_bytes_to_read)
		*pEOF_flag = true;
	return nRet;
}
bool JpegImageDecoder::Decode()
{
	jpgd::jpeg_decoder decoder(this);
	if(decoder.begin_decoding() != jpgd::JPGD_SUCCESS)
		return false;
	int h = decoder.get_height();
	Begin(decoder.get_width(),h,decoder.get_bytes_per_scan_line(),decoder.get_bytes_per_pixel() * 8);
	
	for(int i = 0;i<h;i++)
	{
		const void* pLine;
		jpgd::uint nLen;
		int nRet = decoder.decode(&pLine,&nLen);
		if(nRet == jpgd::JPGD_FAILED) break;
		WriteRow((const unsigned char*)pLine,nLen,i);
		if(nRet == jpgd::JPGD_DONE) break;
	}
	End();
	return true;
}

bool JpegImageDecoder::Decode(UTILITY_NAME::GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	jpgd::jpeg_decoder decoder(this);
	if (decoder.begin_decoding() != jpgd::JPGD_SUCCESS)
		return false;
	info->Height = decoder.get_height();
	info->Width = decoder.get_width();
	info->Bpp = decoder.get_bytes_per_pixel() * 8;
	info->EncodeType = GsImageEncodeType::eJPG;
	return true;
}

  
bool PNGImageEncoder::Begin(int w,int h,int nStride,int bpp)
{   
	 
	 
	return true;
}
bool PNGImageEncoder::WriteRow(const unsigned char* pRow,int nLen,int nRowCount)
{ 
	return true;
}
void PNGImageEncoder::End()
{
	 
}
   
PNGImageDecoder::PNGImageDecoder(ImageIO* pIO)
{
	m_ImageIO = pIO;
}  


BmpImageDecoder::BmpImageDecoder(ImageIO* pIO)
{
	m_ImageIO = pIO;
}
bool BmpImageDecoder::Decode()
{
	unsigned int bytes_read_so_far = 0;
    unsigned int bfSize;
    unsigned int bfOffBits;
    unsigned int bfReserved;
    unsigned int biSize;
    unsigned int biWidth;
	//bmp图片高度不可以用无符号的，h为负值，表示图片坐标原点在左上角，为正，表示坐标原点在左上角
    int biHeight;
    unsigned short biBitCount;
    unsigned int biCompression;
	bool isLeftTop = true;
    /*
    unsigned long biSizeImage;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
    */
	int R=0,G=1,B=2,A=3;

    unsigned int a, b, c, d, i;

//        streamsize num;
	unsigned char buf[100] = { 0 };

	bytes_read_so_far += m_ImageIO->Read(buf,2);
	//前两个字符必须是BM
	if (buf[0] != 'B' || buf[1] != 'M')
		return false;

	// now read the BITMAPFILEHEADER
	bytes_read_so_far += m_ImageIO->Read(buf,12);
	i = 0;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    bfSize = a | (b<<8) | (c<<16) | (d<<24);
	i = 4;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    bfReserved = a | (b<<8) | (c<<16) | (d<<24);
	i = 8;
	a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
	bfOffBits = a | (b<<8) | (c<<16) | (d<<24);
	// if this value isn't zero then there is something wrong
    // with this bitmap.
    if (bfReserved != 0)
		return false;
	// load the BITMAPINFOHEADER
    bytes_read_so_far += m_ImageIO->Read(buf,40);
	i = 0;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    biSize = a | (b<<8) | (c<<16) | (d<<24);

    i += 4;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    biWidth = a | (b<<8) | (c<<16) | (d<<24);

    i += 4;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    biHeight = a | (b<<8) | (c<<16) | (d<<24);
	isLeftTop = biHeight < 0;
	biHeight = abs(biHeight);

    i += 4+2;
    a = buf[i]; b = buf[i+1];
    biBitCount = static_cast<unsigned short>(a | (b<<8));

    i += 2;
    a = buf[i]; b = buf[i+1]; c = buf[i+2]; d = buf[i+3];
    biCompression = a | (b<<8) | (c<<16) | (d<<24);

    if (biSize != 40)
		return false;

    // read and discard any extra bytes that are part of the header
    if (biSize > 40)
		bytes_read_so_far += m_ImageIO->Read(buf,biSize-40);

	if (m_info)
	{
		m_info->Width = biWidth;
		m_info->Height = biHeight;
		m_info->Bpp = biBitCount;
		m_info->EncodeType = GsImageEncodeType::eBMP;
		return true;
	}
	
	m_pImageBuffer->Alloc(biWidth,biHeight);

	if(biBitCount == 1) //索引色模式
	{
        // figure out how the pixels are packed
        int padding;
        if (bfSize - bfOffBits == biWidth*biHeight/8)
            padding = 0;
        else
            padding = 4 - ((biWidth+7)/8)%4;

        const unsigned int palette_size = 2;
        unsigned char red[palette_size];
        unsigned char green[palette_size];
        unsigned char blue[palette_size];

        for (unsigned int i = 0; i < palette_size; ++i)
        {
			bytes_read_so_far += m_ImageIO->Read(buf,4);
			//bytes_read_so_far += 4;//移除bytes_read_so_far重复计算，导致与bfOffBits不匹配
            blue[i] = buf[0];
            green[i] = buf[1];
            red[i] = buf[2];
        }
		// seek to the start of the pixel data
        while (bytes_read_so_far != bfOffBits)
        {
            const int to_read = GsMath::Min(bfOffBits - bytes_read_so_far, (unsigned int)sizeof(buf));
			unsigned int readsize =  m_ImageIO->Read(buf, to_read);
			if (0 == readsize)
				break;
			bytes_read_so_far += readsize;
            
        }
		// load the image data
        for (int row = biHeight-1; row >= 0; --row)
        {
			unsigned char* pColor =m_pImageBuffer->Row(row);
            for (unsigned int col = 0; col < biWidth; col+=8)
            {
				m_ImageIO->Read(buf,1);
                unsigned char pixels[8];

                pixels[0] = (buf[0]>>7);
                pixels[1] = ((buf[0]>>6)&0x01);
                pixels[2] = ((buf[0]>>5)&0x01);
                pixels[3] = ((buf[0]>>4)&0x01);
                pixels[4] = ((buf[0]>>3)&0x01);
                pixels[5] = ((buf[0]>>2)&0x01);
                pixels[6] = ((buf[0]>>1)&0x01);
                pixels[7] = ((buf[0])&0x01);
				
                for (int i = 0; i < 8 && col+i < biWidth; ++i)
                {
                    pColor[R]   = red[pixels[i]];
                    pColor[G] = green[pixels[i]];
                    pColor[B]  = blue[pixels[i]];
                    pColor[A]  = 0xFF;

					pColor+=4;
                }
				
            }
			m_ImageIO->Read(buf,padding);
        }
		return true;
    }
	if(biBitCount == 4) //调色板模式
	{
        // figure out how the pixels are packed
        int padding;
        if (bfSize - bfOffBits == biWidth*biHeight/2)
            padding = 0;
        else
            padding = 4 - ((biWidth+1)/2)%4;

        const unsigned int palette_size = 16;
        unsigned char red[palette_size];
        unsigned char green[palette_size];
        unsigned char blue[palette_size];

        for (unsigned int i = 0; i < palette_size; ++i)
        {
			bytes_read_so_far += m_ImageIO->Read(buf,4);
            blue[i] = buf[0];
            green[i] = buf[1];
            red[i] = buf[2];
        }


        // seek to the start of the pixel data
        while (bytes_read_so_far != bfOffBits)
        {
            const int to_read = (int)GsMath::Min(bfOffBits - bytes_read_so_far, (unsigned int)sizeof(buf));
            bytes_read_so_far += m_ImageIO->Read(buf,to_read);
        }

        // load the image data
        for (int row = biHeight-1; row >= 0; --row)
        {
			
			unsigned char* pColor =m_pImageBuffer->Row(row);
            for (unsigned int col = 0; col < biWidth; col+=2)
            {
				m_ImageIO->Read(buf,1); 

                const unsigned char pixel1 = (buf[0]>>4);
                const unsigned char pixel2 = (buf[0]&0x0F);
				if (col + 1 < biWidth)
				{
					pColor[R] = red[pixel1];
					pColor[G] = green[pixel1];
					pColor[B] = blue[pixel1];
					pColor[A] = 0xFF;
					pColor += 4;
					pColor[R] = red[pixel2];
					pColor[G] = green[pixel2];
					pColor[B] = blue[pixel2];
					pColor[A] = 0xFF;
					pColor += 4;
				}
            }
			
			m_ImageIO->Read(buf,padding); 
        }
		return true;

    } 
	if(biBitCount == 8) //256调色板模式
	{
        // figure out how the pixels are packed
        int padding;
        if (bfSize - bfOffBits == biWidth*biHeight)
            padding = 0;
        else
            padding = 4 - biWidth%4;

        // check for this case.  It shouldn't happen but some BMP writers screw up the files
        // so we have to do this.
        if (biHeight*(biWidth+padding) > bfSize - bfOffBits)
            padding = 0;

        const unsigned int palette_size = 256;
        unsigned char red[palette_size];
        unsigned char green[palette_size];
        unsigned char blue[palette_size];

        for (unsigned int i = 0; i < palette_size; ++i)
        {
			
			bytes_read_so_far += m_ImageIO->Read(buf,4); 
            blue[i] = buf[0];
            green[i] = buf[1];
            red[i] = buf[2];
        }


        // seek to the start of the pixel data
        while (bytes_read_so_far != bfOffBits)
        {
            const int to_read = (int)GsMath::Min(bfOffBits - bytes_read_so_far, (unsigned int)sizeof(buf));
			bytes_read_so_far += m_ImageIO->Read(buf,to_read); 
        }

        // Next we load the image data.

        // if there is no RLE compression
        if (biCompression == 0)
        {
            for (int row = biHeight-1; row >= 0; --row)
            {
				unsigned char* pColor = m_pImageBuffer->Row(row);
                for (unsigned int col = 0; col < biWidth; ++col)
                {
					m_ImageIO->Read(buf,1);
					pColor[R]  = red[buf[0]];
                    pColor[G]  = green[buf[0]];
                    pColor[B]  = blue[buf[0]];
					pColor[A]  = 0xFF; 
					pColor += 4;
                }
				
				m_ImageIO->Read(buf,padding);
            }
        }
        else
        {
            // Here we deal with the psychotic RLE used by BMP files.

            // First zero the image since the RLE sometimes jumps over
            // pixels and assumes the image has been zero initialized.
            
            int row = biHeight-1;
            int col = 0;
            while (true)
            {
				m_ImageIO->Read(buf,2);
                const unsigned char count = buf[0];
                const unsigned char command = buf[1];

                if (count == 0 && command == 0)
                {
                    // This is an escape code that means go to the next row
                    // of the image
                    --row;
                    col = 0;
                    continue;
                }
                else if (count == 0 && command == 1)
                {
                    // This is the end of the image.  So quit this loop.
                    break;
                }
                else if (count == 0 && command == 2)
                {
                    // This is the escape code for the command to jump to
                    // a new part of the image relative to where we are now.
					
					m_ImageIO->Read(buf,2);
                    col += buf[0];
                    row -= buf[1];
                    continue;
                }
                else if (count == 0)
                {
                    // This is the escape code for a run of uncompressed bytes

                    if (row < 0 || col + command > biWidth)
                    {
                        // If this is just some padding bytes at the end then ignore them
                        if (row >= 0 && col + count <= biWidth + padding)
                            continue;
						return false;
                    }

                    // put the bytes into the image
					
					unsigned char* pColor = m_pImageBuffer->Row(row);
					pColor+=(col *4);
                    for (unsigned int i = 0; i < command; ++i)
                    {
						m_ImageIO->Read(buf,1);
                        
                        pColor[R]   = red[buf[0]];
                        pColor[G] = green[buf[0]];
                        pColor[B]  = blue[buf[0]];
						pColor[A] = 0xFF;
                        
						pColor+=4;
                    }

                    // if we read an uneven number of bytes then we need to read and
                    // discard the next byte.
                    if ((command&1) != 1)
                    {
						m_ImageIO->Read(buf,1);
                    }

                    continue;
                }

                
                if (row < 0 || col + count > biWidth)
                {
                    // If this is just some padding bytes at the end then ignore them
                    if (row >= 0 && col + count <= biWidth + padding)
                        continue;

                    return false;
                }
				
				unsigned char* pColor = m_pImageBuffer->Row(row);
				pColor+=(col *4);
                // put the bytes into the image
                for (unsigned int i = 0; i < count; ++i)
                {
					pColor[R] =red[command];
                    pColor[G] = green[command];
                    pColor[B] = blue[command];
					pColor[A] = 0xFF;
					pColor+=4;
                }
            }
        }
		return true;
    }
	
	if(biBitCount == 16) //16色不支持
	{
		return false;
	}
    if(biBitCount == 24) //256调色板模式
	{
		// figure out how the pixels are packed
        int padding;
        if (bfSize - bfOffBits == biWidth*biHeight*3)
            padding = 0;
        else
            padding = 4 - (biWidth*3)%4;

        // check for this case.  It shouldn't happen but some BMP writers screw up the files
        // so we have to do this.
        if (biHeight*(biWidth*3+padding) > bfSize - bfOffBits)
            padding = 0;
                        
        // seek to the start of the pixel data
        while (bytes_read_so_far != bfOffBits)
        {
			const int to_read = (int)GsMath::Min(bfOffBits - bytes_read_so_far, (unsigned int)sizeof(buf));
			bytes_read_so_far += m_ImageIO->Read(buf,to_read);
        }

        // load the image data
        for (int row = biHeight-1; row >= 0; --row)
        {
			
			unsigned char* pColor = m_pImageBuffer->Row(row);
            for (unsigned int col = 0; col < biWidth; ++col)
            {
				m_ImageIO->Read(buf,3);
					 
                pColor[R] = buf[2];
                pColor[G] = buf[1];
                pColor[B] = buf[0];
				pColor[A] = 0xFF;
				pColor+=4;
            }
			m_ImageIO->Read(buf,padding);
        }
		return true;
    }

	if (biBitCount == 32 && biCompression == 0)//32位数据按照BGRA的顺序存储
	{
		m_ImageIO->Read(buf, bfOffBits - 14 - biSize);
		unsigned char temp[4] = { 0 };
		if (isLeftTop)//biHeight为负，表面图形是上下颠倒的
		{
			unsigned char* pColor = m_pImageBuffer->Row(0);
			for (int i = 0; i < biWidth * biHeight; i++)
			{
				m_ImageIO->Read(temp, 4);

				pColor[R] = temp[2];
				pColor[G] = temp[1];
				pColor[B] = temp[0];
				pColor[A] = temp[3];
				pColor += 4;
			}
		}
		else
		{
			for (int i = biHeight - 1; i >= 0; --i)
			{
				unsigned char* pColor = m_pImageBuffer->Row(i);
				for (int j = 0; j < biWidth; j++)
				{
					m_ImageIO->Read(temp, 4);

					pColor[R] = temp[2];
					pColor[G] = temp[1];
					pColor[B] = temp[0];
					pColor[A] = temp[3];
					pColor += 4;
				}
			}
				
		}
		return true;
	}

	return false;
}

bool BmpImageDecoder::Decode(UTILITY_NAME::GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	m_info = info;

	bool b = this->Decode();
	m_ImageIO->Rewind();
	m_info = nullptr;

	return b;
}


#include <image.h>
using namespace UTILITY_NAME;

GsImageDecoder::GsImageDecoder()
{

}
GsImageDecoder::~GsImageDecoder()
{

}

bool GsImageDecoder::Decode(GsImageHeaderInfo* info)
{
	return false;
}


UTILITY_NAME::GsSmarterPtr<GsImageDecoder> GsImageDecoder::CreateImageDecoder(const char* filePath)
{
	GsImageEncodeType type = GsImage::EncodeType(filePath);

	switch (type)
	{
	case GeoStar::Utility::eUnknownImage:
		break;
	case GeoStar::Utility::ePNG:
		return new GsPNGImageDecoder(filePath);
	case GeoStar::Utility::eJPG:
		return new GsJpegImageDecoder(filePath);
	case GeoStar::Utility::eBMP:
		return new GsBMPImageDecoder(filePath);
	case GeoStar::Utility::eDNG:
		break;
	case GeoStar::Utility::eGIF:
		return new GsGIFImageDecoder(filePath);
	case GeoStar::Utility::eTGA:
		return new GsTGAImageDecoder(filePath);
	case GeoStar::Utility::eKTX:
	case GeoStar::Utility::eDDS:
	case GeoStar::Utility::eCRN:
		return new GsTextureDecoder(filePath);
	case GeoStar::Utility::eTIFF:
		return new GsTIFFImageDecoder(filePath);
	case GeoStar::Utility::ePAM:
		break;
	case GeoStar::Utility::eWEBP:
		return new GsWEBPImageDecoder(filePath);
	case GeoStar::Utility::eKTX2:
		break;
	default:
		break;
	}

	return UTILITY_NAME::GsSmarterPtr<GsImageDecoder>();
}

UTILITY_NAME::GsSmarterPtr<GsImageDecoder> GsImageDecoder::CreateImageDecoder(const unsigned char* blob, unsigned int len)
{
	GsImageEncodeType type = GsImage::EncodeType(blob, len);

	switch (type)
	{
	case GeoStar::Utility::eUnknownImage:
		break;
	case GeoStar::Utility::ePNG:
		return new GsPNGImageDecoder(blob, len);
	case GeoStar::Utility::eJPG:
		return new GsJpegImageDecoder(blob, len);
	case GeoStar::Utility::eBMP:
		return new GsBMPImageDecoder(blob, len);
	case GeoStar::Utility::eDNG:
		break;
	case GeoStar::Utility::eGIF:
		return new GsGIFImageDecoder(blob, len);
	case GeoStar::Utility::eTGA:
		return new GsTGAImageDecoder(blob, len);
	case GeoStar::Utility::eKTX:
	case GeoStar::Utility::eDDS:
	case GeoStar::Utility::eCRN:
		return new GsTextureDecoder(blob, len);
	case GeoStar::Utility::eTIFF:
		return new GsTIFFImageDecoder(blob, len);
	case GeoStar::Utility::ePAM:
		break;
	case GeoStar::Utility::eWEBP:
		return new GsWEBPImageDecoder(blob, len);
	case GeoStar::Utility::eKTX2:
		break;
	default:
		break;
	}

	return UTILITY_NAME::GsSmarterPtr<GsImageDecoder>();
}

 
//从github获取此开源库实现。
//https://github.com/hidefromkgb/gif_load
#include "gif_load.h"
#if !defined(_WIN32)
#include <unistd.h>
#endif
GsGIFImageDecoder::GsGIFImageDecoder(const char* file)
{
	m_File = file;
}
GsGIFImageDecoder::GsGIFImageDecoder(const unsigned char *blob, int nlen) 
{
	m_blob = blob;
	m_nBlobLen = nlen;
}
/// \brief 影像解码像素大小
GsSize GsGIFImageDecoder::Size()const
{
	return m_Size;
}
UTILITY_NS
void eamf(void* data, GIF_WHDR* hdr)
{
	std::function<int(void* data)> *f = (std::function<int(void* data)>*)data;
	(*f)(hdr);
}
UTILITY_ENDNS
int GsGIFImageDecoder::OnDecode(  void* data)
{
	GIF_WHDR* hdr = (GIF_WHDR*)data;
	//基本信息
	if (hdr->ifrm <0)
	{
		m_Duration = hdr->time * 10;
		m_nFrameCount = hdr->nfrm;
		m_Size.Width = hdr->xdim;
		m_Size.Height = hdr->ydim;
		return 1;
	} 
	if (m_info)
	{
		return 0;
	}
	if (hdr->time > 0)
		m_Duration = hdr->time * 10;
	int dur = hdr->time *10;
	if (dur <= 0)
		dur = m_Duration;

	GsColor* color = BeginFrame(hdr->ifrm, dur,hdr->nfrm, hdr->xdim, hdr->ydim);
	//如果不关心这一帧则返回0
	if (!color)
		return 0;

	int i = 0;
	int nSize = hdr->xdim * hdr->ydim;
	//如果背景是基于上一帧
	if (hdr->mode == GIF_PREV)
	{
		if (m_LastFrame.BufferSize() >= nSize * 4)
			memcpy(color, m_LastFrame.BufferHead(), nSize * 4);
	}

	//如果是基于背景色
	if (hdr->mode == GIF_BKGD)
	{
		//如果是背景色则添加背景色
		for (int r = 0; r < hdr->ydim; r++)
		{
			for (int c = 0; c < hdr->xdim; c++)
			{
				color[i].Argb = 0;
				 
				if (hdr->bkgd != hdr->tran)
				{
					color[i].A = 255;
					color[i].B = hdr->cpal[hdr->bkgd].R;
					color[i].G = hdr->cpal[hdr->bkgd].G;
					color[i].R = hdr->cpal[hdr->bkgd].B;
				}
			}
		}
	}
	else
		memset(color, 0, nSize * 4);

	i = 0;
	for (int r = hdr->fryo; r < hdr->fryd; r++)
	{
		for (int c = hdr->frxo; c < hdr->frxd; c++)
		{
			i = hdr->xdim * r + c;
			unsigned char idx = hdr->bptr[i];
			if (idx == hdr->tran)
				color[i].Argb = 0;
			else
			{
				color[i].A = 255;
				color[i].B = hdr->cpal[idx].R;
				color[i].G = hdr->cpal[idx].G;
				color[i].R = hdr->cpal[idx].B;
			}
		}
	} 
	m_LastFrame.Allocate(0);
	//保存上一帧
	m_LastFrame.AppendT(color, nSize);
	FrameData(hdr->ifrm);
	return 1;
}
/// \brief 开始一帧
GsColor* GsGIFImageDecoder::BeginFrame(int i, int Duration, int nTotal, int width, int height)
{
	m_Frames.emplace_back(Duration,new GsSimpleBitmap(width, height));
	GsColor* color = (GsColor*)m_Frames.back().second->Bit();
	return color;
}
/// \brief 帧数据
void GsGIFImageDecoder::Frame(int i, GsImage* fm,int Duration)
{

}
/// \brief 帧数据
void GsGIFImageDecoder::FrameData(int i)
{
	if (m_Frames.empty())
		return;
	Frame(i, m_Frames.back().second, m_Frames.back().first);
}

/// \brief 解码图像数据
/// \return bool  返回解码是否成功
bool GsGIFImageDecoder::Decode()
{
	//第一步根据文件名或者blob判断编码类型是否真的就是gif
	GsImageEncodeType eType = eUnknownImage;
	if (!m_File.empty())
		eType = GsImage::EncodeType(m_File.c_str());
	if (eType == eUnknownImage && m_blob)
		eType = GsImage::EncodeType(m_blob, m_nBlobLen);

	//不是GIF肯定不干活
	if (eType != eGIF)
		return false;

	const void* data = m_blob;
	int nLen = m_nBlobLen;
	GsGrowByteBuffer buff;
	if (!m_File.empty())
	{
		GsFile(m_File).ReadAllBytes(&buff);
		data = buff.BufferHead();
		nLen = buff.BufferSize();
	}
	std::function<int(void* data)> fun([=](void* data)->int {
		return OnDecode(data);
	});
	long n = GIF_Load(data, nLen, eamf, eamf, &fun, 0);

	if (0 == n)
		return false;

	return true;
}   

/// \brief 开始读取一帧图像
/// \param nFrameIndex 
/// \return 
GsImage* GsGIFImageDecoder::Frame(int n)
{
	if (n <0 || n>=m_Frames.size())
		return 0;
	return m_Frames.at(n).second.p;
}

/// \brief 图像的帧数量
int GsGIFImageDecoder::FrameCount()
{
	return m_nFrameCount;
}

int GsGIFImageDecoder::Duration(int n)
{ 
	if (n <0 || n>=m_Frames.size()) 
		return 0;

	return m_Frames.at(n).first;
}

bool GsGIFImageDecoder::Decode(GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	m_info = info;
	bool b = Decode();
	m_info = nullptr;
	if (b)
	{
		info->Height = m_Size.Height;
		info->Width = m_Size.Width;
		info->EncodeType = GsImageEncodeType::eGIF;
		info->Bpp = 8;
	}
	return b;
}
 

GsWEBPImageDecoder::GsWEBPImageDecoder(const char* filePath) : GsImageDecoder()
{
	m_WEBPDecoder = new WEBPImageDecoder(filePath);
}

GsWEBPImageDecoder::GsWEBPImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_WEBPDecoder = new WEBPImageDecoder(blob, len);
}

GsWEBPImageDecoder::~GsWEBPImageDecoder()
{
	delete ((WEBPImageDecoder*)m_WEBPDecoder);
}

GsSize GsWEBPImageDecoder::Size() const
{
	return m_Size;
}

bool GsWEBPImageDecoder::Decode()
{
	int bpp = 32;
	GsGrowByteBuffer buff;
	bool b = ((WEBPImageDecoder*)m_WEBPDecoder)->Decode(m_Size.Width, m_Size.Height, bpp, &buff);
	if (!b)
		return false;

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buff.BufferHead(), bpp);
	
	return true;
}

bool GsWEBPImageDecoder::Decode(GsImageHeaderInfo* info)
{
	return ((WEBPImageDecoder*)m_WEBPDecoder)->Decode(info);
}

GsImagePtr GsWEBPImageDecoder::Image()
{
	return m_Image;
}

GsJpegImageDecoder::GsJpegImageDecoder(const char* filePath)
{
	m_ImageIO = new ImageFile(filePath, true);
	m_JpegDecoder = new JpegImageDecoder((ImageIO*)m_ImageIO);
}

GsJpegImageDecoder::GsJpegImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_ImageIO = new ReadImageBuffer(blob,len);
	m_JpegDecoder = new JpegImageDecoder((ImageIO*)m_ImageIO);
}

GsJpegImageDecoder::~GsJpegImageDecoder()
{
	if (m_ImageIO)
		delete ((ImageIO*)m_ImageIO);
	if (m_JpegDecoder)
		delete ((JpegImageDecoder*)m_JpegDecoder); 
}

GsSize GsJpegImageDecoder::Size() const
{
	return m_Size;
}

bool GsJpegImageDecoder::Decode()
{
	auto decoder = ((JpegImageDecoder*)m_JpegDecoder);
	GsGrowByteBuffer buffer;

	MemRGBAVisitor buff(&buffer, &(m_Size.Width), &(m_Size.Height));

	decoder->ImageBuffer(&buff);
	bool b = decoder->Decode();
	if (!b)
		return false;

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buffer.BufferHead());

	return true;
}

bool GsJpegImageDecoder::Decode(GsImageHeaderInfo* info)
{
	auto decoder = ((JpegImageDecoder*)m_JpegDecoder);
	bool b = decoder->Decode(info);

	delete ((JpegImageDecoder*)m_JpegDecoder);
	((ImageIO*)m_ImageIO)->Rewind();
	m_JpegDecoder = new JpegImageDecoder((ImageIO*)m_ImageIO);
	return b;
}

GsImagePtr GsJpegImageDecoder::Image()
{
	return m_Image;
}

GsBMPImageDecoder::GsBMPImageDecoder(const char* filePath)
{
	m_ImageIO = new ImageFile(filePath, true);
	m_BmpDecoder = new BmpImageDecoder((ImageIO*)m_ImageIO);
}

GsBMPImageDecoder::GsBMPImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_ImageIO = new ReadImageBuffer(blob, len);
	m_BmpDecoder = new BmpImageDecoder((ImageIO*)m_ImageIO);
}

GsBMPImageDecoder::~GsBMPImageDecoder()
{
	if (m_ImageIO)
		delete ((ImageIO*)m_ImageIO);
	if (m_BmpDecoder)
		delete ((BmpImageDecoder*)m_BmpDecoder);
}

GsSize GsBMPImageDecoder::Size() const
{
	return m_Size;
}

bool GsBMPImageDecoder::Decode()
{
	auto decoder = ((BmpImageDecoder*)m_BmpDecoder);
	GsGrowByteBuffer buffer;

	MemRGBAVisitor buff(&buffer, &(m_Size.Width), &(m_Size.Height));

	decoder->ImageBuffer(&buff);
	bool b = decoder->Decode();
	if (!b)
		return false;

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buffer.BufferHead());

	return true;
}

GsImagePtr GsBMPImageDecoder::Image()
{
	return m_Image;
}

bool GsBMPImageDecoder::Decode(GsImageHeaderInfo* info)
{
	auto decoder = ((BmpImageDecoder*)m_BmpDecoder);
	return decoder->Decode(info);
}

GsTIFFImageDecoder::GsTIFFImageDecoder(const char* filePath)
{
	m_TIFFDecoder = new TIFFImageDecoder(new GsFileInputStream(filePath, true));
	
}

GsTIFFImageDecoder::GsTIFFImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_TIFFDecoder = new TIFFImageDecoder(new GsMemoryInputStream(blob, len));
}

GsTIFFImageDecoder::~GsTIFFImageDecoder()
{
	if (m_TIFFDecoder)
		delete ((TIFFImageDecoder*)m_TIFFDecoder);
}

GsSize GsTIFFImageDecoder::Size() const
{
	return m_Size;
}

bool GsTIFFImageDecoder::Decode()
{
	int bpp = 32;
	GsGrowByteBuffer buff;
	bool b = ((TIFFImageDecoder*)m_TIFFDecoder)->Decode(m_Size.Width, m_Size.Height, bpp, &buff);
	if (!b)
		return false;

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buff.BufferHead(), bpp);

	return true;
}

GsImagePtr GsTIFFImageDecoder::Image()
{
	return m_Image;
}

bool GsTIFFImageDecoder::Decode(GsImageHeaderInfo* info)
{
	return ((TIFFImageDecoder*)m_TIFFDecoder)->Decode(info);
}

GsTGAImageDecoder::GsTGAImageDecoder(const char* filePath)
{
	m_ImageIO = new ImageFile(filePath, true);
	m_TgaDecoder = new STBImageDecoder((ImageIO*)m_ImageIO);
}

GsTGAImageDecoder::GsTGAImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_ImageIO = new ReadImageBuffer(blob, len);
	m_TgaDecoder = new STBImageDecoder((ImageIO*)m_ImageIO);
}

GsTGAImageDecoder::~GsTGAImageDecoder()
{
	if (m_ImageIO)
		delete ((ImageIO*)m_ImageIO);
	if (m_TgaDecoder)
		delete ((STBImageDecoder*)m_TgaDecoder);
}

GsSize GsTGAImageDecoder::Size() const
{
	return m_Size;
}

bool GsTGAImageDecoder::Decode()
{
	auto decoder = ((STBImageDecoder*)m_TgaDecoder);
	GsGrowByteBuffer buffer;
	int bpp = 32;
	bool b = decoder->Decode(m_Size.Width, m_Size.Height, bpp, &buffer);
	if (!b)
		return false;

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buffer.BufferHead());

	return true;
}

GsImagePtr GsTGAImageDecoder::Image()
{
	return m_Image;
}

bool GsTGAImageDecoder::Decode(GsImageHeaderInfo* info)
{
	info->EncodeType = GsImageEncodeType::eTGA;
	return ((STBImageDecoder*)m_TgaDecoder)->Decode(info);
}

GsPNGImageDecoder::GsPNGImageDecoder(const char* filePath)
{
	m_pngDecoder = new GsFilePNG(filePath);
}

GsPNGImageDecoder::GsPNGImageDecoder(const unsigned char* blob, unsigned int len)
{
	m_pngDecoder = new GsMemoryPNG(blob, len);

}

GsPNGImageDecoder::~GsPNGImageDecoder()
{
	if (m_pngDecoder)
		delete ((GsPNG*)m_pngDecoder);
}

GsSize GsPNGImageDecoder::Size() const
{
	return m_Size;
}

bool GsPNGImageDecoder::Decode()
{
	auto png = ((GsPNG*)m_pngDecoder);
	bool b = png->BeginRead();
	if (!b)
		return false;

	m_Size.Width = png->Width();
	m_Size.Height = png->Height();
	GsGrowByteBuffer buff;
	buff.Allocate(m_Size.Width * m_Size.Height * 4);

	int nScanCount = png->ScanCount();
	for (int s = 0; s < nScanCount; ++s)
	{
		for (int i = 0; i < m_Size.Height; i++)
		{
			png->ReadRGBA8Row(buff.Ptr() + i * m_Size.Width * 4);
		}
	}

	png->FinishRead();

	m_Image = new GsSimpleBitmap(m_Size.Width, m_Size.Height, buff.BufferHead());
	return true;
}

bool GsPNGImageDecoder::Decode(GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	auto png = ((GsPNG*)m_pngDecoder);
	bool b = png->BeginRead();
	if (!b)
		return false;

	info->EncodeType = GsImageEncodeType::ePNG;
	info->Width = png->Width();
	info->Height = png->Height();

	png->FinishRead();

	return true;
}

GsImagePtr GsPNGImageDecoder::Image()
{
	return m_Image;
}

GsImageEncoder::GsImageEncoder()
{

}
GsImageEncoder::~GsImageEncoder()
{

}

//从github获取此开源库实现。 
//https://www.w3.org/Graphics/GIF/spec-gif89a.txt
//https://github.com/ginsweater/gif-h/blob/master/gif.h
#include "gif.h"
GsGIFImageEncoder::GsGIFImageEncoder(GsByteBuffer* buff, const GsSize& size)
{

	m_Size = size;
	m_Buffer = buff; 
}
GsGIFImageEncoder::GsGIFImageEncoder(const char* file,const GsSize& size)
	: m_strPath(file)
	, m_Size(size) 
{
}


GsGIFImageEncoder::~GsGIFImageEncoder()
{
	if (!m_ptrEncoder)
		return;
	delete  (GifWriter*)m_ptrEncoder;
	m_ptrEncoder = NULL;
}
/// \brief 是否抖色
bool &GsGIFImageEncoder::Dither()
{
	return m_Dither;
}
/// \brief 是否抖色
bool GsGIFImageEncoder::Dither()const
{
	return m_Dither;
}
UTILITY_NS
void buffer_fputs(const char* c, void* f)
{ 
	GsByteBuffer* buff = (GsByteBuffer*)f;
	buff->Append(c);
}
void buffer_fputc(int c, void* f)
{ 
	GsByteBuffer* buff = (GsByteBuffer*)f;
	unsigned char cc = c;
	buff->AppendT(cc);
}
void buffer_fwrite(const void* buf, int n, int count, void* f)
{ 
	GsByteBuffer* buff = (GsByteBuffer*)f;
	buff->Append((const unsigned char*)buf,n * count);
}
void buffer_close(void* f)
{

}
UTILITY_ENDNS
/// \brief 结束编码
bool GsGIFImageEncoder::BeginEncode()
{
	if (m_ptrEncoder || m_Size.Height == 0 || m_Size.Width == 0)
		return false;
	if (m_strPath.empty() && !m_Buffer)
		return false;

	GifWriter* w = new GifWriter;
	if (m_Buffer)
	{
		w->f = m_Buffer;
		w->close = &buffer_close;
		w->fwrite = &buffer_fwrite;
		w->fputc = &buffer_fputc;
		w->fputs = &buffer_fputs;
	}
	else
		FileWriter(w, GsFile(m_strPath).Path());

	m_ptrEncoder = w;
	return GifBegin((GifWriter*)m_ptrEncoder, m_Size.Width, m_Size.Height, m_nDuration,8, m_Dither);
}

/// \brief 开始一帧
bool GsGIFImageEncoder::BeginFrame(int nDuration)
{
	if (!m_ptrEncoder)
		return false;

	if (nDuration < 0)
		nDuration = 4;

	m_nDuration = nDuration / 10;
	return false;
}

/// \brief 写入影像或者一帧
bool GsGIFImageEncoder::Write(GsImage* img)
{
	if (!img || img->Width() != m_Size.Width || img->Height() != m_Size.Height)
		return false;
	if (!m_ptrEncoder)
		return false;
	GifWriter* w = (GifWriter*)m_ptrEncoder;
	w->firstFrame = true;
	memset(w->oldImage, 0, m_Size.Width * m_Size.Height * 4);
	return GifWriteFrame(w, img->Bit(), m_Size.Width, m_Size.Height, m_nDuration,8, m_Dither);
}

/// \brief 结束编码
bool GsGIFImageEncoder::FinishEncode()
{
	if (!m_ptrEncoder)
		return false;

	GifWriter* w = (GifWriter*)m_ptrEncoder;
	bool b = GifEnd(w);

	delete w;
	m_ptrEncoder = NULL;
	return b;
}

STBImageDecoder::STBImageDecoder(ImageIO* io)
{
	m_io = io;

}
bool STBImageDecoder::Decode(int& w, int& h, int& bpp, GsGrowByteBuffer* buff)
{
	if (!m_info && !buff)
		return false;
	stbi__context ctx;
	memset(&ctx, 0, sizeof(stbi__context));
	ctx.io_user_data = m_io;
	stbi_io_callbacks cb;
	cb.eof = eof_stbi;
	cb.read = read_stbi;
	cb.skip = skip_stbi;
	m_io->Rewind();

	int com;
	stbi_uc *data =  stbi_load_from_callbacks(&cb, m_io, &w, &h, &com, 4);
	if (!data)
		return false;
	bpp = 32;
	if (m_info)
	{
		m_info->Height = h;
		m_info->Width = w;
		m_info->Bpp = bpp;
		m_io->Rewind();
		return true;
	}

	buff->Append(data, w*h * 4);
	STBI_FREE(data);
	return true;
}

bool STBImageDecoder::Decode(UTILITY_NAME::GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	m_info = info;
	int width, height, bits;

	bool b = this->Decode(width, height, bits, nullptr);

	m_info = nullptr;

	return b;
}

TIFFImageDecoder::TIFFImageDecoder(GsInputStream* pStream)
{
	m_ptrStream = pStream;
}
#ifndef AMALGAMATION
#include "tif/tiffio.h"
static tmsize_t
tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	GsInputStream* io = (GsInputStream*)fd;
	return io->RawRead((unsigned char*)buf, size);
}
static tmsize_t
tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
	GsOutputStream* io = (GsOutputStream*)fd;
	return io->RawWrite((unsigned char*)buf, size);
}
static uint64
tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	GsInputStream* io = (GsInputStream*)fd;
	io->Seek(off, (GsStreamSeekOrigin)whence);
	return io->Offset();
}
static int
tiffCloseProc(thandle_t fd)
{
	return 0;
}
static uint64
tiffSizeProc(thandle_t fd)
{
	return 0;
}
static int
tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
{
	return 0;

}
static void
tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{

}
struct CloseTiff
{
	void operator() (TIFF* tif)const
	{
		TIFFClose(tif);
	}

};
bool TIFFImageDecoder::Decode(int& width, int& height, int& bits, GsGrowByteBuffer* buff)
{
	if (!m_info && !buff)
		return false;
	width = 0;
	height = 0;
	TIFF* tif = TIFFClientOpen("tiff", "r", m_ptrStream.p, tiffReadProc, tiffWriteProc, tiffSeekProc, tiffCloseProc, tiffSizeProc,
		tiffMapProc, tiffUnmapProc);
	if (!tif)
		return false;
	bits = 32;
	std::unique_ptr<TIFF, CloseTiff> ptrTiff(tif);
	//get image info
	int w = 0;
	int h = 0;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
	int samplesperpixel = 0;
	int bitspersample = 0;
	int rowsperstrip = 0;
	int photometric = 0;
	int orientation = 0;
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
	TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
	TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
	TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
	if (w <= 0 || h <= 0)
		return false;
	int metric = PHOTOMETRIC_RGB;
	if (m_info)
	{
		m_info->Height = h;
		m_info->Width = w;
		m_info->Bpp = samplesperpixel * bitspersample;
		m_info->EncodeType = GsImageEncodeType::eTIFF;
		m_ptrStream->Seek(0, GsStreamSeekOrigin::eSet);
		return true;
	}
	buff->Allocate(w * h * 4);
	GsColor* color = buff->PtrT<GsColor>();
	unsigned int* head = buff->PtrT<unsigned int>();

	int n = TIFFReadRGBAImageOriented(tif, w, h, head,
		ORIENTATION_TOPLEFT, 1);
	if (n == 0)
		return false;
	
	GsColor c;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			c.A = TIFFGetA(*head);
			c.B = TIFFGetR(*head);
			c.G = TIFFGetG(*head);
			c.R = TIFFGetB(*head);
			*color = c;
			color++;
			head++;
		}

	}
	color = buff->PtrT<GsColor>();
	//PHOTOMETRIC_RGB
	width = w;
	height = h;
	return true;
}

#else
bool TIFFImageDecoder::Decode(int& width, int& height, int& bits, GsGrowByteBuffer* buff)
{
    return false;
}
#endif //AMALGAMATION

bool TIFFImageDecoder::Decode(UTILITY_NAME::GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	m_info = info;
	int width, height, bits;

	bool b = this->Decode(width, height, bits, nullptr);
	
	m_info = nullptr;

	return b;
}

#ifdef ANDROID


#else
class BufferStreamer :public crnlib::buffer_stream
{
	GsByteBuffer* m_buff;
public:
	BufferStreamer(GsByteBuffer* buff)
	{
		m_buff = buff;
	}
	virtual crnlib::uint write(const void* pBuf, crnlib::uint len)
	{
		m_buff->Append((const unsigned char*)pBuf, len);
		return len;
	}
};
#endif // ANDROID


class KTXEncoder
{
	 
	std::string m_file;
	GsByteBuffer* m_Buffer = NULL; 
public:
	KTXEncoder(const GsSize& s, const char* file)
	{ 
		m_Size = s;
		m_file = file;
		//kts2 压缩得相关参数有几十个, 暂时先不开放,先用模式,
		m_CompressType = GsGlobalConfig::Instance()["Utility/Image/KTX2/CompressType"].IntValue(0);
		m_DeflateUseZstd = GsGlobalConfig::Instance()["Utility/Image/KTX2/UseZstd"].BoolValue(false);
	}
	KTXEncoder(const GsSize& s, GsByteBuffer* buff)
	{ 
		m_Size = s;
		m_Buffer = buff;
		//kts2 压缩得相关参数有几十个, 暂时先不开放,先用模式,
		m_CompressType = GsGlobalConfig::Instance()["Utility/Image/KTX2/CompressType"].IntValue(0);
		m_DeflateUseZstd = GsGlobalConfig::Instance()["Utility/Image/KTX2/UseZstd"].BoolValue(false);
	}  
	GsSize m_Size;
	GsImageEncodeType m_eType;
	GsTexturePixelFormat m_PixelFormat;
	int					m_QualityLevel;
	int					m_MipmapCount;
	int					m_CompressType = 0; //Basis Etc1s Or Uastc or Astc
	bool				m_DeflateUseZstd = false;

#ifdef ANDROID
	bool WriteImage(GsImage* img)
	{
		return true;
	}
#else
	bool WriteImage(GsImage* img)
	{
		if (m_eType == eKTX2)
			return ToKTX2(img);
		crnlib::mipmapped_texture src_tex;
		crnlib::mip_level* mip = crnlib::crnlib_new<crnlib::mip_level>();
		crnlib::image_u8* imgData = crnlib::crnlib_new<crnlib::image_u8>();
		crnlib::color_quad_u8* bits = (crnlib::color_quad_u8*)img->Bit();
		imgData->alias(bits, img->Width(), img->Height(), img->Width());
		mip->assign(imgData);
		src_tex.assign(mip);

		if (m_MipmapCount > 0)
		{
			crnlib::mipmapped_texture::generate_mipmap_params  params;
			params.m_max_mips = m_MipmapCount + 1;
			params.m_min_mip_size = 2;

			src_tex.generate_mipmaps(params, true);
		}
		if (m_eType == eCRN)
			return ToCRN(src_tex);
		if (m_eType == eKTX)
			return ToKTX(src_tex);
		if (m_eType == eDDS)
			return ToCRN(src_tex);
		return false;
	}
	crn_dxt_quality QualityLevelToDXTQuality(int nLevel)
	{
		if (nLevel <= 51)
			return cCRNDXTQualitySuperFast;
		if (nLevel <= 51 * 2)
			return cCRNDXTQualityFast;
		if (nLevel <= 51 * 3)
			return cCRNDXTQualityNormal;
		if (nLevel <= 51 * 4)
			return cCRNDXTQualityBetter;

		return cCRNDXTQualityUber;
	}
	bool ToKTX(crnlib::mipmapped_texture& src_tex)
	{
		crnlib::pixel_format format = ToPixelFormat(m_PixelFormat);
		if (src_tex.get_format() != format)
		{
			crnlib::dxt_image::pack_params pack_params;
			pack_params.m_compressor = cCRNDXTCompressorCRN;
			pack_params.m_quality = QualityLevelToDXTQuality(m_QualityLevel);
			src_tex.convert(format, pack_params);
		}
		if (m_Buffer)
		{
			m_Buffer->Allocate(m_Size.Width * m_Size.Height * 4);
			m_Buffer->Allocate(0);
			BufferStreamer buff(m_Buffer);
			crnlib::data_stream_serializer ser(&buff);

			return src_tex.write_ktx(ser);
		}
		else
		{
			crnlib::cfile_stream write_stream;
			if (!write_stream.open(GsFile(m_file.c_str()).Path(), crnlib::cDataStreamWritable | crnlib::cDataStreamSeekable))
				return false;
			crnlib::data_stream_serializer serializer(write_stream);
			return src_tex.write_ktx(serializer);
		}
		return true;
	}

	VkFormat ToKtx2VkFormat(GsRGBAType type) {
		if (GsRGBAType::eABGR32 == type)
			return VkFormat::VK_FORMAT_A8B8G8R8_UNORM_PACK32;
		else if (GsRGBAType::eARGB32 == type)
			return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32;
		else if (GsRGBAType::eBGR24 == type)
			return VkFormat::VK_FORMAT_B8G8R8_UNORM;
		else if (GsRGBAType::eBGRA32 == type)
			return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
		else if (GsRGBAType::eRGB24 == type)
			return  VkFormat::VK_FORMAT_R8G8B8_UNORM;
		else
			return  VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
	}
	
	bool ToKTX2(GsImage* img) 
	{
		ktxTexture* ktx2; 
		//ktx_error_code_e err;
		ktx_error_code_e result;
		ktxTextureCreateInfo createInfo;
		//createInfo.glInternalformat = 32856;//GL_RGB8;   // Ignored if creating a ktxTexture2.
		createInfo.vkFormat = ToKtx2VkFormat(img->RGBAType());//VK_FORMAT_R8G8B8_UNORM;   // Ignored if creating a ktxTexture1.
		createInfo.baseWidth = img->Width();
		createInfo.baseHeight = img->Height();
		createInfo.baseDepth = 1;
		createInfo.numDimensions = 2;
		createInfo.numLevels = 1;//log2(max_dim) + 1;
		createInfo.numLayers = 1;
		createInfo.numFaces = 1;
		createInfo.isArray = KTX_FALSE;
		createInfo.generateMipmaps = KTX_FALSE;
		result = ktxTexture2_Create(&createInfo, ktxTextureCreateStorageEnum::KTX_TEXTURE_CREATE_ALLOC_STORAGE, (ktxTexture2**)&ktx2);
		result = ktxTexture_SetImageFromMemory(ktx2, 0, 0, 0, (ktx_uint8_t*)img->Bit(), ktx2->dataSize);
		if (KTX_SUCCESS != result)
		{
			GS_E << " failed to create ktxTexture2; " << ktxErrorString(result);
			return false;
		}

		int threadcount = std::thread::hardware_concurrency() / 4;
		if (threadcount <= 0)
			threadcount = 1;
		if (1 >= m_CompressType)
		{
			ktxBasisParams params = {};
			params.structSize = sizeof(params);
			params.threadCount = threadcount;
			params.uastc =  m_CompressType;//0 默认ETC1S/BLZ  1 uastc
			params.compressionLevel = KTX_ETC1S_DEFAULT_COMPRESSION_LEVEL;
			params.qualityLevel = m_QualityLevel;
			result = ktxTexture2_CompressBasisEx((ktxTexture2*)ktx2, &params);// ktx_uint32_t(m_QualityLevel / 12));
		}
		else
		{
			ktxAstcParams params{};
			params.structSize = sizeof(params);
			params.threadCount = threadcount;
			params.blockDimension = KTX_PACK_ASTC_BLOCK_DIMENSION_6x6;
			params.mode = KTX_PACK_ASTC_ENCODER_MODE_LDR;
			params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_MEDIUM;
			params.normalMap = false;
			if (m_QualityLevel >= KTX_PACK_ASTC_QUALITY_LEVEL_FASTEST)
				params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_FASTEST;
			if (m_QualityLevel >= KTX_PACK_ASTC_QUALITY_LEVEL_FAST)
				params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_FAST;
			if (m_QualityLevel >= KTX_PACK_ASTC_QUALITY_LEVEL_MEDIUM)
				params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_MEDIUM;
			if (m_QualityLevel >= KTX_PACK_ASTC_QUALITY_LEVEL_THOROUGH)
				params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_THOROUGH;
			if (m_QualityLevel >= KTX_PACK_ASTC_QUALITY_LEVEL_EXHAUSTIVE)
				params.qualityLevel = KTX_PACK_ASTC_QUALITY_LEVEL_EXHAUSTIVE;

			result = ktxTexture2_CompressAstcEx((ktxTexture2*)ktx2, &params);
		}

		if (m_DeflateUseZstd)
		{
			result = ktxTexture2_DeflateZstd((ktxTexture2*)ktx2, ktx_uint32_t(m_QualityLevel / 12));
		}
	
		
		if (KTX_SUCCESS != result)
		{
			GS_E << " failed to deFlate ktxTexture2; " << ktxErrorString(result);
			return false;
		}

		if (m_Buffer) {
			m_Buffer->Allocate(0);
			ktx_uint8_t* pHead = nullptr;
			ktx_size_t size =0;
			ktxTexture_WriteToMemory(ktx2, &pHead, &size);
			m_Buffer->Append(pHead, size);
		}
		else {
			ktxTexture_WriteToNamedFile(ktx2, m_file.c_str());
		}
		return true;

	}

	crnlib::pixel_format ToPixelFormat(GsTexturePixelFormat format)
	{
		switch (format)
		{
		case eDXT1:
			return crnlib::PIXEL_FMT_DXT1;
		case eDXT2:
			return crnlib::PIXEL_FMT_DXT2;
		case eDXT3:
			return crnlib::PIXEL_FMT_DXT3;
		case eDXT4:
			return crnlib::PIXEL_FMT_DXT4;
		case eDXT5:
			return crnlib::PIXEL_FMT_DXT5;
		case e3DC:
			return crnlib::PIXEL_FMT_3DC;
		case eDXN:
			return crnlib::PIXEL_FMT_DXN;
		case eDXT5A:
			return crnlib::PIXEL_FMT_DXT5A;
		case eDXT5_CCxY:
			return crnlib::PIXEL_FMT_DXT5_CCxY;
		case eDXT5_xGxR:
			return crnlib::PIXEL_FMT_DXT5_xGxR;
		case eDXT5_xGBR:
			return crnlib::PIXEL_FMT_DXT5_xGBR;
		case eDXT5_AGBR:
			return crnlib::PIXEL_FMT_DXT5_AGBR;
		case eDXT1A:
			return crnlib::PIXEL_FMT_DXT1A;
		case eETC1:
			return crnlib::PIXEL_FMT_ETC1;
		case eR8G8B8:
			return crnlib::PIXEL_FMT_R8G8B8;
		case eL8:
			return crnlib::PIXEL_FMT_L8;
		case eA8:
			return crnlib::PIXEL_FMT_A8;
		case eA8L8:
			return crnlib::PIXEL_FMT_A8L8;
		case eA8R8G8B8:
			return crnlib::PIXEL_FMT_A8R8G8B8;
			break;
		}
		return crnlib::PIXEL_FMT_DXT5;
	}
	crn_format ToFormat(GsTexturePixelFormat format)
	{
		switch (format)
		{
		case eDXT1A:
		case eDXT1:
			return cCRNFmtDXT1;
		case 	eDXT2:
		case eDXT3:
		case eR8G8B8:
		case eL8:
		case eA8:
		case eA8L8:
		case eA8R8G8B8:
			return cCRNFmtDXT3;
		case eDXT4:
		case eDXT5:
			return cCRNFmtDXT5;
		case eDXT5A:
			return cCRNFmtDXT5A;
		case e3DC:
			return cCRNFmtDXN_XY;
		case eDXN:
			return cCRNFmtDXN_YX;
		case eDXT5_CCxY:
			return cCRNFmtDXT5_CCxY;
		case eDXT5_xGxR:
			return cCRNFmtDXT5_xGxR;
		case eDXT5_xGBR:
			return cCRNFmtDXT5_xGBR;
		case eDXT5_AGBR:
			return cCRNFmtDXT5_AGBR;
		case eETC1:
			return cCRNFmtETC1;
			break;
		}
		return cCRNFmtDXT5;
	}
	bool ToCRN(crnlib::mipmapped_texture& src_tex)
	{
		crn_comp_params pComp_params;
		if (m_eType == eCRN)
			pComp_params.m_file_type = cCRNFileTypeCRN;
		else
			pComp_params.m_file_type = cCRNFileTypeDDS;
		pComp_params.m_width = m_Size.Width;
		pComp_params.m_height = m_Size.Height;
		pComp_params.m_quality_level = m_QualityLevel;
		pComp_params.m_dxt_compressor_type = cCRNDXTCompressorCRN;
		pComp_params.m_format = ToFormat(m_PixelFormat);
		pComp_params.m_levels = src_tex.get_num_levels();

		crnlib::image_u8 temp_images[cCRNMaxFaces][cCRNMaxLevels];
		for (crnlib::uint f = 0; f < src_tex.get_num_faces(); f++)
		{
			for (crnlib::uint l = 0; l < src_tex.get_num_levels(); l++)
			{
				crnlib::image_u8* p = src_tex.get_level_image(f, l, temp_images[f][l]);

				pComp_params.m_pImages[f][l] = (crn_uint32*)p->get_ptr();
			}
		}

		unsigned int pActual_quality_level = m_QualityLevel;
		unsigned int image_write_flags = 0;
		float bitrate;
		crnlib::vector<crnlib::uint8> comp_data;
		if (!crnlib::create_compressed_texture(pComp_params, comp_data, &pActual_quality_level, &bitrate))
			return false;
		if (comp_data.size_in_bytes() <= 0)
			return false;
		if (m_Buffer)
			m_Buffer->Append(&comp_data[0], comp_data.size());
		else
			GsFile(m_file.c_str()).WriteAllBytes(&comp_data[0], comp_data.size());
		return true;
	}
#endif // ANDROID

	~KTXEncoder()
	{
		 
	}

};
 

/// \brief 输出编码后的数据到内存
GsTextureEncoder::GsTextureEncoder(GsByteBuffer* buff, const GsSize& size, GsImageEncodeType eType)
{ 
	m_Buffer = buff;
	m_Size = size;
	m_eType = eType;
	m_PixelFormat = eDXT5;
	m_QualityLevel = 128;
	m_Size.Width = GsMath::RoundUpPowerOf2(m_Size.Width);
	m_Size.Height = GsMath::RoundUpPowerOf2(m_Size.Height);
	m_MipmapCount = 0;
}
/// \brief 输出编码后的数据到文件
GsTextureEncoder::GsTextureEncoder(const char* file, const GsSize& size, GsImageEncodeType eType)
{
	m_MipmapCount = 0;
	m_eType = eType;
	m_Buffer = NULL;
	m_FileName = file;
	m_Size = size; 
	m_PixelFormat = eDXT5;
	m_QualityLevel = 128;
	m_Size.Width = GsMath::RoundUpPowerOf2(m_Size.Width);
	m_Size.Height = GsMath::RoundUpPowerOf2(m_Size.Height);
}
/// \brief 编码类型
GsImageEncodeType GsTextureEncoder::EncodeType()
{
	return m_eType;
}


GsTextureEncoder::~GsTextureEncoder()
{
	FinishEncode();
}
/// \brief 生成Mipmap的数量，缺省为0，不生产mimap
int	&GsTextureEncoder::MipmapCount()
{
	return m_MipmapCount;
}
/// \brief 生成Mipmap的数量，缺省为0，不生产mimap
int GsTextureEncoder::MipmapCount()const
{
	return m_MipmapCount;
}


/// \brief 设置纹理压缩格式，缺省为DXT5
GsTexturePixelFormat& GsTextureEncoder::PixelFormat()
{
	return m_PixelFormat;
}
/// \brief 设置纹理压缩格式，缺省为DXT5
GsTexturePixelFormat GsTextureEncoder::PixelFormat()const
{
	return m_PixelFormat;
}
/// \brief 纹理压缩质量，[0-255] 255=最佳，缺省128
int& GsTextureEncoder::QualityLevel()
{
	return m_QualityLevel;
}
/// \brief 纹理压缩质量，[0-255] 255=最佳，缺省128
int GsTextureEncoder::QualityLevel()const
{
	return m_QualityLevel;
}



/// \brief 开始编码
/// \return 
bool GsTextureEncoder::BeginEncode()
{ 
	if (m_QualityLevel < 0 || m_QualityLevel > 255)
		return false;
	if (m_eType < eKTX || m_eType > eCRN)
		return false;
	return true;
}

/// \brief 写入影像或者一帧
/// \param img 
/// \return 
bool GsTextureEncoder::Write(GsImage* img)
{
	if (!img)
		return false;

	//如果长和宽不是2的N次方对齐的。
	GsImagePtr ptrImg = img;
	if (img->Width() != m_Size.Width || img->Height() != m_Size.Height)
		ptrImg = new GsSimpleBitmap(m_Size.Width, m_Size.Height, img, GsRect(0, 0, img->Width(), img->Height()));
	
	if (m_Buffer)
	{
		KTXEncoder ktx(m_Size, m_Buffer);
		ktx.m_eType = m_eType;
		ktx.m_MipmapCount = m_MipmapCount;
		ktx.m_PixelFormat = m_PixelFormat;
		ktx.m_QualityLevel = m_QualityLevel;

		ktx.WriteImage(img);
	}
	else
	{
		KTXEncoder ktx(m_Size, m_FileName.c_str());
		ktx.m_eType = m_eType;
		ktx.m_MipmapCount = m_MipmapCount;
		ktx.m_PixelFormat = m_PixelFormat;
		ktx.m_QualityLevel = m_QualityLevel;
		
		ktx.WriteImage(img);
	}
	return true;
}

/// \brief 结束编码
/// \return 
bool GsTextureEncoder::FinishEncode()
{
	return true;
}

class KTX2Decoder {
private:
	GsSize m_Size;
	GsString m_File;
	const unsigned char* m_blob = NULL;
	int m_nBlobLen = 0;
	//GsImageEncodeType m_eType = eUnknownImage;
	GsTexturePixelFormat m_ePixelType;
	int m_nMipMap = 0;
	GsVector<GsImagePtr> m_vecImages;
public:
	KTX2Decoder(const char* file){
		m_File = file;
	}

	KTX2Decoder(const unsigned char* blob, int len) {
		m_blob = blob;
		m_nBlobLen = len;
	}

	/// \brief 获取影像解码像素大小
	/// \return GsSize 影响宽高结构体
	GsSize Size()const {
		return m_Size;
	}
	/// \brief 解码图像数据
	/// \return bool  返回解码是否成功
	bool Decode() {
		//解压
		//ktxTexture2_inflateZstdInt();
		ktxTexture* pKtx2;
		ktx_error_code_e result;
			//ktxTexture_CreateFromStdioStream(FILE * stdioStream,KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &pKtx2);
		if (m_blob) {
			result = ktxTexture2_CreateFromMemory(m_blob, m_nBlobLen, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, (ktxTexture2**)&pKtx2);
		}
		else {
			result = ktxTexture2_CreateFromNamedFile(m_File.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, (ktxTexture2**)&pKtx2);
		}
		if (KTX_SUCCESS != result)
		{
			GS_E << " failed to create ktxTexture2; " << ktxErrorString(result);
			return false;
		}
		m_Size.Width = pKtx2->baseWidth;
		m_Size.Height = pKtx2->baseHeight;
	/*	ktx_uint32_t format= pKtx2->vkFormat;
		ktx_uint32_t numLayers = pKtx2->numLayers;
		ktx_uint32_t numLevels = pKtx2->numLevels;
		ktx_uint32_t numFaces = pKtx2->numFaces;*/

	/*	ktx_uint8_t* ktxMemFile = 0;
		ktx_size_t ktxMemFileLen = 0;
		if (pKtx2->isCompressed) {
			auto size = ktxTexture_GetDataSizeUncompressed(pKtx2);
			result = ktxTexture2_DeflateZstd((ktxTexture2*)pKtx2, 0);
		}*/
		/*result = ktxTexture_WriteToMemory(pKtx2, &ktxMemFile,
			&ktxMemFileLen);*/

		m_nMipMap = pKtx2->numLevels;
		GsSimpleBitmapPtr ptrBmp = new GsSimpleBitmap(m_Size.Width, m_Size.Height);
		memcpy((void*)ptrBmp->Bit(), pKtx2->pData, pKtx2->dataSize);
		//memcpy((void*)ptrBmp->Bit(), ktxMemFile, ktxMemFileLen);
		m_vecImages.push_back(ptrBmp);
		return true;
	}

	/// \brief 纹理的像素压缩格式
	GsTexturePixelFormat PixelFormatType()const;

	/// \brief 纹理的Mipmap数量
	int MipmapCount()const {
		return m_nMipMap;
	}

	/// \brief 获取纹理影像数据
	GsImage* Image()const
	{
		return m_vecImages.front().p;
	}
	/// \brief 获取第N个mipmap影像
	GsImage* Mipmap(int n)const
	{
		if (n >= m_nMipMap)
			return nullptr;
		return m_vecImages[n];
	}
};


/// \brief 从文件解码
GsTextureDecoder::GsTextureDecoder(const char* file)
{
	m_File = file;
}
/// \brief  从内存解码
GsTextureDecoder::GsTextureDecoder(const unsigned char *blob, int nlen)
{
	m_blob = blob;
	m_nBlobLen = nlen;

}
/// \brief 获取影像解码像素大小
/// \return GsSize 影响宽高结构体
GsSize GsTextureDecoder::Size()const
{
	return m_Size;
}
#ifdef ANDROID

/// \brief 解码图像数据
/// \return bool  返回解码是否成功
bool GsTextureDecoder::Decode()
{
	return true;
}
#else
UTILITY_NS
crnlib::texture_file_types::format ToTextureFileType(GsImageEncodeType eType)
{
	if (eType == eDDS)
		return crnlib::texture_file_types::cFormatDDS;
	if (eType == eKTX)
		return crnlib::texture_file_types::cFormatKTX;

	if (eType == eCRN)
		return crnlib::texture_file_types::cFormatCRN;

	return crnlib::texture_file_types::cFormatInvalid;
}
GsTexturePixelFormat ToPixelFormat(crnlib::pixel_format f)
{
	switch (f)
	{
	case crnlib::PIXEL_FMT_DXT1:
		return eDXT1;
	case crnlib::PIXEL_FMT_DXT2:
		return eDXT2;
	case crnlib::PIXEL_FMT_DXT3:
		return eDXT3;
	case crnlib::PIXEL_FMT_DXT4:
		return eDXT4;
	case crnlib::PIXEL_FMT_DXT5:
		return eDXT5;
	case crnlib::PIXEL_FMT_3DC:
		return e3DC;
	case crnlib::PIXEL_FMT_DXN:
		return eDXN;
	case crnlib::PIXEL_FMT_DXT5A:
		return eDXT5A;
	case crnlib::PIXEL_FMT_DXT5_CCxY:
		return eDXT5_CCxY;
	case crnlib::PIXEL_FMT_DXT5_xGxR:
		return eDXT5_xGxR;
	case crnlib::PIXEL_FMT_DXT5_xGBR:
		return eDXT5_xGBR;
	case crnlib::PIXEL_FMT_DXT5_AGBR:
		return eDXT5_AGBR;
	case crnlib::PIXEL_FMT_DXT1A:
		return eDXT1A;
	case crnlib::PIXEL_FMT_ETC1:
		return eETC1;

	case crnlib::PIXEL_FMT_R8G8B8:
		return eR8G8B8;

	case crnlib::PIXEL_FMT_L8:
		return eL8;

	case crnlib::PIXEL_FMT_A8:
		return eA8;

	case crnlib::PIXEL_FMT_A8L8:
		return eA8L8;

	case crnlib::PIXEL_FMT_A8R8G8B8:
		return eA8R8G8B8;
		break;
	}
	return eDXT5;
}
UTILITY_ENDNS
/// \brief 解码图像数据
/// \return bool  返回解码是否成功
bool GsTextureDecoder::Decode()
{
	//第一步根据文件名或者blob判断编码类型是否真的就是gif
	m_eType = eUnknownImage;
	if (!m_File.empty())
		m_eType = GsImage::EncodeType(m_File.c_str());
	if (m_eType == eUnknownImage && m_blob)
		m_eType = GsImage::EncodeType(m_blob, m_nBlobLen);

	if (m_eType == eKTX2)
	{
		KTX2Decoder dec(m_File.c_str());
		if (!dec.Decode())
			return false;
		m_Size = dec.Size();
		m_nMipMap = dec.MipmapCount();
		m_vecImages.push_back(dec.Image());
		return true;
	}
	//不是GIF肯定不干活
	if (m_eType < eKTX || m_eType >eCRN)
		return false;

	crnlib::mipmapped_texture tex;
	if (!m_File.empty())
	{
		if (!tex.read_from_file(m_File.c_str(), ToTextureFileType(m_eType)))
			return false;
	}
	else
	{
		crnlib::buffer_stream stream(m_blob, m_nBlobLen);
		crnlib::data_stream_serializer ser(stream);
		if (!tex.read_from_stream(ser, ToTextureFileType(m_eType)))
			return false;
	}

	if (tex.get_num_faces() <= 0)
		return false;

	m_Size.Width = tex.get_width();
	m_Size.Height = tex.get_height();
	m_ePixelType = ToPixelFormat(tex.get_format());
	m_nMipMap = tex.get_num_levels() - 1;

	if (m_info)
		return true;

	//如果不是32位颜色则转换之。
	if (tex.get_format() != crnlib::PIXEL_FMT_A8R8G8B8)
	{
		crnlib::dxt_image::pack_params pack_params;
		pack_params.m_compressor = cCRNDXTCompressorCRN;
		if (!tex.convert(crnlib::PIXEL_FMT_A8R8G8B8, pack_params))
			return false;
	}

	crnlib::mip_ptr_vec& vec = tex.get_face(0);
	for (int i = 0; i < vec.size(); i++)
	{
		crnlib::mip_level* level = vec.at(i);
		GsSimpleBitmapPtr ptrBmp = new GsSimpleBitmap(level->get_width(), level->get_height());
		memcpy((void*)ptrBmp->Bit(), level->get_image()->get_pixels(), level->get_total_pixels() * 4);
		m_vecImages.push_back(ptrBmp);
	}

	return true;
}

#endif // ANDROID


bool GsTextureDecoder::Decode(GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	m_info = info;
	bool b = Decode();
	m_info = nullptr;
	if (b)
	{
		info->Height = m_Size.Height;
		info->Width = m_Size.Width;
		info->EncodeType = m_eType;
		info->Bpp = 32;
	}
	return b;
}


/// \brief 解码类型
GsImageEncodeType GsTextureDecoder::Type()const
{
	return m_eType;
}
/// \brief 纹理的像素压缩格式
GsTexturePixelFormat GsTextureDecoder::PixelFormatType()const
{
	return m_ePixelType;
}

/// \brief 纹理的Mipmap数量
int GsTextureDecoder::MipmapCount()const
{
	return m_nMipMap;
}

/// \brief 获取纹理影像数据
GsImage* GsTextureDecoder::Image()const
{
	if (m_vecImages.empty())
		return 0;
	return m_vecImages.front().p;
}

/// \brief 获取第N个mipmap影像
GsImage* GsTextureDecoder::Mipmap(int n)const
{
	if (n <0 || n + 1 >= m_vecImages.size())
		return 0;
	return m_vecImages.at(n + 1).p;
}

int CustomWebPWriterFunction(const uint8_t * data, size_t data_size, const WebPPicture * picture)
{
	ImageIO* io = (ImageIO*)picture->custom_ptr;
	return data_size ? ((int)io->Write(data, data_size)) : 1;
}

WEBPImageEncoder::WEBPImageEncoder(ImageIO * io)
{
	m_ImageIO = (io);
	if (!WebPPictureInit(&m_Picture) ||		!WebPConfigInit(&m_Config))
		GS_E << "WEBP encode Error!";

	m_Config.method = 6;
	m_Config.quality = 75;
	m_Config.autofilter = 1;
	m_Config.alpha_quality = 0;
	m_Config.emulate_jpeg_size = 1;
	//m_Config.thread_level = 1;
	if (!WebPValidateConfig(&m_Config)) {
		GS_E << "WebPValidateConfig  Error!";
		return ;
	}
}

WEBPImageEncoder::~WEBPImageEncoder()
{
}


bool WEBPImageEncoder::Begin(int w, int h, int nStride, int bpp)
{
	//只有argb和YUV直接选择,所有传入要更改颜色顺序, 但是颜色顺序为Picture.argb 为bgra才能转换成功
	m_Picture.use_argb = (((int)m_inputType <= 3) ? 1 : 0);
	m_Picture.width = w;
	m_Picture.height = h;
	m_Picture.argb_stride = w;
	m_Picture.writer =  &CustomWebPWriterFunction;
	m_Picture.custom_ptr = (void*)m_ImageIO;
	if (!WebPPictureAlloc(&m_Picture)) {
		GS_E << "WEBP Alloc memory Error!";
		return false;   
	}

	return true;
}

template<GsRGBAType T, int R, int G, int B, int A>
struct ToARGB 
{
	int ToARGBProcess(int len, unsigned int * pixelhead)
	{
		if (len < 1)
			return 0;
		
		for (int i = 0; i < len; i++, pixelhead++)
		{
			unsigned char* pixel = (unsigned char*)pixelhead;
			unsigned char a = 255;
			if(A >= 0)
				a = (char)pixel[A];

			unsigned char r = (char)pixel[R];
			unsigned char g = (char)pixel[G];
			unsigned char b = (char)pixel[B];
			if (A >= 0)
				pixel[3] = a;
			pixel[0] = b;
			pixel[1] = g;
			pixel[2] = r;
		}
		return -1;
	}
};
typedef ToARGB<GsRGBAType::eRGBA32, 0, 1, 2, 3>	  FromRGBA32;
typedef ToARGB<GsRGBAType::eABGR32, 3, 2, 1, 0>   FromABGR32;
typedef ToARGB<GsRGBAType::eBGRA32, 2, 1, 0, 3>   FromBGRA32;
typedef ToARGB<GsRGBAType::eARGB32, 1, 2, 3, 0>   FromARGB32;

typedef ToARGB<GsRGBAType::eRGB24, 0, 1, 2, -1>   FromRGB24;
typedef ToARGB<GsRGBAType::eBGR24, 2, 1, 0, -1>   FromBGR24;
bool WEBPImageEncoder::WriteRow(const unsigned char * pRow, int nLen, int nRowCount)
{
	//这里要将所有颜色转为argb
	switch (m_inputType)
	{
		case GeoStar::Utility::GsRGBAType::eBGRA32:
		{
			FromBGRA32 convert;
			convert.ToARGBProcess(nLen/4, (unsigned int*)pRow);
		}
		break;
		case GeoStar::Utility::GsRGBAType::eABGR32:
		{
			FromABGR32 convert;
			convert.ToARGBProcess(nLen/4, (unsigned int*)pRow);
		}
		break;
		case GeoStar::Utility::GsRGBAType::eRGBA32:
		{
			FromRGBA32 convert;
			convert.ToARGBProcess(nLen/4, (unsigned int*)pRow);
		}
		break;
		case GeoStar::Utility::GsRGBAType::eARGB32:
		{
			FromARGB32 convert;
			convert.ToARGBProcess(nLen / 4, (unsigned int*)pRow);
		}
		break;
		case GeoStar::Utility::GsRGBAType::eRGB24:
		{
			FromRGB24 convert;
			convert.ToARGBProcess(nLen/4, (unsigned int*)pRow);
		}
		case GeoStar::Utility::GsRGBAType::eBGR24:
		{
			FromBGR24 convert;
			convert.ToARGBProcess(nLen, (unsigned int*)pRow);
		}
		default:
			break;
	}
	
	memcpy( (unsigned char*)(m_Picture.argb) + nRowCount*nLen , pRow, nLen);
	return true;
}

void WEBPImageEncoder::End()
{
	WebPAuxStats stats;
	m_Picture.stats = &stats;
	if (!WebPEncode(&m_Config, &m_Picture)) 
	{
		GS_E<< "webp encoding error";
	}

	WebPPictureFree(&m_Picture);
}

void WEBPImageEncoder::Quality(int nQuality)
{
	if (nQuality < 0 || nQuality > 100)
		nQuality = 75;
	m_Config.quality = nQuality;
}

void WEBPImageEncoder::Speed(int speed)
{
	//修改配置参数可以改变编码质量和文件大小,控制是否无损压缩, 但是引入这个webp 就是为了减小大小,
	//建议在无文字的瓦片可以直接压缩默认, 有问题需要具体看效果
	if (speed < 0 || speed > 6)
		speed = 6;
	m_Config.method = speed;
}

void WEBPImageEncoder::RGBAType(UTILITY_NAME::GsRGBAType inputType)
{
	m_inputType = inputType;
}

WEBPImageDecoder::WEBPImageDecoder(const char* file)
{
	GsFile fileo(file);
	if (!fileo.Exists())
		return;
	m_Buff = (unsigned char*)malloc(sizeof(unsigned char)* fileo.Size());
	m_nLen = fileo.ReadAllBytes(m_Buff, fileo.Size());
	m_IsBuffOwn = true;
}
WEBPImageDecoder::WEBPImageDecoder(const unsigned  char* blob, int nLen )
{
	m_Buff = (unsigned  char* )blob;
	m_nLen = nLen;
}
WEBPImageDecoder::~WEBPImageDecoder()
{
	if (m_IsBuffOwn)
		free(m_Buff);
}
#include "src/webp/decode.h"

bool WEBPImageDecoder::Decode(int & w, int & h, int & bits, GsGrowByteBuffer * buff)
{
	WebPDecoderConfig config;
	WebPDecBuffer*  output_buffer = &config.output;
	WebPBitstreamFeatures*  bitstream = &config.input;
	if (!WebPInitDecoderConfig(&config)) {
		GS_E<<"WEBP Library version mismatch!\n";
		return false;
	}
	output_buffer->colorspace = MODE_RGBA;
	config.options.no_fancy_upsampling = 1;
	config.options.bypass_filtering = 1;
	config.options.use_threads = 1;
	config.output.colorspace = MODE_RGBA;
	VP8StatusCode status = VP8_STATUS_OK;
	size_t data_size = 0;
	status = WebPDecode(m_Buff, m_nLen, &config);
	if (VP8_STATUS_OK != status)
		return false;
	if (buff) {
		buff->Allocate(0);
		buff->Append(output_buffer->u.RGBA.rgba, output_buffer->u.RGBA.size);
		WebPFreeDecBuffer(output_buffer);
		w = output_buffer->width;
		h = output_buffer->height;
		if (w != 0 && h != 0)
			bits = output_buffer->u.RGBA.size / w / h * 8;
		else
			bits = 0;
	}
	return true;
}

bool WEBPImageDecoder::Decode(GsImageHeaderInfo* info)
{
	if (!info)
		return false;
	WebPBitstreamFeatures fea;
	VP8StatusCode code = WebPGetFeatures(m_Buff, m_nLen, &fea);

	info->EncodeType = GsImageEncodeType::eWEBP;
	info->Width = fea.width;
	info->Height = fea.height;
	info->RGBAType = fea.has_alpha ? GsRGBAType::eRGBA32 : GsRGBAType::eRGB24;
	fea.has_animation;// 是否包含动画
	fea.format;// 是否是无损图像，有损图像。混合模式等等
	
	return code == VP8StatusCode::VP8_STATUS_OK;
}

MemRGBAVisitor::MemRGBAVisitor(UTILITY_NAME::GsGrowByteBuffer* pBuffer, int* w, int* h) :m_W(w), m_H(h), m_pBuffer(pBuffer)
{

}

void MemRGBAVisitor::Alloc(int w, int h)
{
	*m_W = w;
	*m_H = h;
	m_pBuffer->Allocate(w * h * 4);
}

unsigned char* MemRGBAVisitor::Row(int i)
{
	return m_pBuffer->BufferHead() + (i * *m_W * 4);
}