#include "pch.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "ktx.h"
#include "vkformat_enum.h"
#include "vk_format.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <omp.h>
#include <algorithm>
#include <regex>
#include "aimage.h"
#include "b64.h"
#include <map>

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
using namespace std;
#ifndef PI
#define PI 3.1415926535897932
#endif
/// \brief ��ȡԲ����PI
double AMath::Pi()
{
	return PI;
}

/// \brief ��ȡ��Ȼ����
double AMath::Log(double val)
{
	return log(val);
}

/// \brief ��ת��Ϊ����
double AMath::ToRadian(double degree)
{
	static double d2r = PI / 180.0;
	return degree * d2r;
}
/// \brief ����ת��Ϊ��
double AMath::ToDegree(double radian)
{
	static double r2d = 180.0 / PI;
	return radian * r2d;
}
/// \brief ��Чfloat
/// \details Not-A-Number����һ�����ֵ���д�����Ա�ʾ��Ч��floatֵ��0/0.����Ϊ0�Ľ��
float AMath::NaNF()
{
	static float d = nanf("");
	return d;
}
/// \brief ��Чdouble
double AMath::NaN()
{
	static double d = nan("");
	return d;
}
/// \brief �ж�һ��doubleֵ�Ƿ�����Ч��Doubleֵ
bool AMath::IsNaN(double val)
{
	return isnan(val);
}


bool AMath::IsEqual(double d1, double d2, double tol)
{
	return  fabs(d1 - d2) < tol;
}
bool AMath::IsEqual(float f1, float f2, float tol)
{
	return  fabs(f1 - f2) < tol;
}

/// \brief ���������뵽2��N�η�
// From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
unsigned int AMath::RoundUpPowerOf2(unsigned int n)
{
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	++n;
	return n;
}

/// \brief ���뵽2��N�η�
unsigned int AMath::NextPowerOfTwo(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}
/// \brief ���뵽2��N�η�
unsigned long long AMath::NextPowerOfTwo(unsigned long long v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;

}

/// \brief �ж������Ƿ���뵽2��N�η�
bool AMath::IsPowerOfTwo(unsigned long long v)
{
	return ((v != 0) && ((v & (v - 1)) == 0));
}

bool AMath::IsEqual(double d1, double d2)
{
	return  fabs(d1 - d2) < DBL_EPSILON;
}
bool AMath::IsEqual(float f1, float f2)
{
	return  fabs(f1 - f2) < FLT_EPSILON;
}
double AMath::Round(double r, unsigned int place)
{
	if (place > 15 || r == 0.0) return r;
	r > 0 ? r += pow(0.1, 15) * 5 : r -= pow(0.1, 15) * 5;
	long long i = pow(10, place);
	r *= i;
	r = floor(r > 0.0 ? r + 0.5 : r - 0.5);
	return r / i;
}

float AMath::Round(float r, unsigned int place)
{
	if (place > 6 || r == 0.0) return r;
	r > 0 ? r += pow(0.1, 6) * 5 : r -= pow(0.1, 6) * 5;
	int i = pow(10, place);
	r *= i;
	r = floor(r > 0.0 ? r + 0.5 : r - 0.5);
	return r / i;
}

AByteBuffer::operator char* ()const
{
	return PtrT<char>();
}

AByteBuffer::operator int* ()const
{
	return PtrT<int>();
}
AByteBuffer::operator unsigned int* ()const
{
	return PtrT<unsigned int>();
}
AByteBuffer::operator short* ()const
{
	return PtrT<short>();
}
AByteBuffer::operator unsigned short* ()const
{
	return PtrT<unsigned short>();
}
AByteBuffer::operator long long* ()const
{
	return PtrT<long long>();
}
AByteBuffer::operator unsigned long long* ()const
{
	return PtrT< unsigned long long>();
}
AByteBuffer::operator double* ()const
{
	return PtrT< double>();
}
AByteBuffer::operator float* ()const
{
	return PtrT< float>();
}
AByteBuffer::operator bool* ()const
{
	return PtrT< bool>();
}

AByteBuffer::operator unsigned char* ()const
{
	return PtrT<unsigned char>();
}

AByteBuffer::AByteBuffer()
{
}
AByteBuffer::~AByteBuffer()
{
}
/// \brief ��������βָ��
/// \details >=��ָ���ʾ�����˻�������Χ
unsigned char* AByteBuffer::EndPtr() const
{
	return Ptr() + BufferSize();
}

/// \brief ���������ڴ��ת��Ϊbase64�����ַ���
std::string AByteBuffer::ToBase64() const
{
	if (this->BufferSize() == 0) return "";
	std::stringstream ss;
	b64_encode(this->Ptr(), this->BufferSize(), ss);

	return ss.str();
}

bool __IsBase64(char b)
{
	return (
		(b >= '0' && b <= '9') ||
		(b >= 'a' && b <= 'z') ||
		(b >= 'A' && b <= 'Z') ||
		b == '+' ||
		b == '/' ||
		b == '\r' ||
		b == '\n' ||
		b == '=');
}
bool IsNullOrEmpty(const char* str)
{
	return NULL == str || *str == 0;
}

/// \brief �ж��ַ����Ƿ���һ��base64������ַ���
bool AByteBuffer::IsBase64(const char* strBase64)
{
	if (IsNullOrEmpty(strBase64)) return false;
	int n = strlen(strBase64);
	for (int i = 0; i < n; i++)
	{
		if (!__IsBase64(strBase64[i]))
			return false;
	}
	return true;
}
/// \brief ��β������һ���ַ���
/// \param str �����ַ���
/// \return 
unsigned char* AByteBuffer::Append(const std::string& str) {
	return Append((const unsigned char*)str.data(), str.size());
}


void B64CallFun(void* pOpaque, const unsigned char b)
{
	AByteBuffer* pBuffer = (AByteBuffer*)pOpaque;
	pBuffer->AppendT(b);
}
/// \brief ��Base64�ַ��������Ϊ�ڴ��
bool AByteBuffer::FromBase64(const char* strBase64)
{
	if (strBase64 == NULL || strlen(strBase64) == 0)
		return false;

	int nLen = strlen(strBase64);
	Allocate(b64_decod_length(nLen));
	int realLen = BufferSize();
	Allocate(0);
	b64_decode(strBase64, nLen, B64CallFun, this);
	if (realLen > BufferSize())
		Ptr()[BufferSize()] = '\0';
	return this->BufferSize() > 0;

}



