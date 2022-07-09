#pragma once

#define UTILITY_NS namespace GG{namespace Utility{
#define UTILITY_ENDNS }}
#define UTILITY_NAME GG::Utility



#ifndef GG_API
#if defined(GG_DLL)
#if defined(_WIN32)
#if GG_IMPLEMENTATION
#define GG_API __declspec(dllexport)
#else
#define GG_API __declspec(dllimport)
#endif
#else
#define GG_API __attribute__((visibility("default")))
#endif
#else
#define GG_API
#endif
#endif


#ifdef __GNUC__
#define GS_DEPRECATED(id) id __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define GS_DEPRECATED(id) __declspec(deprecated) id
#else
#define GS_DEPRECATED(id) id
#endif



#ifdef _MSC_VER
#pragma warning(disable : 4018)

#pragma warning(disable : 4100)
#pragma warning(disable : 4101)
#pragma warning(disable : 4189)
#pragma warning(disable : 4192)

#pragma warning(disable : 4238)
#pragma warning(disable : 4239)
#pragma warning(disable : 4244)
#pragma warning(disable : 4245)
#pragma warning(disable : 4251)
#pragma warning(disable : 4267)


#pragma warning(disable : 4305)
#pragma warning(disable : 4311)
#pragma warning(disable : 4389)

#pragma warning(disable : 4482)
#pragma warning(disable : 4635) 
#pragma warning(disable : 4701)

#pragma warning(disable : 4800)
#pragma warning(disable : 4996)

#endif

#include <stdio.h>
#include <memory>


#ifdef _WIN32

#ifdef _WIN64
#define INT3264_MAX _I64_MAX
#else //_WIN64
#define INT3264_MAX INT_MAX
#endif //_WIN64

#else //WIN32
#include "limits.h"
#ifdef _LP64
#define INT3264_MAX LONG_LONG_MAX
#else //_LP64
#define INT3264_MAX INT_MAX
#endif //_LP64

#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

/// \brief 数学相关的函数
class GG_API AMath
{
public:
	template<class T>
	static T Random(T min, T max)
	{
		int n = rand();
		return min + n * 1.0 * (max - min) / INT_MAX;
	}
	template<class U, class V>
	static U Max(U a, V b)
	{
		return a > b ? a : b;
	}
	template<class U, class V>
	static U Min(U a, V b)
	{
		return a < b ? a : b;
	}
	/// \brief 将整数对齐到2的N次方
	static unsigned int RoundUpPowerOf2(unsigned int n);

	/// \brief 对齐到2的N次方
	static unsigned int NextPowerOfTwo(unsigned int v);
	/// \brief 对齐到2的N次方
	static unsigned long long NextPowerOfTwo(unsigned long long v);

	/// \brief 判断整数是否对齐到2的N次方
	static bool IsPowerOfTwo(unsigned long long n);

	/// \brief 判断两个double是否相等
	static bool IsEqual(double d1, double d2);

	/// \brief 判断两个float是否相等
	static bool IsEqual(float f1, float f2);

	/// \brief 判断两个double是否相等
	static bool IsEqual(double d1, double d2, double tol);

	/// \brief 判断两个float是否相等
	static bool IsEqual(float f1, float f2, float tol);


	/// \brief 四舍五入到最邻近的整数
	/// \param place保留小数位数
	static double Round(double r, unsigned int place = 0);

	/// \brief 四舍五入到最邻近的整数
	/// \param place保留小数位数
	static float Round(float r, unsigned int place = 0);

	/// \brief 度转换为弧度
	static double ToRadian(double degree);
	/// \brief 弧度转换为度
	static double ToDegree(double radian);

	/// \brief 无效double
	/// \details Not-A-Number不是一个数字的缩写，可以表示无效的double值，0/0.除数为0的结果
	static double NaN();
	/// \brief 无效float
	/// \details Not-A-Number不是一个数字的缩写，可以表示无效的float值，0/0.除数为0的结果
	static float NaNF();

	/// \brief 判断一个double值是否是无效的Double值
	static bool IsNaN(double val);

	/// \brief 获取圆周率PI
	static double Pi();



	/// \brief 获取自然对数
	static double Log(double val);


	/// \brief 在最大最小值之间夹取
	template<typename T>
	static T ClampTo(T v, T minimum, T maximum) { return v<minimum ? minimum : v>maximum ? maximum : v; }

