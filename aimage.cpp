#include "pch.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
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
using namespace std;
#ifndef PI
#define PI 3.1415926535897932
#endif
/// \brief 获取圆周率PI
double AMath::Pi()
{
	return PI;
}

/// \brief 获取自然对数
double AMath::Log(double val)
{
	return log(val);
}

/// \brief 度转换为弧度
double AMath::ToRadian(double degree)
{
	static double d2r = PI / 180.0;
	return degree * d2r;
}
/// \brief 弧度转换为度
double AMath::ToDegree(double radian)
{
	static double r2d = 180.0 / PI;
	return radian * r2d;
}
/// \brief 无效float
/// \details Not-A-Number不是一个数字的缩写，可以表示无效的float值，0/0.除数为0的结果
float AMath::NaNF()
{
	static float d = nanf("");
	return d;
}
/// \brief 无效double
double AMath::NaN()
{
	static double d = nan("");
	return d;
}
/// \brief 判断一个double值是否是无效的Double值
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

/// \brief 将整数对齐到2的N次方
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

/// \brief 对齐到2的N次方
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
/// \brief 对齐到2的N次方
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

/// \brief 判断整数是否对齐到2的N次方
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
/// \brief 缓冲区的尾指针
/// \details >=此指针表示超出了缓冲区范围
unsigned char* AByteBuffer::EndPtr() const
{
	return Ptr() + BufferSize();
}

/// \brief 将二进制内存块转换为base64编码字符串
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

/// \brief 判断字符串是否是一个base64编码的字符串
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
/// \brief 向尾部增加一段字符串
/// \param str 添加字符串
/// \return 
unsigned char* AByteBuffer::Append(const std::string& str) {
	return Append((const unsigned char*)str.data(), str.size());
}


void B64CallFun(void* pOpaque, const unsigned char b)
{
	AByteBuffer* pBuffer = (AByteBuffer*)pOpaque;
	pBuffer->AppendT(b);
}
/// \brief 从Base64字符串解码成为内存块
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
/// \brief 移动构造
AGrowByteBuffer::AGrowByteBuffer(AGrowByteBuffer&& rhs)
{
	m_pBuffer = NULL;
	Swap(rhs);
}
/// \brief 拷贝构造
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
/// \brief 赋值
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
/// \brief 赋值
AGrowByteBuffer& AGrowByteBuffer::operator=(const AGrowByteBuffer& rsh)
{
	// 同一个指针不触发操作
	if ((void*)this == (void*)&rsh)
		return *this;
	Allocate(0);
	Append(rsh.BufferHead(), rsh.BufferSize());
	return *this;
}
/// \brief 交换两个缓冲区的数据
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

/// \brief 点Offset点
APT APT::operator +(const APT& pt) const
{
	return APT(X + pt.X, Y + pt.Y);
}

/// \brief offset自身
APT& APT::operator +=(const APT& pt)
{
	X += pt.X;
	Y += pt.Y;
	return *this;
}


/// \brief 点Offset点
APT APT::operator -(const APT& pt) const
{
	return APT(X - pt.X, Y - pt.Y);
}

/// \brief offset自身
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

/// \brief 点Offset点
APTF APTF::operator +(const APTF& pt) const
{
	return APTF(X + pt.X, Y + pt.Y);
}

/// \brief offset自身
APTF& APTF::operator +=(const APTF& pt)
{
	X += pt.X;
	Y += pt.Y;
	return *this;
}


/// \brief 点Offset点
APTF APTF::operator -(const APTF& pt) const
{
	return APTF(X - pt.X, Y - pt.Y);
}

/// \brief offset自身
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
/// \brief 从点构造
ASize::ASize(const APT& p)
{
	Width = p.X;
	Height = p.Y;
}