AGrowByteBuffer::AGrowByteBuffer(int nLen)
{
	m_pBuffer = 0;
	Allocate(nLen);
}
AGrowByteBuffer::~AGrowByteBuffer()
{
	Reset();
}

AGrowByteBuffer::AGrowByteBuffer(unsigned char* pBuffer)
{
	m_pBuffer = nullptr;
	Copy(pBuffer, strlen((const char*)pBuffer));
}
/// \brief �ƶ�����
AGrowByteBuffer::AGrowByteBuffer(AGrowByteBuffer&& rhs)
{
	m_pBuffer = NULL;
	Swap(rhs);
}
/// \brief ��������
AGrowByteBuffer::AGrowByteBuffer(const AGrowByteBuffer& rhs)
{
	m_pBuffer = NULL;
	Append(rhs.BufferHead(), rhs.BufferSize());
}
AGrowByteBuffer::AGrowByteBuffer(const unsigned char* pBuffer, int nLen)
{
	m_pBuffer = NULL;
	Copy(pBuffer, nLen);
}
void AGrowByteBuffer::Attach(unsigned char* pBuffer)
{
	Reset();
	m_pBuffer = pBuffer;
}
unsigned char* AGrowByteBuffer::Detach()
{
	unsigned char* pBuffer = m_pBuffer;
	m_pBuffer = NULL;
	return pBuffer;
}


AGrowByteBuffer::operator unsigned char* () const
{
	return Ptr();
}

unsigned char* AGrowByteBuffer::Ptr() const
{
	if (!m_pBuffer)
		return 0;
	return m_pBuffer + 8;
}
unsigned char* AGrowByteBuffer::BufferHead() const
{
	return Ptr();
}
unsigned int AGrowByteBuffer::BufferSize()const
{
	if (0 == m_pBuffer)
		return 0;
	return m_pMeta->Size;
}
unsigned int AGrowByteBuffer::RealSize() const
{
	if (0 == m_pBuffer)
		return 0;
	return m_pMeta->Real;
}
unsigned char* AGrowByteBuffer::SetBufferValue(int nValue)
{
	if (0 == m_pBuffer)
		return 0;
	memset(Ptr(), nValue, RealSize());
	return Ptr();
}

unsigned char* AGrowByteBuffer::Append(const unsigned char* pBuff, int nLen)
{
	if (nLen <= 0)
		return Ptr();
	int nOld = BufferSize();
	Allocate(nOld + nLen);
	memcpy(Ptr() + nOld, pBuff, nLen);
	return Ptr();
}
unsigned char* AGrowByteBuffer::Append(const char* pStr)
{
	if (0 == pStr)
		return Ptr();
	return Append((unsigned char*)pStr, strlen(pStr));
}
unsigned char* AGrowByteBuffer::Insert(unsigned int nPos, const unsigned char* pStr, int nLen)
{
	if (nPos + nLen > RealSize())
		Allocate(nPos + nLen);
	memcpy(Ptr() + nPos, pStr, nLen);
	return Ptr();
}
unsigned char* AGrowByteBuffer::Allocate(unsigned int nLen)
{
	if (BufferSize() == nLen)
		return Ptr();
	if (RealSize() < nLen)
	{
		if (0 == m_pBuffer)
			m_pBuffer = (unsigned char*)malloc(nLen + 8);
		else
			m_pBuffer = (unsigned char*)realloc(m_pBuffer, nLen + 8);
		m_pMeta->Real = nLen;
	}
	m_pMeta->Size = nLen;
	return Ptr();
}
void AGrowByteBuffer::Clear()
{
	if (0 == m_pBuffer)
		return;
	*(unsigned int*)(m_pBuffer + 4) = 0;
}
void AGrowByteBuffer::Reset()
{
	if (0 == m_pBuffer)
		return;
	free(m_pBuffer);
	m_pBuffer = 0;
}
unsigned char* AGrowByteBuffer::Copy(const unsigned char* pBuff, int nLen)
{
	if (BufferSize() < nLen)
		Allocate(nLen);
	memcpy(m_pBuffer + 8, pBuff, nLen);
	return Ptr();
}
/// \brief ��ֵ
AGrowByteBuffer& AGrowByteBuffer::operator=(AGrowByteBuffer&& rsh)
{
	Swap(rsh);
	return *this;
}
AGrowByteBuffer* AGrowByteBuffer::Swap(unsigned char* pBuff)
{
	std::swap(m_pBuffer, pBuff);
	return this;
}
/// \brief ��ֵ
AGrowByteBuffer& AGrowByteBuffer::operator=(const AGrowByteBuffer& rsh)
{
	// ͬһ��ָ�벻��������
	if ((void*)this == (void*)&rsh)
		return *this;
	Allocate(0);
	Append(rsh.BufferHead(), rsh.BufferSize());
	return *this;
}
/// \brief ��������������������
AGrowByteBuffer* AGrowByteBuffer::Swap(AGrowByteBuffer& rhs)
{
	std::swap(m_pBuffer, rhs.m_pBuffer);
	return this;
}


APT::APT()
{
	X = 0;
	Y = 0;
}
APT::APT(const APT& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
}
APT::APT(int x, int y)
{
	X = x;
	Y = y;
}

/// \brief ��Offset��
APT APT::operator +(const APT& pt) const
{
	return APT(X + pt.X, Y + pt.Y);
}

/// \brief offset����
APT& APT::operator +=(const APT& pt)
{
	X += pt.X;
	Y += pt.Y;
	return *this;
}


/// \brief ��Offset��
APT APT::operator -(const APT& pt) const
{
	return APT(X - pt.X, Y - pt.Y);
}

/// \brief offset����
APT& APT::operator -=(const APT& pt)
{
	X -= pt.X;
	Y -= pt.Y;
	return *this;
}




APTF::APTF()
{
	X = 0;
	Y = 0;
}
APTF::APTF(const APT& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
}
APTF::APTF(const APTF& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
}
APTF::APTF(int x, int y)
{
	X = x;
	Y = y;
}
APTF::APTF(float x, float y)
{
	X = x;
	Y = y;
}

/// \brief ��Offset��
APTF APTF::operator +(const APTF& pt) const
{
	return APTF(X + pt.X, Y + pt.Y);
}