	/// \brief 夹取最小值
	template<typename T>
	static T ClampAbove(T v, T minimum) { return v < minimum ? minimum : v; }

	/// \brief 夹取最大值
	template<typename T>
	static T ClampBelow(T v, T maximum) { return v > maximum ? maximum : v; }

	/// \brief 等同ClampTo
	template<typename T>
	static T ClampBetween(T v, T minimum, T maximum)
	{
		return ClampBelow(ClampAbove(v, minimum), maximum);
	}

	/// \brief 获取符号
	template<typename T>
	static T Sign(T v) { return v < (T)0 ? (T)-1 : (T)1; }

	/// \brief 获取符号
	template<typename T>
	static T SignOrZero(T v) { return v < (T)0 ? (T)-1 : (v > (T)0 ? (T)1 : 0); }

	/// \brief 平方
	template<typename T>
	T Square(T v) { return v * v; }

	/// \brief 带符号平方
	template<typename T>
	static T SignedSquare(T v) { return v < (T)0 ? -v * v : v * v; }
};

/// \brief 直接内存块
class GG_API AByteBuffer
{
protected:
	/// \brief 受保护的构造函数，只有子类才能创建对象
	AByteBuffer();
public:
	virtual ~AByteBuffer();

	/// \brief 类型转换重载函数，转换为无符号字符指针类型
	virtual operator unsigned char* ()const;
	/// \brief 类型转换重载函数，转换为字符指针类型
	virtual operator char* ()const;
	/// \brief 类型转换重载函数，转换为整数指针类型
	virtual operator int* ()const;
	/// \brief 类型转换重载函数，转换为无符号整数指针类型
	virtual operator unsigned int* ()const;
	/// \brief 类型转换重载函数，转换为16位整数指针类型
	virtual operator short* ()const;
	/// \brief 类型转换重载函数，转换为无符号16位整数指针类型
	virtual operator unsigned short* ()const;
	/// \brief 类型转换重载函数，转换为64位整数指针类型
	virtual operator long long* ()const;
	/// \brief 类型转换重载函数，转换为无符号64位整数指针类型
	virtual operator unsigned long long* ()const;
	/// \brief 类型转换重载函数，转换为64位浮点指针类型
	virtual operator double* ()const;
	/// \brief 类型转换重载函数，转换为32位浮点指针类型
	virtual operator float* ()const;
	/// \brief 类型转换重载函数，转换为BOOL指针类型
	virtual operator bool* ()const;

	/// \brief 缓冲区的头指针，相当于BufferHead()
	/// \return 
	virtual unsigned char* Ptr() const = 0;

	/// \brief 缓冲区的尾指针
	/// \details >=此指针表示超出了缓冲区范围
	/// \return 
	virtual unsigned char* EndPtr() const;

	/// \brief 函数模板
	template<typename T>
	T* PtrT() const
	{
		return (T*)(Ptr());
	}

	/// \brief 函数模板
	template<typename T>
	T* EndPtrT() const
	{
		return (T*)(EndPtr());
	}

	/// \brief 缓冲区的头指针
	/// \return 
	virtual unsigned char* BufferHead()const = 0;
	/// \brief 缓冲区的长度
	/// \return 
	virtual unsigned int BufferSize() const = 0;

	/// \brief 依照结构类型计算的元素数量
	template<typename T>
	/// \brief 
	/// \return 
	unsigned int BufferSizeT() const
	{
		return BufferSize() / sizeof(T);
	}

	/// \brief 设置缓冲区的字节值为特定值
	/// \details 类似memset的功能
	/// \param nValue 
	/// \return 
	virtual unsigned char* SetBufferValue(int nValue) = 0;

	/// \brief 依照结构类型计算的元素数量
	template<typename T>
	T* SetBufferValueT(const T& val)
	{
		//获取数据的字节
		const unsigned char* pByte = (const unsigned char*)&val;
		//判断整个数据为重复字节，或者全部为0
		bool bIsEqual = true;
		for (int i = 1;i < sizeof(val);i++)
		{
			if (pByte[i] != pByte[0])
			{
				bIsEqual = false;
				break;
			}
		}
		//如果整个为重复字节。
		if (bIsEqual)
			return (T*)SetBufferValue(pByte[0]);
		//如果是非重复字节这需要租个赋值
		T* pHead = PtrT<T>();
		T* pEnd = EndPtrT<T>();
		for (;pHead < pEnd;pHead++)
		{
			pHead[0] = val;
		}
		return PtrT<T>();
	}