/// \brief 是否相等
bool ASize::operator == (const ASize& s)const
{
	return s.Width == Width && s.Height == Height;
}
/// \brief 复制
ASize& ASize::operator = (const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 是否不等
bool ASize::operator != (const ASize& s)const
{
	return s.Width != Width || s.Height != Height;
}
/// \brief 宽高构成的矩形面积
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

/// \brief 是否相等
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

/// \brief 是否不等
bool ASizeF::operator != (const ASizeF& s)const
{
	return !AMath::IsEqual(s.Width, Width) ||
		!AMath::IsEqual(s.Height, Height);
}



/// \brief 缺省构造函数
ASizeD::ASizeD() :Width(0), Height(0)
{

}
/// \brief 重载构造函数
ASizeD::ASizeD(const ASize& s) : Width(s.Width), Height(s.Height)
{
}
/// \brief 重载构造函数
ASizeD::ASizeD(const ASizeF& s) : Width(s.Width), Height(s.Height)
{

}
/// \brief 重载构造函数
ASizeD::ASizeD(const ASizeD& s) : Width(s.Width), Height(s.Height)
{

}

/// \brief 重载构造函数
ASizeD::ASizeD(double w, double h) :Width(w), Height(h)
{

}

/// \brief 是否相等
bool ASizeD::operator == (const ASizeD& s)const
{
	return AMath::IsEqual(s.Width, Width) && AMath::IsEqual(s.Height, Height);
}
/// \brief 是否不等
bool ASizeD::operator != (const ASizeD& s)const
{
	return !AMath::IsEqual(s.Width, Width) || !AMath::IsEqual(s.Height, Height);
};
/// \brief 赋值
ASizeD& ASizeD::operator = (const ASizeD& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 赋值
ASizeD& ASizeD::operator = (const ASize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 赋值
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
/// \brief 中心点
APT ARect::Center()const
{
	return APT((Left + Right) / 2, (Top + Bottom) / 2);
}
/// \brief 是否相交
bool ARect::IsIntersects(const ARect& rhs)const
{
	return !IsDisjoin(rhs);
}
/// \brief 计算两个相交的部分
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


//描述：返回me是否和env相离
template<class T>
bool IsDisjoinT(T xmin, T ymin, T xmax, T ymax, T sx, T sy, T ex, T ey)
{
	//以下是矩形选中线段中部的情况
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

/// \brief 是否和线相交
bool ARect::IsDisjoin(int x1, int y1, int x2, int y2)const
{
	//外接矩形相离则相离。
	ARect box(AMath::Min(x1, x2), AMath::Min(y1, y2), AMath::Max(x1, x2), AMath::Max(y1, y2));
	if (IsDisjoin(box))
		return true;
	return IsDisjoinT(Left, Top, Right, Bottom, x1, y1, x2, y2);
}
/// \brief 是否相离
bool ARect::IsDisjoin(const ARect& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top || Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief 是否包含
bool ARect::IsContain(int x, int y)const
{
	return x >= Left && x < Right&& y >= Top && y < Bottom;
}

/// \brief 是否包含
bool ARect::IsContain(const APT& pt)const
{
	return IsContain(pt.X, pt.Y);
}

/// \brief 是否包含
bool ARect::IsContain(const ARect& rhs)const
{
	return IsContain(rhs.Left, rhs.Top) && IsContain(rhs.Right - 1, rhs.Bottom - 1);
}
/// \brief 定位点
APT ARect::Location()const
{
	return LeftTop();
}

/// \brief 大小
ASize ARect::Size()const
{
	return ASize(Width(), Height());
}

/// \brief 左上角
APT ARect::LeftTop()const
{
	return APT(Left, Top);
}


/// \brief 右上角
APT ARect::RightTop()const
{
	return APT(Right, Top);
}


/// \brief 左下角
APT ARect::LeftBottom()const
{
	return APT(Left, Bottom);
}


/// \brief 右下角
APT ARect::RightBottom()const
{
	return APT(Right, Bottom);
}
/// \brief 矩形偏移
ARect ARect::operator + (const APT& pt)const
{
	return ARect(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief 矩形偏移
ARect ARect::operator - (const APT& pt)const
{
	return ARect(Left - pt.X, Top - pt.Y, Width(), Height());
}

/// \brief 矩形偏移返回新的偏移后的矩形
ARect ARect::OffsetTo(const APT& pt)const
{
	return ARect(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief 矩形合并
ARect ARect::operator + (const ARect& rect)const
{
	ARect ret(*this);
	ret.Union(rect);
	return ret;
}
/// \brief 合并到自身
ARect& ARect::operator += (const ARect& rect)
{
	return Union(rect);
}

/// \brief 矩形合并
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
/// \brief 中心点
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

/// \brief 是否相交
bool ARectF::IsIntersects(const ARectF& rhs)const
{
	return !IsDisjoin(rhs);
}

/// \brief 计算两个相交的部分
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


/// \brief 是否和线相交
bool ARectF::IsDisjoin(float x1, float y1, float x2, float y2)const
{
	//外接矩形相离则相离。
	ARectF box((float)AMath::Min(x1, x2), AMath::Min(y1, y2), AMath::Max(x1, x2), AMath::Max(y1, y2));
	if (IsDisjoin(box))
		return true;
	return IsDisjoinT(Left, Top, Right, Bottom, x1, y1, x2, y2);
}
/// \brief 是否相离
bool ARectF::IsDisjoin(const ARectF& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top || Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief 是否包含
bool ARectF::IsContain(float x, float y)const
{

	return x >= Left && x < Right&& y >= Top && y < Bottom;
}

/// \brief 是否包含
bool ARectF::IsContain(const APTF& pt)const
{
	return IsContain(pt.X, pt.Y);
}

/// \brief 是否包含
bool ARectF::IsContain(const ARectF& rhs)const
{

	return IsContain(rhs.Left, rhs.Top) && IsContain(rhs.Right, rhs.Bottom);
}

/// \brief 定位点
APTF ARectF::Location()const
{
	return LeftTop();
}

/// \brief 大小
ASizeF ARectF::Size()const
{
	return ASizeF(Width(), Height());
}

/// \brief 左上角
APTF ARectF::LeftTop()const
{
	return APTF(Left, Top);
}


/// \brief 右上角
APTF ARectF::RightTop()const
{
	return APTF(Right, Top);
}


/// \brief 左下角
APTF ARectF::LeftBottom()const
{
	return APTF(Left, Bottom);
}


/// \brief 右下角
APTF ARectF::RightBottom()const
{
	return APTF(Right, Bottom);
}
/// \brief 矩形偏移
ARectF ARectF::operator + (const APTF& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}

/// \brief 矩形偏移
ARectF ARectF::operator - (const APTF& pt)const
{
	return ARectF(Left - pt.X, Top - pt.Y, Width(), Height());
}


/// \brief 矩形偏移返回新的偏移后的矩形
ARectF ARectF::OffsetTo(const APTF& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}



/// \brief 矩形偏移
ARectF ARectF::operator + (const APT& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}


/// \brief 矩形偏移
ARectF ARectF::operator - (const APT& pt)const
{
	return ARectF(Left - pt.X, Top - pt.Y, Width(), Height());
}

/// \brief 矩形偏移返回新的偏移后的矩形
ARectF ARectF::OffsetTo(const APT& pt)const
{
	return ARectF(Left + pt.X, Top + pt.Y, Width(), Height());
}
/// \brief 矩形合并
ARectF ARectF::operator + (const ARectF& rect)const
{
	ARectF ret(*this);
	ret.Union(rect);
	return ret;
}
/// \brief 合并到自身
ARectF& ARectF::operator += (const ARectF& rect)
{
	return Union(rect);
}

/// \brief 矩形合并
ARectF& ARectF::Union(const ARectF& rect)
{
	Left = AMath::Min(Left, rect.Left);
	Top = AMath::Min(Top, rect.Top);
	Right = AMath::Max(Right, rect.Right);
	Bottom = AMath::Max(Bottom, rect.Bottom);
	return *this;
}




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
AImage::AImage(const unsigned char* blob, int nLen)
{
}
AImage::~AImage()
{
}

/// \brief 拷贝
bool AImage::CopyFrom(AImage* pImage)
{
	return false;
}

/// \brief 获取图片颜色格式

AColorBandType AImage::RGBAType()
{
	//目前统一存储的RGBA，所以固定返回RGBA，以后有需要再扩展
	//20200826，当前已经处于一个历史的变革期，需要扩展
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

/// \brief 拷贝外部数据到自身
bool AImage::FillImageData(const unsigned char* buff, int nLen, AColorBandType eType)
{
	return CopyImage(const_cast<unsigned char*>(buff), eType, const_cast<unsigned char*>(Bit()), m_Info.RGBAType, Width(), Height());
}




/// \brief 拷贝自身的数据到外部影像
int AImage::CopyImageData(unsigned char* pBuff, int nLen, AColorBandType eType)
{
	if (CopyImage(const_cast<unsigned char*>(Bit()), m_Info.RGBAType, pBuff, eType, Width(), Height()))
		return nLen;
	return 0;
}


/// \brief 图像一行的字节长度
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

/// \brief 获取图像的字节数组，并非所有图像都能够成功
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
	return nullptr;
}

AImage* AImage::LoadFrom(const unsigned char* pBlob, int nLen)
{
	return nullptr;
}

#pragma region imageIO 
//影像IO
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
//读写文件的io实现
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
//读写内存中的数据的io实现
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
//内存中的只读io实现
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

AImageEncodeType AImage::EncodeType(const unsigned char* buff, int nLen)
{
	if (!buff || nLen <= 0)
		return eUnknownImage;

	//如果内容的头是PAM的头。
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


/// \brief 根据影像的编码类型获取对应影像文件扩展名
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
/// \brief 根据影像的编码类型获取对应的mimetype定义
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