/// \brief offset����
APTF& APTF::operator +=(const APTF& pt)
{
	X += pt.X;
	Y += pt.Y;
	return *this;
}


/// \brief ��Offset��
APTF APTF::operator -(const APTF& pt) const
{
	return APTF(X - pt.X, Y - pt.Y);
}

/// \brief offset����
APTF& APTF::operator -=(const APTF& pt)
{
	X -= pt.X;
	Y -= pt.Y;
	return *this;
}



ASize::ASize()
{
	Width = 0;
	Height = 0;
}
ASize::ASize(const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
}
ASize::ASize(int w, int h)
{
	Width = w;
	Height = h;
}
/// \brief �ӵ㹹��
ASize::ASize(const APT& p)
{
	Width = p.X;
	Height = p.Y;
}

/// \brief �Ƿ����
bool ASize::operator == (const ASize& s)const
{
	return s.Width == Width && s.Height == Height;
}
/// \brief ����
ASize& ASize::operator = (const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief �Ƿ񲻵�
bool ASize::operator != (const ASize& s)const
{
	return s.Width != Width || s.Height != Height;
}
/// \brief ���߹��ɵľ������
int ASize::Area()const
{
	return Width * Height;
}



ASizeF::ASizeF()
{
	Width = 0;
	Height = 0;
}
ASizeF::ASizeF(const ASizeF& s)
{
	Width = s.Width;
	Height = s.Height;
}
ASizeF::ASizeF(const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
}
ASizeF::ASizeF(float w, float h)
{
	Width = w;
	Height = h;
}

/// \brief �Ƿ����
bool ASizeF::operator == (const ASizeF& s)const
{
	return AMath::IsEqual(s.Width, Width) &&
		AMath::IsEqual(s.Height, Height);
}
ASizeF& ASizeF::operator = (const ASizeF& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
ASizeF& ASizeF::operator = (const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}

/// \brief �Ƿ񲻵�
bool ASizeF::operator != (const ASizeF& s)const
{
	return !AMath::IsEqual(s.Width, Width) ||
		!AMath::IsEqual(s.Height, Height);
}



/// \brief ȱʡ���캯��
ASizeD::ASizeD() :Width(0), Height(0)
{

}
/// \brief ���ع��캯��
ASizeD::ASizeD(const ASize& s) : Width(s.Width), Height(s.Height)
{
}
/// \brief ���ع��캯��
ASizeD::ASizeD(const ASizeF& s) : Width(s.Width), Height(s.Height)
{

}
/// \brief ���ع��캯��
ASizeD::ASizeD(const ASizeD& s) : Width(s.Width), Height(s.Height)
{

}

/// \brief ���ع��캯��
ASizeD::ASizeD(double w, double h) :Width(w), Height(h)
{

}

/// \brief �Ƿ����
bool ASizeD::operator == (const ASizeD& s)const
{
	return AMath::IsEqual(s.Width, Width) && AMath::IsEqual(s.Height, Height);
}
/// \brief �Ƿ񲻵�
bool ASizeD::operator != (const ASizeD& s)const
{
	return !AMath::IsEqual(s.Width, Width) || !AMath::IsEqual(s.Height, Height);
};
/// \brief ��ֵ
ASizeD& ASizeD::operator = (const ASizeD& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief ��ֵ
ASizeD& ASizeD::operator = (const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief ��ֵ
ASizeD& ASizeD::operator = (const ASizeF& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}



ARect::ARect()
{
	Left = 0;
	Right = 0;
	Top = 0;
	Bottom = 0;
}
ARect::ARect(const APT& pt, const ASize& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X + s.Width;
	Bottom = pt.Y + s.Height;
}
ARect::ARect(const ARect& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
ARect::ARect(int x, int y, int w, int h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom = y + h;
}
int ARect::Width()const
{
	return Right - Left;
}
int ARect::Height()const
{
	return Bottom - Top;
}
ARect& ARect::Offset(const APT& pt)
{
	Left += pt.X;
	Top += pt.Y;
	Right += pt.X;
	Bottom += pt.Y;
	return *this;
}
/// \brief ���ĵ�
APT ARect::Center()const
{
	return APT((Left + Right) / 2, (Top + Bottom) / 2);
}
/// \brief �Ƿ��ཻ
bool ARect::IsIntersects(const ARect& rhs)const
{
	return !IsDisjoin(rhs);
}
/// \brief ���������ཻ�Ĳ���
ARect ARect::Intersects(const ARect& rhs) const
{
	if (IsDisjoin(rhs))
		return ARect();
	ARect rect;
	rect.Left = AMath::Max(rhs.Left, Left);
	rect.Top = AMath::Max(rhs.Top, Top);

	rect.Right = AMath::Min(rhs.Right, Right);
	rect.Bottom = AMath::Min(rhs.Bottom, Bottom);

	return rect;
}


//����������me�Ƿ��env����
template<class T>
bool IsDisjoinT(T xmin, T ymin, T xmax, T ymax, T sx, T sy, T ex, T ey)
{
	//�����Ǿ���ѡ���߶��в������
	double dx, dy, x0, x1, y0, y1;

	dx = ex - sx;
	dy = ey - sy;

	x0 = xmin - sx;
	x1 = xmax - sx;
	y0 = ymin - sy;
	y1 = ymax - sy;

	bool a, b;
	a = x0 * dy > y0 * dx;
	b = x0 * dy > y1 * dx;
	if (a ^ b)
		return false;
	b = x1 * dy > y0 * dx;
	if (a ^ b)
		return false;
	b = x1 * dy > y1 * dx;
	if (a ^ b)
		return false;
	return true;
}

/// \brief �Ƿ�����ཻ
bool ARect::IsDisjoin(int x1, int y1, int x2, int y2)const
{
	//��Ӿ������������롣
	ARect box(AMath::Min(x1, x2), AMath::Min(y1, y2), AMath::Max(x1, x2), AMath::Max(y1, y2));
	if (IsDisjoin(box))
		return true;
	return IsDisjoinT(Left, Top, Right, Bottom, x1, y1, x2, y2);
}
/// \brief �Ƿ�����
bool ARect::IsDisjoin(const ARect& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top || Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief �Ƿ����
bool ARect::IsContain(int x, int y)const
{
	return x >= Left && x < Right&& y >= Top && y < Bottom;
}

/// \brief �Ƿ����
bool ARect::IsContain(const APT& pt)const
{
	return IsContain(pt.X, pt.Y);
}

/// \brief �Ƿ����
bool ARect::IsContain(const ARect& rhs)const
{
	return IsContain(rhs.Left, rhs.Top) && IsContain(rhs.Right - 1, rhs.Bottom - 1);
}
/// \brief ��λ��
APT ARect::Location()const
{
	return LeftTop();
}

/// \brief ��С
ASize ARect::Size()const
{
	return ASize(Width(), Height());
}

/// \brief ���Ͻ�
APT ARect::LeftTop()const
{
	return APT(Left, Top);
}


/// \brief ���Ͻ�
APT ARect::RightTop()const
{
	return APT(Right, Top);
}


/// \brief ���½�
APT ARect::LeftBottom()const
{
	return APT(Left, Bottom);
}


/// \brief ���½�
APT ARect::RightBottom()const
{
	return APT(Right, Bottom);
}
/// \brief ����ƫ��
ARect ARect::operator + (const APT& pt)const
{
	return ARect(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief ����ƫ��
ARect ARect::operator - (const APT& pt)const
{
	return ARect(Left - pt.X, Top - pt.Y, Width(), Height());
}

/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
ARect ARect::OffsetTo(const APT& pt)const
{
	return ARect(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief ���κϲ�
ARect ARect::operator + (const ARect& rect)const
{
	ARect ret(*this);
	ret.Union(rect);
	return ret;
}
/// \brief �ϲ�������
ARect& ARect::operator += (const ARect& rect)
{
	return Union(rect);
}

/// \brief ���κϲ�
ARect& ARect::Union(const ARect& rect)
{
	Left = AMath::Min(Left, rect.Left);
	Top = AMath::Min(Top, rect.Top);
	Right = AMath::Max(Right, rect.Right);
	Bottom = AMath::Max(Bottom, rect.Bottom);
	return *this;
}




ARectF::ARectF()
{
	Left = 0;
	Right = 0;
	Top = 0;
	Bottom = 0;
}

ARectF::ARectF(const APTF& pt, const ASizeF& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X + s.Width;
	Bottom = pt.Y + s.Height;
}

ARectF::ARectF(const APT& pt, const ASize& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X + s.Width;
	Bottom = pt.Y + s.Height;
}
ARectF::ARectF(const ARect& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
ARectF::ARectF(const ARectF& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
ARectF::ARectF(int x, int y, int w, int h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom = y + h;
}
ARectF::ARectF(float x, float y, float w, float h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom = y + h;
}
float ARectF::Width()const
{
	return Right - Left;
}
float ARectF::Height()const
{
	return Bottom - Top;
}
/// \brief ���ĵ�
APTF ARectF::Center()const
{
	return APTF((Left + Right) / 2, (Top + Bottom) / 2);
}
ARectF& ARectF::Offset(const APTF& pt)
{
	Left += pt.X;
	Top += pt.Y;
	Right += pt.X;
	Bottom += pt.Y;
	return *this;
}

/// \brief �Ƿ��ཻ
bool ARectF::IsIntersects(const ARectF& rhs)const
{
	return !IsDisjoin(rhs);
}

/// \brief ���������ཻ�Ĳ���
ARectF ARectF::Intersects(const ARectF& rhs) const
{
	if (IsDisjoin(rhs))
		return ARect();
	ARectF rect;
	rect.Left = AMath::Max(rhs.Left, Left);
	rect.Top = AMath::Max(rhs.Top, Top);

	rect.Right = AMath::Min(rhs.Right, Right);
	rect.Bottom = AMath::Min(rhs.Bottom, Bottom);

	return rect;
}


/// \brief �Ƿ�����ཻ
bool ARectF::IsDisjoin(float x1, float y1, float x2, float y2)const
{
	//��Ӿ������������롣
	ARectF box((float)AMath::Min(x1, x2), AMath::Min(y1, y2), AMath::Max(x1, x2), AMath::Max(y1, y2));
	if (IsDisjoin(box))
		return true;
	return IsDisjoinT(Left, Top, Right, Bottom, x1, y1, x2, y2);
}
/// \brief �Ƿ�����
bool ARectF::IsDisjoin(const ARectF& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top || Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief �Ƿ����
bool ARectF::IsContain(float x, float y)const
{

	return x >= Left && x < Right&& y >= Top && y < Bottom;
}

/// \brief �Ƿ����
bool ARectF::IsContain(const APTF& pt)const
{
	return IsContain(pt.X, pt.Y);
}

/// \brief �Ƿ����
bool ARectF::IsContain(const ARectF& rhs)const
{

	return IsContain(rhs.Left, rhs.Top) && IsContain(rhs.Right, rhs.Bottom);
}

/// \brief ��λ��
APTF ARectF::Location()const
{
	return LeftTop();
}

/// \brief ��С
ASizeF ARectF::Size()const
{
	return ASizeF(Width(), Height());
}

/// \brief ���Ͻ�
APTF ARectF::LeftTop()const
{
	return APTF(Left, Top);
}


/// \brief ���Ͻ�
APTF ARectF::RightTop()const
{
	return APTF(Right, Top);
}


/// \brief ���½�
APTF ARectF::LeftBottom()const
{
	return APTF(Left, Bottom);
}


/// \brief ���½�
APTF ARectF::RightBottom()const
{
	return APTF(Right, Bottom);
}
/// \brief ����ƫ��
ARectF ARectF::operator + (const APTF& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief ����ƫ��
ARectF ARectF::operator - (const APTF& pt)const
{
	return ARectF(Left - pt.X, Top - pt.Y, Width(), Height());
}


/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
ARectF ARectF::OffsetTo(const APTF& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}



/// \brief ����ƫ��
ARectF ARectF::operator + (const APT& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}


/// \brief ����ƫ��
ARectF ARectF::operator - (const APT& pt)const
{
	return ARectF(Left - pt.X, Top - pt.Y, Width(), Height());
}

/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
ARectF ARectF::OffsetTo(const APT& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}
/// \brief ���κϲ�
ARectF ARectF::operator + (const ARectF& rect)const
{
	ARectF ret(*this);
	ret.Union(rect);
	return ret;
}
/// \brief �ϲ�������
ARectF& ARectF::operator += (const ARectF& rect)
{
	return Union(rect);
}

/// \brief ���κϲ�
ARectF& ARectF::Union(const ARectF& rect)
{
	Left = AMath::Min(Left, rect.Left);
	Top = AMath::Min(Top, rect.Top);
	Right = AMath::Max(Right, rect.Right);
	Bottom = AMath::Max(Bottom, rect.Bottom);
	return *this;
}
#pragma region imageIO 
//Ӱ��IO
class ImageIO
{
public:
	virtual ~ImageIO() {}
	virtual unsigned int  Write(const unsigned char* pBlob, int nLen) = 0;
	virtual unsigned int Read(unsigned char* pBlob, int nLen) = 0;

	virtual bool Rewind() = 0;
	virtual bool Eof() = 0;
	AImageEncodeType ImageType();
};
//��д�ļ���ioʵ��
class ImageFile :public ImageIO
{
	std::fstream m_fStream;
	std::fstream::pos_type m_nLen;
public:
	ImageFile(const char* file, bool bIn = false);
	virtual unsigned int  Write(const unsigned char* pBlob, int nLen);
	virtual unsigned int Read(unsigned char* pBlob, int nLen);
	virtual bool Eof();
	virtual bool Rewind();
	bool IsOpen();
};
//��д�ڴ��е����ݵ�ioʵ��
class ImageBuffer :public ImageIO
{
	AByteBuffer* m_pBuffer;
	unsigned int  m_Index;
public:
	ImageBuffer(AByteBuffer* pBuffer);
	virtual unsigned int Write(const unsigned char* pBlob, int nLen);
	virtual unsigned int Read(unsigned char* pBlob, int nLen);
	virtual bool Eof();
	virtual bool Rewind();
};
//�ڴ��е�ֻ��ioʵ��
class ReadImageBuffer :public ImageIO
{
	const unsigned char* m_Buff;
	unsigned int   m_nLen;
	unsigned int  m_Index;
public:
	ReadImageBuffer(const unsigned char* buff, int nLen);
	virtual unsigned int Write(const unsigned char* pBlob, int nLen);
	virtual unsigned int Read(unsigned char* pBlob, int nLen);
	virtual bool Eof();
	virtual bool Rewind();
	virtual unsigned int Seek(int nLen,int nwhere);
	virtual unsigned int Offset() {
		return m_Index;
	}
};

int read_stbi(void* user, char* data, int size)    // fill 'data' with 'size' bytes.  return number of bytes actually read
{
	ImageIO* img = (ImageIO*)user;
	return img->Read((unsigned char*)data, size);
}
void skip_stbi(void* user, int n)                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
{
	ImageIO* img = (ImageIO*)user;
	unsigned char buff[1024];
	while (n > 0)
	{
		int s = AMath::Min(1024, n);
		img->Read(buff, s);
		n -= s;
	}
}
int eof_stbi(void* user)                        // returns nonzero if we are at end of file/data
{
	ImageIO* img = (ImageIO*)user;
	return img->Eof();
}

AImageEncodeType ImageIO::ImageType()
{
	// Determine the true image type using link:
	// http://en.wikipedia.org/wiki/List_of_file_signatures
	char buffer[12] = { 0 };
	if (Read((unsigned char*)buffer, 12) < 12) return eUnknownImage;

	if (strncmp(buffer, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) == 0)
		return ePNG;
	if (buffer[0] == '\xff' && buffer[1] == '\xd8' && buffer[2] == '\xff')
		return eJPG;
	if (buffer[0] == '\x4D' && buffer[1] == '\x4D')
		return eTIFF;
	if (buffer[0] == '\x49' && buffer[1] == '\x49')
		return eTIFF;

	if (buffer[0] == 'B' && buffer[1] == 'M')
		return eBMP;
	if (buffer[0] == 'D' && buffer[1] == 'N' && buffer[2] == 'G')
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
	std::string f = buffer;
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
ImageFile::ImageFile(const char* file, bool bIn)
{
	if (bIn)
	{
		m_fStream.open(file, std::ios::in | std::ios::binary);
		m_fStream.seekg(0, std::ios::end);
		m_nLen = m_fStream.tellg();
		m_fStream.seekg(0, std::ios::beg);
	}
	else
		m_fStream.open(file, std::ios::out | std::ios::binary);

}
bool ImageFile::Rewind()
{
	if (m_fStream.eof())
		m_fStream.clear(std::ios::eofbit);
	m_fStream.seekg(0, std::ios::beg);
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
unsigned int ImageFile::Read(unsigned char* pBlob, int nLen)
{
	std::fstream::pos_type pos = m_fStream.tellg();
	if (NULL == pBlob)
		m_fStream.seekg(nLen, std::ios::cur);
	else
		m_fStream.read((char*)pBlob, nLen);
	if (m_fStream.eof())
		return m_nLen - pos;
	return m_fStream.tellg() - pos;
}
unsigned int ImageFile::Write(const unsigned char* pBlob, int nLen)
{
	m_fStream.write((const char*)pBlob, nLen);
	return nLen;
}
ReadImageBuffer::ReadImageBuffer(const unsigned char* buff, int nLen)
{
	m_Buff = buff;
	m_nLen = nLen;
	m_Index = 0;
}
bool ReadImageBuffer::Eof()
{
	return m_Index >= m_nLen;
}
unsigned int ReadImageBuffer::Write(const unsigned char* pBlob, int nLen)
{
	return 0;
}
unsigned int ReadImageBuffer::Read(unsigned char* pBlob, int nLen)
{
	if (m_Index >= m_nLen)
		return 0;
	int nRead = nLen;
	if (m_Index + nRead > m_nLen)
		nRead = m_nLen - m_Index;
	if (pBlob)
		memcpy(pBlob, m_Buff + m_Index, nRead);
	m_Index += nRead;
	return nRead;
}
bool ReadImageBuffer::Rewind()
{
	m_Index = 0;
	return true;
}
unsigned int ReadImageBuffer::Seek(int nOffset, int nWhence)
{
	if (nOffset < 0 || nOffset > m_nLen)
		return -1;
	if (nWhence == SEEK_SET)
	{
		m_Index = nOffset;
	}
	else if (nWhence == SEEK_CUR)
	{
		m_Index = m_Index + nOffset;
	}
	else
	{
		m_Index = m_nLen + nOffset;
	}
	//beof = false;
	return 0;
}

ImageBuffer::ImageBuffer(AByteBuffer* pBuffer)
{
	m_pBuffer = pBuffer;
	m_Index = 0;
}
bool ImageBuffer::Eof()
{
	return m_Index >= m_pBuffer->BufferSize();
}
unsigned int  ImageBuffer::Write(const unsigned char* pBlob, int nLen)
{
	m_pBuffer->Append(pBlob, nLen);
	return nLen;
}
unsigned int ImageBuffer::Read(unsigned char* pBlob, int nLen)
{
	if (m_Index >= m_pBuffer->BufferSize())
		return 0;
	int nRead = nLen;
	if (m_Index + nRead > m_pBuffer->BufferSize())
		nRead = m_pBuffer->BufferSize() - m_Index;
	if (pBlob)
		memcpy(pBlob, m_pBuffer->BufferHead() + m_Index, nRead);
	m_Index += nRead;
	return nRead;
}
bool ImageBuffer::Rewind()
{
	m_Index = 0;
	return true;
}
#pragma endregion 



AImage::AImage()
{
	m_Info.RGBAType  = AColorBandType::eRGBA32;
}
AImage::AImage(unsigned int nWidth, unsigned int nHeight, AImage* img, const ARect& extent)
{
}
AImage::AImage(unsigned int nWidth, unsigned int nHeight, const unsigned char* raw, int nBpp)
{
}
AImage::AImage(unsigned int nWidth, unsigned int nHeight, AColorBandType eColorType)
{
}
AImage::AImage(const char* strFile)
{
}


#include "tif/tiffio.h"
static tmsize_t
tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	ReadImageBuffer* io = (ReadImageBuffer*)fd;
	return io->Read((unsigned char*)buf, size);
}
static tmsize_t
tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
	ReadImageBuffer* io = (ReadImageBuffer*)fd;
	return io->Write((unsigned char*)buf, size);
}
static uint64
tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	ReadImageBuffer* io = (ReadImageBuffer*)fd;
	io->Seek(off, whence);
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

crnlib::texture_file_types::format ToTextureFileType(AImageEncodeType eType)
{
	if (eType == eDDS)
		return crnlib::texture_file_types::cFormatDDS;
	if (eType == eKTX)
		return crnlib::texture_file_types::cFormatKTX;

	if (eType == eCRN)
		return crnlib::texture_file_types::cFormatCRN;

	return crnlib::texture_file_types::cFormatInvalid;
}

/// \brief ����ѹ����ʽ
enum ATexturePixelFormat
{
	eDXT1,
	eDXT2,
	eDXT3,
	eDXT4,
	eDXT5,
	e3DC,
	eDXN,
	eDXT5A,
	eDXT5_CCxY,
	eDXT5_xGxR,
	eDXT5_xGBR,
	eDXT5_AGBR,
	eDXT1A,
	eETC1,
	eR8G8B8,
	eL8,
	eA8,
	eA8L8,
	eA8R8G8B8,
};
ATexturePixelFormat ToPixelFormat(crnlib::pixel_format f)
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
#include "src/webp/decode.h"
AImage::AImage(const unsigned char* blob, int nLen)
{
	m_LoadOK = Init(blob, nLen);
}
bool AImage::Init(const unsigned char* blob, int nLen)
{
	if (blob == NULL || nLen <= 0) return false;

	ReadImageBuffer io(blob, nLen);
	AImageEncodeType eType = io.ImageType();
	m_Info.EncodeType = eType;
	m_Info.RGBAType = AColorBandType::eRGBA32;
	io.Rewind();
	if (eType == eUnknownImage) return false;
	if (eType == ePNG|| eType == eJPG || eType == eBMP || eType == eGIF ||eType == eTGA)
	{
		//unsigned char* data = stbi_load_from_memory(blob,  nLen, 
		//	&m_Info.Width, &m_Info.Height, &m_Info.Bpp, 0);	

		stbi__context ctx;
		memset(&ctx, 0, sizeof(stbi__context));
		ctx.io_user_data = &io;
		stbi_io_callbacks cb;
		cb.eof = eof_stbi;
		cb.read = read_stbi;
		cb.skip = skip_stbi;
		io.Rewind();

		int com;
		stbi_uc* data = stbi_load_from_callbacks(&cb, &io, &m_Info.Width, &m_Info.Height, &com, 4);
		if (!data)
			return false;
		m_Info.Bpp = com * 8;
		if(com == 4)
			m_Info.RGBAType = AColorBandType::eRGBA32;
		else
			m_Info.RGBAType = AColorBandType::eRGB24;
		m_Buffer.Append(data, m_Info.Width*m_Info.Height * com);
		STBI_FREE(data);
	}
	else if (eType >= eKTX && eType <= eCRN)
	{
		if (m_Info.EncodeType == eKTX2)
		{
			ktxTexture* pKtx2;
			ktx_error_code_e result;
			if (blob) {
				result = ktxTexture2_CreateFromMemory(blob, nLen, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, (ktxTexture2**)&pKtx2);
			}
			
			if (KTX_SUCCESS != result)
			{
				return false;
			}
			m_Info.Width = pKtx2->baseWidth;
			m_Info.Height = pKtx2->baseHeight;
			m_Buffer.Allocate(m_Info.Width * m_Info.Height * 4);
			memcpy((void*)m_Buffer.BufferHead(), pKtx2->pData, pKtx2->dataSize);

			return true;
		}
		//����GIF�϶����ɻ�
		if (m_Info.EncodeType < eKTX || m_Info.EncodeType >eCRN)
			return false;

		crnlib::mipmapped_texture tex;
		//if (!m_File.empty())
		//{
		//	if (!tex.read_from_file(m_File.c_str(), ToTextureFileType(m_eType)))
		//		return false;
		//}
		//else
		{
			crnlib::buffer_stream stream(blob,  nLen);
			crnlib::data_stream_serializer ser(stream);
			if (!tex.read_from_stream(ser, ToTextureFileType(m_Info.EncodeType)))
				return false;
		}

		if (tex.get_num_faces() <= 0)
			return false;

		m_Info.Width = tex.get_width();
		m_Info.Height = tex.get_height();
		//m_ePixelType = ToPixelFormat(tex.get_format());
		//m_nMipMap = tex.get_num_levels() - 1;
		if (m_Info.Width <= 0 || m_Info.Height <= 0)
			return false;

		//�������32λ��ɫ��ת��֮��
		if (tex.get_format() != crnlib::PIXEL_FMT_A8R8G8B8)
		{
			crnlib::dxt_image::pack_params pack_params;
			pack_params.m_compressor = cCRNDXTCompressorCRN;
			if (!tex.convert(crnlib::PIXEL_FMT_A8R8G8B8, pack_params))
				return false;
			m_Info.RGBAType = AColorBandType::eARGB32;
		}
		//ȡһ֡
		crnlib::mip_ptr_vec& vec = tex.get_face(0);
		for (int i = 0; i < vec.size() && i < 1; i++)
		{
			crnlib::mip_level* level = vec.at(i);
			this->m_Buffer.Allocate(level->get_total_pixels() * 4);
			memcpy((void*)m_Buffer.BufferHead(), level->get_image()->get_pixels(), level->get_total_pixels() * 4);
		}
		return true;
	}
	else if (eType == eTIFF)
	{
		TIFF* tif = TIFFClientOpen("tiff", "r", &io, tiffReadProc, tiffWriteProc, tiffSeekProc, tiffCloseProc, tiffSizeProc,
			tiffMapProc, tiffUnmapProc);
		if (!tif)
			return false;
		m_Info.Bpp = 32;
		std::unique_ptr<TIFF, CloseTiff> ptrTiff(tif);
		//get image info
		m_Info.Width = 0;
		m_Info.Height= 0;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &m_Info.Width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &m_Info.Height);
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
		if (m_Info.Width <= 0 || m_Info.Height <= 0)
			return false;
		int metric = PHOTOMETRIC_RGB;

		m_Buffer.Allocate(m_Info.Width * m_Info.Height * 4);
		//unsigned int* color = m_Buffer.PtrT<unsigned int>();
		unsigned int* head = m_Buffer.PtrT<unsigned int>();

		int n = TIFFReadRGBAImageOriented(tif, m_Info.Width , m_Info.Height, head,	ORIENTATION_TOPLEFT, 1);
		if (n == 0)
			return false;
	}
	else if (eType == eWEBP)
	{
		WebPDecoderConfig config;
		WebPDecBuffer* output_buffer = &config.output;
		WebPBitstreamFeatures* bitstream = &config.input;
		if (!WebPInitDecoderConfig(&config)) {
			printf ( "WEBP Library version mismatch!\n");
			return false;
		}
		output_buffer->colorspace = MODE_RGBA;
		config.options.no_fancy_upsampling = 1;
		config.options.bypass_filtering = 1;
		config.options.use_threads = 1;
		config.output.colorspace = MODE_RGBA;
		VP8StatusCode status = VP8_STATUS_OK;
		size_t data_size = 0;
		status = WebPDecode(blob, nLen, &config);
		if (VP8_STATUS_OK != status)
			return false;
		
			m_Buffer.Allocate(0);
			m_Buffer.Append(output_buffer->u.RGBA.rgba, output_buffer->u.RGBA.size);
			WebPFreeDecBuffer(output_buffer);
			m_Info.Width = output_buffer->width;
			m_Info.Height = output_buffer->height;
			if (m_Info.Width != 0 && m_Info.Height != 0)
				m_Info.Bpp = output_buffer->u.RGBA.size / m_Info.Width / m_Info.Height * 8;
			else
				m_Info.Bpp = 0;
	}
	return true;
}
AImage::~AImage()
{
}

/// \brief ����
bool AImage::CopyFrom(AImage* pImage)
{
	return false;
}

/// \brief ��ȡͼƬ��ɫ��ʽ

AColorBandType AImage::RGBAType()
{
	//Ŀǰͳһ�洢��RGBA�����Թ̶�����RGBA���Ժ�����Ҫ����չ
	//20200826����ǰ�Ѿ�����һ����ʷ�ı���ڣ���Ҫ��չ
	return m_Info.RGBAType;
}

unsigned int AImage::Width()
{
	return 0;
}

unsigned int AImage::Height()
{
	return 0;
}

const unsigned char* AImage::Bit()
{
	return nullptr;
}


int AImage::RGBAIndex(AColorBandType t, char c)
{
	static string str = "rgbaRGBA";
	if (str.find(c) == string::npos)
	{
		return -1;
	}

	static const char r = 'r', g = 'g', b = 'b', a = 'a';
	static const char R = 'R', G = 'G', B = 'B', A = 'A';
	static std::map<const char, int> m = { {r, -1}, {g, -1}, {b, -1}, {a, -1} };
	switch (t)
	{
	case AColorBandType::eBGRA32:
		m[b] = m[B] = 0, m[g] = m[G] = 1, m[r] = m[R] = 2, m[a] = m[A] = 3;
		break;
	case AColorBandType::eABGR32:
		m[a] = m[A] = 0, m[b] = m[B] = 1, m[g] = m[G] = 2, m[r] = m[R] = 3;
		break;
	case AColorBandType::eRGBA32:
		m[r] = m[R] = 0, m[g] = m[G] = 1, m[b] = m[B] = 2, m[a] = m[A] = 3;
		break;
	case AColorBandType::eARGB32:
		m[a] = m[A] = 0, m[r] = m[R] = 1, m[g] = m[G] = 2, m[b] = m[B] = 3;
		break;
	case AColorBandType::eRGB24:
		m[r] = m[R] = 0, m[g] = m[G] = 1, m[b] = m[B] = 2, m[a] = m[A] = -1;
		break;
	case AColorBandType::eBGR24:
		m[b] = m[B] = 0, m[g] = m[G] = 1, m[r] = m[R] = 2, m[a] = m[A] = -1;
		break;
	default:
		break;
	}
	auto iter = m.find(c);
	if (iter != m.end())
		return iter->second;
	return -1;
}

bool CopyImage(unsigned char* pSrc, AColorBandType srcType, unsigned char* pDsc, AColorBandType dscType, int width, int height)
{
	if (dscType == srcType)
	{
		memcpy(pDsc, pSrc, AImage::Stride(width, dscType) * height);
		return true;
	}

	int srcStride = AImage::Stride(width, srcType);
	int dscStride = AImage::Stride(width, dscType);
	int srcBPP = AImage::BytePerPixcel(srcType);
	int dscBPP = AImage::BytePerPixcel(dscType);

	int srcRIndex = AImage::RGBAIndex(srcType, 'r');
	int srcGIndex = AImage::RGBAIndex(srcType, 'g');
	int srcBIndex = AImage::RGBAIndex(srcType, 'b');
	int srcAIndex = AImage::RGBAIndex(srcType, 'a');
	int dscRIndex = AImage::RGBAIndex(dscType, 'r');
	int dscGIndex = AImage::RGBAIndex(dscType, 'g');
	int dscBIndex = AImage::RGBAIndex(dscType, 'b');
	int dscAIndex = AImage::RGBAIndex(dscType, 'a');

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			pDsc[w * dscBPP + dscRIndex] = pSrc[w * srcBPP + srcRIndex];
			pDsc[w * dscBPP + dscGIndex] = pSrc[w * srcBPP + srcGIndex];
			pDsc[w * dscBPP + dscBIndex] = pSrc[w * srcBPP + srcBIndex];
			if (dscAIndex >= 0)
			{
				if (srcAIndex >= 0)
					pDsc[w * dscBPP + dscAIndex] = pSrc[w * srcBPP + srcAIndex];
				else
					pDsc[w * dscBPP + dscAIndex] = 255;
			}
		}
		pSrc += srcStride;
		pDsc += dscStride;
	}
	return true;
}

/// \brief �����ⲿ���ݵ�����
bool AImage::FillImageData(const unsigned char* buff, int nLen, AColorBandType eType)
{
	return CopyImage(const_cast<unsigned char*>(buff), eType, const_cast<unsigned char*>(Bit()), m_Info.RGBAType, Width(), Height());
}




/// \brief �������������ݵ��ⲿӰ��
int AImage::CopyImageData(unsigned char* pBuff, int nLen, AColorBandType eType)
{
	if (CopyImage(const_cast<unsigned char*>(Bit()), m_Info.RGBAType, pBuff, eType, Width(), Height()))
		return nLen;
	return 0;
}


/// \brief ͼ��һ�е��ֽڳ���
unsigned int AImage::Stride()
{
	return Width() * BytePerPixcel(m_Info.RGBAType);
}

unsigned int AImage::Stride(unsigned int w, AColorBandType t)
{
	return w * BytePerPixcel(t);
}

unsigned int AImage::BytePerPixcel(AColorBandType t)
{
	int size = 4;
	switch (t)
	{
	case AColorBandType::eBGRA32:
	case AColorBandType::eABGR32:
	case AColorBandType::eRGBA32:
	case AColorBandType::eARGB32:
		break;
	case AColorBandType::eRGB24:
	case AColorBandType::eBGR24:
		size = 3;
		break;
	default:
		break;
	}
	return size;
}

bool AImage::ParseImageHeaderInfo(const char* file, AImageHeaderInfo* info)
{
	if (!info || !file)
		return false;
	// to do
	return false;
}

bool AImage::ParseImageHeaderInfo(const unsigned char* blob, unsigned int len, AImageHeaderInfo* info)
{
	if (!blob || !info)
		return false;

	// to do
	return false;
}

/// \brief ��ȡͼ����ֽ����飬��������ͼ���ܹ��ɹ�
const unsigned char* AImage::Row(int r)
{
	if (r < 0 || r >= Height())
		return 0;
	const unsigned char* pHead = Bit();
	if (NULL == pHead)
		return 0;
	return pHead + Stride() * r;
}

bool AImage::Save(const char* strFile, AImageEncodeType type)
{
	return false;
}

bool AImage::Save(const unsigned char* pBlob, int nLen, AImageEncodeType type)
{
	return false;
}

AImage* AImage::LoadFrom(const char* strFile)
{
	AGrowByteBuffer buff;
	FILE* pF = fopen(strFile,"rb");
	if (!pF)
		return  0;
#ifdef _WIN32
	struct _stat64 s;
	_stat64(strFile, &s);
#else
	struct stat s;
	stat(strFile, &s);
#endif
	buff.Allocate(s.st_size);
	size_t n = fread((void*)buff.BufferHead(), 1, s.st_size, pF);
	fclose(pF);
	auto img =  new AImage(buff.BufferHead(), s.st_size);
	if(img && img->m_LoadOK)
		return img;
	return nullptr;
}

AImage* AImage::LoadFrom(const unsigned char* pBlob, int nLen)
{
	return new AImage(pBlob, nLen);
}



AImageEncodeType AImage::EncodeType(const unsigned char* buff, int nLen)
{
	if (!buff || nLen <= 0)
		return eUnknownImage;

	//������ݵ�ͷ��PAM��ͷ��
	if (nLen > 3 && strncmp((const char*)buff, "P7\n", 3) == 0)
		return ePAM;

	ReadImageBuffer io(buff, nLen);
	return (AImageEncodeType)io.ImageType();
}
AImageEncodeType AImage::EncodeType(const char* file)
{
	ImageFile io(file, true);
	return (AImageEncodeType)io.ImageType();
}


/// \brief ����Ӱ��ı������ͻ�ȡ��ӦӰ���ļ���չ��
string FileExtension(AImageEncodeType eType)
{
	switch (eType)
	{
	case ePNG:
		return "png";
	case eJPG:
		return "jpeg";
	case eBMP:
		return "bmp";
	case eDNG:
		return "dng";
	case eGIF:
		return "gif";
	case eTGA:
		return "tga";
	case eKTX2:
		return "ktx2";
	case eKTX:
		return "ktx";
	case eDDS:
		return "dds";
	case eCRN:
		return "crn";
	}
	return "";
}
/// \brief ����Ӱ��ı������ͻ�ȡ��Ӧ��mimetype����
string MIMEType(AImageEncodeType eType)
{
	switch (eType)
	{
	case ePNG:
		return "image/png";
	case eJPG:
		return "image/jpeg";
	case eBMP:
		return "image/bmp";
	case eDNG:
		return "image/dng";
	case eGIF:
		return "image/gif";
	case eTGA:
		return "image/tga";
	case eKTX:
		return "image/ktx";
	case eDDS:
		return "image/dds";
	case eCRN:
		return "image/crn";
	case eTIFF:
		return "image/tiff";
	}
	return "";
}