	/// \brief 向尾部添加一段数据
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Append(const unsigned char* pBuff, int nLen) = 0;
	/// \brief 向尾部增加一段字符串
	/// \param pStr 
	/// \return 
	virtual unsigned char* Append(const char* pStr) = 0;
	/// \brief 向尾部增加一段特定结构的数组
	template<typename T>
	/// \brief 
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	unsigned char* AppendT(const T* pBuff, int nLen)
	{
		return Append((const unsigned char*)pBuff, nLen * sizeof(T));
	}
	/// \brief 向尾部增加一个特定结构数据
	template<typename T>
	/// \brief 
	/// \param pBuff 
	/// \return 
	unsigned char* AppendT(const T& pBuff)
	{
		return Append((const unsigned char*)&pBuff, sizeof(T));
	}
	/// \brief 从特定位置覆盖数据
	/// \param nPos 
	/// \param pStr 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Insert(unsigned int nPos, const unsigned char* pStr, int nLen) = 0;
	/// \brief 分配特定字节长度的缓冲区。
	/// \param nLen 
	/// \return 
	virtual unsigned char* Allocate(unsigned int nLen) = 0;
	/// \brief 函数模板，分配内存
	template<typename T>
	T* AllocateT(int nLen)
	{
		Allocate(sizeof(T) * nLen);
		return PtrT<T>();
	}
	/// \brief 清除内存数据
	virtual void Clear() = 0;
	/// \brief 释放内存
	virtual void Reset() = 0;
	/// \brief 复制输入内存块中特定长度的数据
	virtual unsigned char* Copy(const unsigned char* pBuff, int nLen) = 0;

	/// \brief 将二进制内存块转换为base64编码字符串
	/// \return 
	std::string ToBase64()const;

	/// \brief 从Base64字符串解码成为内存块
	/// \param strBase64 
	/// \return 
	bool FromBase64(const char* strBase64);

	/// \brief 判断字符串是否是一个base64编码的字符串
	/// \param strBase64 
	/// \return 
	static bool IsBase64(const char* strBase64);

	/// \brief 向尾部增加一段字符串
	/// \param str 添加字符串
	/// \return 
	virtual unsigned char* Append(const std::string& str);


};

/// \brief 增长型直接内存块，内存只增加不减少
class GG_API AGrowByteBuffer :public AByteBuffer
{
	/// \brief 联合体
	union {
		unsigned char* m_pBuffer;
		/// \brief 结构体，m_pMeta为结构体类型的指针
		struct
		{
			unsigned int Real;
			unsigned int Size;
		}*m_pMeta;
	};
public:
	/// \brief 
	/// \param nLen 
	AGrowByteBuffer(int nLen = 0);
	/// \brief 构造函数，传递指针地址给类成员变量
	/// \param pBuffer 
	AGrowByteBuffer(unsigned char* pBuffer);
	/// \brief 构造函数，分配nLen个字符内存
	/// \param pBuffer 
	/// \param nLen 
	AGrowByteBuffer(const unsigned char* pBuffer, int nLen);
	/// \brief 拷贝构造
	/// \param rhs 
	AGrowByteBuffer(const AGrowByteBuffer& rhs);

	/// \brief 移动构造
	AGrowByteBuffer(AGrowByteBuffer&& rhs);

	virtual ~AGrowByteBuffer();
	/// \brief 释放内存，传递指针地址
	/// \param pBuffer 
	void Attach(unsigned char* pBuffer);
	/// \brief 传递指针地址
	/// \return 
	unsigned char* Detach();
	/// \brief 类型转换
	virtual operator unsigned char* () const;
	/// \brief 分配8个字符内存
	/// \return 
	virtual unsigned char* Ptr() const;
	/// \brief 分配8个字符内存
	/// \return 
	virtual unsigned char* BufferHead()const;
	/// \brief 分配nLen个字符内存
	/// \return 
	virtual unsigned int BufferSize() const;
	/// \brief 
	/// \return 
	virtual unsigned int RealSize() const;
	/// \brief 复制int类型数据的前real个字节
	/// \param nValue 
	/// \return 
	virtual unsigned char* SetBufferValue(int nValue);
	/// \brief 增加内存
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Append(const unsigned char* pBuff, int nLen);
	/// \brief 函数重载，增加内存
	/// \param pStr 
	/// \return 
	virtual unsigned char* Append(const char* pStr);
	/// \brief 
	/// \param nPos 
	/// \param pStr 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Insert(unsigned int nPos, const unsigned char* pStr, int nLen);
	/// \brief 分配nLen个字节内存
	/// \param nLen 
	/// \return 
	virtual unsigned char* Allocate(unsigned int nLen);
	/// \brief 清除内存数据
	virtual void Clear();
	/// \brief 释放内存
	virtual void Reset();
	/// \brief 复制nLen个字节
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Copy(const unsigned char* pBuff, int nLen);

	/// \brief 交换两个缓冲区的数据
	/// \param rhs 
	/// \return 
	AGrowByteBuffer* Swap(AGrowByteBuffer& rhs);

	/// \brief 交换两个缓冲区的数据
	/// \param rhs 
	/// \return 
	AGrowByteBuffer* Swap(unsigned char* pBuff);
	/// \brief 赋值
	AGrowByteBuffer& operator=(const AGrowByteBuffer& rsh);
	/// \brief 赋值
	AGrowByteBuffer& operator=(AGrowByteBuffer&& rsh);


};

/// \brief 创建一个int类型的点结构体
struct GG_API APT
{
	int X, Y;
	APT();
	/// \brief 
	/// \param rhs 
	APT(const APT& rhs);
	/// \brief 
	/// \param x 
	/// \param y 
	APT(int x, int y);

	/// \brief 点Offset点
	APT operator +(const APT& pt) const;

	/// \brief offset自身
	APT& operator +=(const APT& pt);

	/// \brief 点Offset点
	APT operator -(const APT& pt) const;

	/// \brief offset自身
	APT& operator -=(const APT& pt);
};

/// \brief 创建一个float类型的点结构体
struct GG_API APTF
{
	float X, Y;

	/// \brief 缺省构造函数
	APTF();
	/// \brief 重载构造函数
	/// \param rhs 
	APTF(const APT& rhs);
	/// \brief 重载构造函数
	/// \param rhs 
	APTF(const APTF& rhs);
	/// \brief 重载构造函数
	/// \param x 
	/// \param y 
	APTF(int x, int y);
	/// \brief 重载构造函数
	/// \param x 
	/// \param y 
	APTF(float x, float y);

	/// \brief 点Offset点
	APTF operator +(const APTF& pt) const;

	/// \brief offset自身
	APTF& operator +=(const APTF& pt);

	/// \brief 点Offset点
	APTF operator -(const APTF& pt) const;

	/// \brief offset自身
	APTF& operator -=(const APTF& pt);
};

/// \brief 设置int型宽度和高度
struct GG_API ASize
{
	int Width, Height;
	/// \brief 缺省构造函数
	ASize();
	/// \brief 重载构造函数
	/// \param s 
	ASize(const ASize& s);
	/// \brief 从点构造
	/// \param p 
	ASize(const APT& p);
	/// \brief 重载构造函数
	/// \param w 
	/// \param h 
	ASize(int w, int h);

	/// \brief 是否相等
	bool operator == (const ASize& s)const;
	/// \brief 赋值
	ASize& operator = (const ASize& s);
	/// \brief 是否不等
	bool operator != (const ASize& s)const;

	/// \brief 宽高构成的矩形面积
	int Area()const;

};

/// \brief 设置float型宽度和高度 
struct GG_API ASizeF
{
	float Width, Height;
	/// \brief 缺省构造函数
	ASizeF();
	/// \brief 重载构造函数
	/// \param s 
	ASizeF(const ASize& s);
	/// \brief 重载构造函数
	/// \param s 
	ASizeF(const ASizeF& s);
	/// \brief 重载构造函数
	/// \param w 
	/// \param h 
	ASizeF(float w, float h);

	/// \brief 是否相等
	bool operator == (const ASizeF& s)const;
	/// \brief 赋值
	ASizeF& operator = (const ASizeF& s);
	/// \brief 赋值
	ASizeF& operator = (const ASize& s);

	/// \brief 是否不等
	bool operator != (const ASizeF& s)const;
};

/// \brief 设置Double型宽度和高度
struct GG_API ASizeD
{
	double Width, Height;
	/// \brief 缺省构造函数
	ASizeD();
	/// \brief 重载构造函数
	/// \param s 
	ASizeD(const ASize& s);
	/// \brief 重载构造函数
	/// \param s 
	ASizeD(const ASizeF& s);
	/// \brief 重载构造函数
	/// \param s 
	ASizeD(const ASizeD& s);

	/// \brief 重载构造函数
	/// \param w 
	/// \param h 
	ASizeD(double w, double h);

	/// \brief 是否相等
	bool operator == (const ASizeD& s)const;
	/// \brief 是否不等
	bool operator != (const ASizeD& s)const;
	/// \brief 赋值
	ASizeD& operator = (const ASizeD& s);
	/// \brief 赋值
	ASizeD& operator = (const ASize& s);
	/// \brief 赋值
	ASizeD& operator = (const ASizeF& s);

};

/// \brief 创建一个int类型的矩形
struct GG_API ARect
{
	int Left, Right, Top, Bottom;
	/// \brief 缺省构造函数
	ARect();
	/// \brief 重载构造函数
	/// \param pt 
	/// \param s 
	ARect(const APT& pt, const ASize& s);
	/// \brief 重载构造函数
	/// \param rhs 
	ARect(const ARect& rhs);
	/// \brief 重载构造函数
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARect(int x, int y, int w, int h);
	/// \brief 设置宽度
	/// \return 
	int Width()const;
	/// \brief 设置高度
	/// \return 
	int Height()const;
	/// \brief 偏移一个位置
	/// \param pt 
	/// \return 
	ARect& Offset(const APT& pt);

	/// \brief 中心点
	/// \return 
	APT Center()const;

	/// \brief 是否相交
	/// \param rhs 
	/// \return 
	bool IsIntersects(const ARect& rhs)const;

	/// \brief 计算两个相交的部分
	/// \param rhs 
	/// \return 
	ARect Intersects(const ARect& rhs) const;

	/// \brief 是否和线相离
	/// \param x1 
	/// \param y1 
	/// \param x2 
	/// \param y2 
	/// \return 
	bool IsDisjoin(int x1, int y1, int x2, int y2)const;

	/// \brief 是否相离
	/// \param rhs 
	/// \return 
	bool IsDisjoin(const ARect& rhs)const;

	/// \brief 是否包含
	/// \param x 
	/// \param y 
	/// \return 
	bool IsContain(int x, int y)const;

	/// \brief 是否包含
	/// \param pt 
	/// \return 
	bool IsContain(const APT& pt)const;

	/// \brief 是否包含
	/// \param rhs 
	/// \return 
	bool IsContain(const ARect& rhs)const;

	/// \brief 定位点
	/// \return 
	APT Location()const;

	/// \brief 大小
	/// \return 
	ASize Size() const;

	/// \brief 左上角
	/// \return 
	APT LeftTop()const;

	/// \brief 右上角
	/// \return 
	APT RightTop()const;

	/// \brief 左下角
	/// \return 
	APT LeftBottom()const;

	/// \brief 右下角
	/// \return 
	APT RightBottom()const;

	/// \brief 矩形偏移
	ARect operator + (const APT& pt)const;

	/// \brief 矩形偏移
	ARect operator - (const APT& pt)const;

	/// \brief 矩形偏移返回新的偏移后的矩形
	/// \param pt 
	/// \return 
	ARect OffsetTo(const APT& pt)const;

	/// \brief 矩形合并
	ARect operator + (const ARect& rect)const;
	/// \brief 合并到自身
	ARect& operator += (const ARect& rect);

	/// \brief 矩形合并
	/// \param rect 
	/// \return 
	ARect& Union(const ARect& rect);

};

/// \brief 创建一个float类型的矩形
struct GG_API ARectF
{
	float Left, Right, Top, Bottom;

	/// \brief 缺省构造函数
	ARectF();
	/// \brief 构造函数重载
	/// \param pt 
	/// \param s 
	ARectF(const APTF& pt, const ASizeF& s);
	/// \brief 构造函数重载
	/// \param pt 
	/// \param s 
	ARectF(const APT& pt, const ASize& s);
	/// \brief 构造函数重载
	/// \param rhs 
	ARectF(const ARect& rhs);
	/// \brief 构造函数重载
	/// \param rhs 
	ARectF(const ARectF& rhs);
	/// \brief 构造函数重载
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARectF(int x, int y, int w, int h);
	/// \brief 构造函数重载
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARectF(float x, float y, float w, float h);
	/// \brief 设置宽度
	/// \return 
	float Width()const;
	/// \brief 设置高度
	/// \return 
	float Height()const;

	/// \brief 
	/// \param pt 
	/// \return 
	ARectF& Offset(const APTF& pt);

	/// \brief 中心点
	/// \return 
	APTF Center()const;

	/// \brief 是否相交
	/// \param rhs 
	/// \return 
	bool IsIntersects(const ARectF& rhs)const;
	/// \brief 计算两个相交的部分
	/// \param rhs 
	/// \return 
	ARectF Intersects(const ARectF& rhs) const;

	/// \brief 是否和线相离
	/// \param x1 
	/// \param y1 
	/// \param x2 
	/// \param y2 
	/// \return 
	bool IsDisjoin(float x1, float y1, float x2, float y2)const;

	/// \brief 是否相离
	/// \param rhs 
	/// \return 
	bool IsDisjoin(const ARectF& rhs)const;

	/// \brief 是否包含
	/// \param x 
	/// \param y 
	/// \return 
	bool IsContain(float x, float y)const;

	/// \brief 是否包含
	/// \param pt 
	/// \return 
	bool IsContain(const APTF& pt)const;

	/// \brief 是否包含
	/// \param rhs 
	/// \return 
	bool IsContain(const ARectF& rhs)const;

	/// \brief 定位点
	/// \return 
	APTF Location() const;

	/// \brief 大小
	/// \return 
	ASizeF Size()const;

	/// \brief 左上角
	/// \return 
	APTF LeftTop()const;

	/// \brief 右上角
	/// \return 
	APTF RightTop()const;

	/// \brief 左下角
	/// \return 
	APTF LeftBottom()const;

	/// \brief 右下角
	/// \return 
	APTF RightBottom()const;

	/// \brief 矩形合并
	ARectF operator + (const ARectF& rect)const;
	/// \brief 合并到自身
	ARectF& operator += (const ARectF& rect);

	/// \brief 矩形合并
	/// \param rect 
	/// \return 
	ARectF& Union(const ARectF& rect);


	/// \brief 矩形偏移
	ARectF operator + (const APTF& pt)const;

	/// \brief 矩形偏移
	ARectF operator - (const APTF& pt)const;

	/// \brief 矩形偏移返回新的偏移后的矩形
	/// \param pt 
	/// \return 
	ARectF OffsetTo(const APTF& pt)const;


	/// \brief 矩形偏移
	ARectF operator + (const APT& pt)const;

	/// \brief 矩形偏移
	ARectF operator - (const APT& pt)const;

	/// \brief 矩形偏移返回新的偏移后的矩形
	/// \param pt 
	/// \return 
	ARectF OffsetTo(const APT& pt)const;


};

/// \brief RGBA4通道颜色类型，默认为读取为char *
enum class AColorBandType
{
	eBGRA32,//BGRA是指将内存块转换为char *，依次读取分别为BGRA。而不是int，下同。
	eABGR32,

	eRGBA32,
	eARGB32,

	eRGB24,
	eBGR24,
};

/// \brief 图像编码类型
enum AImageEncodeType
{
	eUnknownImage,
	ePNG,
	eJPG,
	eBMP,
	eDNG,
	eGIF,
	eTGA,
	eKTX,
	eDDS,
	eKTX2,
	eCRN,
	eTIFF,
	ePAM,
	eWEBP,
};

/// \brief 图像头信息，通过解析图像头可以得到的信息
struct AImageHeaderInfo
{
	/// \brief 图像的宽
	int Width;
	/// \brief 图像的高
	int Height;
	/// \brief 像素深度, Bits Per Pixel
	int Bpp;

	/// \brief 图像编码方式
	AImageEncodeType EncodeType;
	/// \brief 颜色编码方式
	AColorBandType RGBAType;
};
/// \brief 图像对象
class GG_API AImage
{
protected:
	AGrowByteBuffer m_Buffer;
	AImageHeaderInfo m_Info;
	AImage();
public:
	/// \brief 从另外一个图像裁切局部数据并采样到自身
	/// \param nWidth 图像的宽度
	/// \param nHeight 图像的高度 
	/// \param img  其他图像
	/// \param extent	要裁切的范围，如果范围超出了图像本身的范围则会根据范围裁剪
	AImage(unsigned int nWidth, unsigned int nHeight, AImage* img, const ARect& extent);
	/// \brief 从RGBA原始数据直接构造
	/// \param nWidth 图像的宽度
	/// \param nHeight 图像的高度 
	/// \param raw   原始数据  
	/// \param nBpp  颜色位数，  
	AImage(unsigned int nWidth, unsigned int nHeight, const unsigned char* raw, int nBpp = 32);

	/// \brief 构造指定颜色格式的位图，因为颜色格式不支持修改，因此需要创建非32位图像是，通过此构造函数来创建位图
	/// \param nWidth
	/// \param nHeight
	/// \param eColorType
	AImage(unsigned int nWidth, unsigned int nHeight, AColorBandType eColorType = AColorBandType::eRGBA32);

	/// \brief 解码文件格式的图像
	/// \param strFile 
	AImage(const char* strFile);
	/// \brief 解码内存中的图像
	/// \param blob 
	/// \param nLen 
	AImage(const unsigned char* blob, int nLen);
	/// \brief 析构函数
	virtual ~AImage();


	/// \brief 拷贝
	/// \param pImage 
	/// \return 
	virtual bool CopyFrom(AImage* pImage);

	/// \brief 拷贝外部数据到自身
	/// \param buff 
	/// \param nLen 
	/// \param eType 
	/// \return 
	virtual bool FillImageData(const unsigned char* buff, int nLen, AColorBandType eType);

	/// \brief 拷贝自身的数据到外部影像
	/// \param buff 
	/// \param nLen 
	/// \param eType 
	/// \return 
	virtual int CopyImageData(unsigned char* buff, int nLen, AColorBandType eType);


	/// \brief 获取图片颜色格式
	/// \return 
	virtual AColorBandType RGBAType();

	/// \brief 获得宽度
	/// \return 
	virtual unsigned int Width();
	/// \brief 获得高度
	/// \return 
	virtual unsigned int Height();

	/// \brief 获取图像的字节数组，并非所有图像都能够成功
	/// \return 
	virtual const unsigned char* Bit();

	/// \brief 图像一行的字节长度
	/// \return 
	unsigned int Stride();
	static unsigned int Stride(unsigned int w, AColorBandType  t);
	/// \brief 获取每像素所占字节大小
	static unsigned int BytePerPixcel(AColorBandType t);

	/// \brief 获取某行的首指针
	/// \param r 
	/// \return 
	virtual const unsigned char* Row(int r);


	/// \brief 保存图像为文件
	/// \param strFile 保存到文件
	/// \param type 图像编码类型
	/// \param params  图像编码参数
	/// \return true 成功 false失败
	virtual bool Save(const char* strFile, AImageEncodeType type );

	/// \brief 保存图像到buff的内存块
	/// \param pBuffer 保存到buff内存块
	/// \param type 图像编码类型
	/// \param params  图像编码参数
	/// \return true 成功 false失败
	virtual bool Save(AGrowByteBuffer* buff, AImageEncodeType type);

	/// \brief 从文件载入生成图像
	/// \param strFile 
	/// \return 
	static AImage* LoadFrom(const char* strFile);

	/// \brief 从内存块载入生成图像对象，编码后的数据，比如直接从PNG里面读取的二进制数据
	/// \param pBlob 
	/// \param nLen 
	/// \return 
	static AImage* LoadFrom(const unsigned char* pBlob, int nLen);

	/// \brief 判断一个影像编码内存块的编码类型
	/// \param buff 
	/// \param nLen 
	/// \return 
	static AImageEncodeType EncodeType(const unsigned char* buff, int nLen);

	/// \brief 判断一个影像文件的编码类型
	/// \param file 
	/// \return 
	static AImageEncodeType EncodeType(const char* file);

	/// \brief 只解析图像头信息
	/// \param file 图像文件完整路径
	/// \info 返回解析的图像头信息
	/// \return 返回是否解析成功。	
	static bool ParseImageHeaderInfo(const char* file, AImageHeaderInfo* info);

	/// \brief 只解析图像头信息
	/// \param blob 图像数据
	/// \param len 图像数据的长度
	/// \info 返回解析的图像头信息
	/// \return 返回是否解析成功。
	static bool ParseImageHeaderInfo(const unsigned char* blob, unsigned int len, AImageHeaderInfo* info);

	static int RGBAIndex(AColorBandType t, char c);

	bool IsValid()const  { return m_LoadOK; };
	protected:
		bool m_LoadOK = false;
		bool Init(const unsigned char* blob, int nLen);
	
};

