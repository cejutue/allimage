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

/// \brief ��ѧ��صĺ���
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
	/// \brief ���������뵽2��N�η�
	static unsigned int RoundUpPowerOf2(unsigned int n);

	/// \brief ���뵽2��N�η�
	static unsigned int NextPowerOfTwo(unsigned int v);
	/// \brief ���뵽2��N�η�
	static unsigned long long NextPowerOfTwo(unsigned long long v);

	/// \brief �ж������Ƿ���뵽2��N�η�
	static bool IsPowerOfTwo(unsigned long long n);

	/// \brief �ж�����double�Ƿ����
	static bool IsEqual(double d1, double d2);

	/// \brief �ж�����float�Ƿ����
	static bool IsEqual(float f1, float f2);

	/// \brief �ж�����double�Ƿ����
	static bool IsEqual(double d1, double d2, double tol);

	/// \brief �ж�����float�Ƿ����
	static bool IsEqual(float f1, float f2, float tol);


	/// \brief �������뵽���ڽ�������
	/// \param place����С��λ��
	static double Round(double r, unsigned int place = 0);

	/// \brief �������뵽���ڽ�������
	/// \param place����С��λ��
	static float Round(float r, unsigned int place = 0);

	/// \brief ��ת��Ϊ����
	static double ToRadian(double degree);
	/// \brief ����ת��Ϊ��
	static double ToDegree(double radian);

	/// \brief ��Чdouble
	/// \details Not-A-Number����һ�����ֵ���д�����Ա�ʾ��Ч��doubleֵ��0/0.����Ϊ0�Ľ��
	static double NaN();
	/// \brief ��Чfloat
	/// \details Not-A-Number����һ�����ֵ���д�����Ա�ʾ��Ч��floatֵ��0/0.����Ϊ0�Ľ��
	static float NaNF();

	/// \brief �ж�һ��doubleֵ�Ƿ�����Ч��Doubleֵ
	static bool IsNaN(double val);

	/// \brief ��ȡԲ����PI
	static double Pi();



	/// \brief ��ȡ��Ȼ����
	static double Log(double val);


	/// \brief �������Сֵ֮���ȡ
	template<typename T>
	static T ClampTo(T v, T minimum, T maximum) { return v<minimum ? minimum : v>maximum ? maximum : v; }

	/// \brief ��ȡ��Сֵ
	template<typename T>
	static T ClampAbove(T v, T minimum) { return v < minimum ? minimum : v; }

	/// \brief ��ȡ���ֵ
	template<typename T>
	static T ClampBelow(T v, T maximum) { return v > maximum ? maximum : v; }

	/// \brief ��ͬClampTo
	template<typename T>
	static T ClampBetween(T v, T minimum, T maximum)
	{
		return ClampBelow(ClampAbove(v, minimum), maximum);
	}

	/// \brief ��ȡ����
	template<typename T>
	static T Sign(T v) { return v < (T)0 ? (T)-1 : (T)1; }

	/// \brief ��ȡ����
	template<typename T>
	static T SignOrZero(T v) { return v < (T)0 ? (T)-1 : (v > (T)0 ? (T)1 : 0); }

	/// \brief ƽ��
	template<typename T>
	T Square(T v) { return v * v; }

	/// \brief ������ƽ��
	template<typename T>
	static T SignedSquare(T v) { return v < (T)0 ? -v * v : v * v; }
};

/// \brief ֱ���ڴ��
class GG_API AByteBuffer
{
protected:
	/// \brief �ܱ����Ĺ��캯����ֻ��������ܴ�������
	AByteBuffer();
public:
	virtual ~AByteBuffer();

	/// \brief ����ת�����غ�����ת��Ϊ�޷����ַ�ָ������
	virtual operator unsigned char* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ�ַ�ָ������
	virtual operator char* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ����ָ������
	virtual operator int* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ�޷�������ָ������
	virtual operator unsigned int* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ16λ����ָ������
	virtual operator short* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ�޷���16λ����ָ������
	virtual operator unsigned short* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ64λ����ָ������
	virtual operator long long* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ�޷���64λ����ָ������
	virtual operator unsigned long long* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ64λ����ָ������
	virtual operator double* ()const;
	/// \brief ����ת�����غ�����ת��Ϊ32λ����ָ������
	virtual operator float* ()const;
	/// \brief ����ת�����غ�����ת��ΪBOOLָ������
	virtual operator bool* ()const;

	/// \brief ��������ͷָ�룬�൱��BufferHead()
	/// \return 
	virtual unsigned char* Ptr() const = 0;

	/// \brief ��������βָ��
	/// \details >=��ָ���ʾ�����˻�������Χ
	/// \return 
	virtual unsigned char* EndPtr() const;

	/// \brief ����ģ��
	template<typename T>
	T* PtrT() const
	{
		return (T*)(Ptr());
	}

	/// \brief ����ģ��
	template<typename T>
	T* EndPtrT() const
	{
		return (T*)(EndPtr());
	}

	/// \brief ��������ͷָ��
	/// \return 
	virtual unsigned char* BufferHead()const = 0;
	/// \brief �������ĳ���
	/// \return 
	virtual unsigned int BufferSize() const = 0;

	/// \brief ���սṹ���ͼ����Ԫ������
	template<typename T>
	/// \brief 
	/// \return 
	unsigned int BufferSizeT() const
	{
		return BufferSize() / sizeof(T);
	}

	/// \brief ���û��������ֽ�ֵΪ�ض�ֵ
	/// \details ����memset�Ĺ���
	/// \param nValue 
	/// \return 
	virtual unsigned char* SetBufferValue(int nValue) = 0;

	/// \brief ���սṹ���ͼ����Ԫ������
	template<typename T>
	T* SetBufferValueT(const T& val)
	{
		//��ȡ���ݵ��ֽ�
		const unsigned char* pByte = (const unsigned char*)&val;
		//�ж���������Ϊ�ظ��ֽڣ�����ȫ��Ϊ0
		bool bIsEqual = true;
		for (int i = 1;i < sizeof(val);i++)
		{
			if (pByte[i] != pByte[0])
			{
				bIsEqual = false;
				break;
			}
		}
		//�������Ϊ�ظ��ֽڡ�
		if (bIsEqual)
			return (T*)SetBufferValue(pByte[0]);
		//����Ƿ��ظ��ֽ�����Ҫ�����ֵ
		T* pHead = PtrT<T>();
		T* pEnd = EndPtrT<T>();
		for (;pHead < pEnd;pHead++)
		{
			pHead[0] = val;
		}
		return PtrT<T>();
	}

	/// \brief ��β�����һ������
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Append(const unsigned char* pBuff, int nLen) = 0;
	/// \brief ��β������һ���ַ���
	/// \param pStr 
	/// \return 
	virtual unsigned char* Append(const char* pStr) = 0;
	/// \brief ��β������һ���ض��ṹ������
	template<typename T>
	/// \brief 
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	unsigned char* AppendT(const T* pBuff, int nLen)
	{
		return Append((const unsigned char*)pBuff, nLen * sizeof(T));
	}
	/// \brief ��β������һ���ض��ṹ����
	template<typename T>
	/// \brief 
	/// \param pBuff 
	/// \return 
	unsigned char* AppendT(const T& pBuff)
	{
		return Append((const unsigned char*)&pBuff, sizeof(T));
	}
	/// \brief ���ض�λ�ø�������
	/// \param nPos 
	/// \param pStr 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Insert(unsigned int nPos, const unsigned char* pStr, int nLen) = 0;
	/// \brief �����ض��ֽڳ��ȵĻ�������
	/// \param nLen 
	/// \return 
	virtual unsigned char* Allocate(unsigned int nLen) = 0;
	/// \brief ����ģ�壬�����ڴ�
	template<typename T>
	T* AllocateT(int nLen)
	{
		Allocate(sizeof(T) * nLen);
		return PtrT<T>();
	}
	/// \brief ����ڴ�����
	virtual void Clear() = 0;
	/// \brief �ͷ��ڴ�
	virtual void Reset() = 0;
	/// \brief ���������ڴ�����ض����ȵ�����
	virtual unsigned char* Copy(const unsigned char* pBuff, int nLen) = 0;

	/// \brief ���������ڴ��ת��Ϊbase64�����ַ���
	/// \return 
	std::string ToBase64()const;

	/// \brief ��Base64�ַ��������Ϊ�ڴ��
	/// \param strBase64 
	/// \return 
	bool FromBase64(const char* strBase64);

	/// \brief �ж��ַ����Ƿ���һ��base64������ַ���
	/// \param strBase64 
	/// \return 
	static bool IsBase64(const char* strBase64);

	/// \brief ��β������һ���ַ���
	/// \param str ����ַ���
	/// \return 
	virtual unsigned char* Append(const std::string& str);


};

/// \brief ������ֱ���ڴ�飬�ڴ�ֻ���Ӳ�����
class GG_API AGrowByteBuffer :public AByteBuffer
{
	/// \brief ������
	union {
		unsigned char* m_pBuffer;
		/// \brief �ṹ�壬m_pMetaΪ�ṹ�����͵�ָ��
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
	/// \brief ���캯��������ָ���ַ�����Ա����
	/// \param pBuffer 
	AGrowByteBuffer(unsigned char* pBuffer);
	/// \brief ���캯��������nLen���ַ��ڴ�
	/// \param pBuffer 
	/// \param nLen 
	AGrowByteBuffer(const unsigned char* pBuffer, int nLen);
	/// \brief ��������
	/// \param rhs 
	AGrowByteBuffer(const AGrowByteBuffer& rhs);

	/// \brief �ƶ�����
	AGrowByteBuffer(AGrowByteBuffer&& rhs);

	virtual ~AGrowByteBuffer();
	/// \brief �ͷ��ڴ棬����ָ���ַ
	/// \param pBuffer 
	void Attach(unsigned char* pBuffer);
	/// \brief ����ָ���ַ
	/// \return 
	unsigned char* Detach();
	/// \brief ����ת��
	virtual operator unsigned char* () const;
	/// \brief ����8���ַ��ڴ�
	/// \return 
	virtual unsigned char* Ptr() const;
	/// \brief ����8���ַ��ڴ�
	/// \return 
	virtual unsigned char* BufferHead()const;
	/// \brief ����nLen���ַ��ڴ�
	/// \return 
	virtual unsigned int BufferSize() const;
	/// \brief 
	/// \return 
	virtual unsigned int RealSize() const;
	/// \brief ����int�������ݵ�ǰreal���ֽ�
	/// \param nValue 
	/// \return 
	virtual unsigned char* SetBufferValue(int nValue);
	/// \brief �����ڴ�
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Append(const unsigned char* pBuff, int nLen);
	/// \brief �������أ������ڴ�
	/// \param pStr 
	/// \return 
	virtual unsigned char* Append(const char* pStr);
	/// \brief 
	/// \param nPos 
	/// \param pStr 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Insert(unsigned int nPos, const unsigned char* pStr, int nLen);
	/// \brief ����nLen���ֽ��ڴ�
	/// \param nLen 
	/// \return 
	virtual unsigned char* Allocate(unsigned int nLen);
	/// \brief ����ڴ�����
	virtual void Clear();
	/// \brief �ͷ��ڴ�
	virtual void Reset();
	/// \brief ����nLen���ֽ�
	/// \param pBuff 
	/// \param nLen 
	/// \return 
	virtual unsigned char* Copy(const unsigned char* pBuff, int nLen);

	/// \brief ��������������������
	/// \param rhs 
	/// \return 
	AGrowByteBuffer* Swap(AGrowByteBuffer& rhs);

	/// \brief ��������������������
	/// \param rhs 
	/// \return 
	AGrowByteBuffer* Swap(unsigned char* pBuff);
	/// \brief ��ֵ
	AGrowByteBuffer& operator=(const AGrowByteBuffer& rsh);
	/// \brief ��ֵ
	AGrowByteBuffer& operator=(AGrowByteBuffer&& rsh);


};

/// \brief ����һ��int���͵ĵ�ṹ��
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

	/// \brief ��Offset��
	APT operator +(const APT& pt) const;

	/// \brief offset����
	APT& operator +=(const APT& pt);

	/// \brief ��Offset��
	APT operator -(const APT& pt) const;

	/// \brief offset����
	APT& operator -=(const APT& pt);
};

/// \brief ����һ��float���͵ĵ�ṹ��
struct GG_API APTF
{
	float X, Y;

	/// \brief ȱʡ���캯��
	APTF();
	/// \brief ���ع��캯��
	/// \param rhs 
	APTF(const APT& rhs);
	/// \brief ���ع��캯��
	/// \param rhs 
	APTF(const APTF& rhs);
	/// \brief ���ع��캯��
	/// \param x 
	/// \param y 
	APTF(int x, int y);
	/// \brief ���ع��캯��
	/// \param x 
	/// \param y 
	APTF(float x, float y);

	/// \brief ��Offset��
	APTF operator +(const APTF& pt) const;

	/// \brief offset����
	APTF& operator +=(const APTF& pt);

	/// \brief ��Offset��
	APTF operator -(const APTF& pt) const;

	/// \brief offset����
	APTF& operator -=(const APTF& pt);
};

/// \brief ����int�Ϳ�Ⱥ͸߶�
struct GG_API ASize
{
	int Width, Height;
	/// \brief ȱʡ���캯��
	ASize();
	/// \brief ���ع��캯��
	/// \param s 
	ASize(const ASize& s);
	/// \brief �ӵ㹹��
	/// \param p 
	ASize(const APT& p);
	/// \brief ���ع��캯��
	/// \param w 
	/// \param h 
	ASize(int w, int h);

	/// \brief �Ƿ����
	bool operator == (const ASize& s)const;
	/// \brief ��ֵ
	ASize& operator = (const ASize& s);
	/// \brief �Ƿ񲻵�
	bool operator != (const ASize& s)const;

	/// \brief ��߹��ɵľ������
	int Area()const;

};

/// \brief ����float�Ϳ�Ⱥ͸߶� 
struct GG_API ASizeF
{
	float Width, Height;
	/// \brief ȱʡ���캯��
	ASizeF();
	/// \brief ���ع��캯��
	/// \param s 
	ASizeF(const ASize& s);
	/// \brief ���ع��캯��
	/// \param s 
	ASizeF(const ASizeF& s);
	/// \brief ���ع��캯��
	/// \param w 
	/// \param h 
	ASizeF(float w, float h);

	/// \brief �Ƿ����
	bool operator == (const ASizeF& s)const;
	/// \brief ��ֵ
	ASizeF& operator = (const ASizeF& s);
	/// \brief ��ֵ
	ASizeF& operator = (const ASize& s);

	/// \brief �Ƿ񲻵�
	bool operator != (const ASizeF& s)const;
};

/// \brief ����Double�Ϳ�Ⱥ͸߶�
struct GG_API ASizeD
{
	double Width, Height;
	/// \brief ȱʡ���캯��
	ASizeD();
	/// \brief ���ع��캯��
	/// \param s 
	ASizeD(const ASize& s);
	/// \brief ���ع��캯��
	/// \param s 
	ASizeD(const ASizeF& s);
	/// \brief ���ع��캯��
	/// \param s 
	ASizeD(const ASizeD& s);

	/// \brief ���ع��캯��
	/// \param w 
	/// \param h 
	ASizeD(double w, double h);

	/// \brief �Ƿ����
	bool operator == (const ASizeD& s)const;
	/// \brief �Ƿ񲻵�
	bool operator != (const ASizeD& s)const;
	/// \brief ��ֵ
	ASizeD& operator = (const ASizeD& s);
	/// \brief ��ֵ
	ASizeD& operator = (const ASize& s);
	/// \brief ��ֵ
	ASizeD& operator = (const ASizeF& s);

};

/// \brief ����һ��int���͵ľ���
struct GG_API ARect
{
	int Left, Right, Top, Bottom;
	/// \brief ȱʡ���캯��
	ARect();
	/// \brief ���ع��캯��
	/// \param pt 
	/// \param s 
	ARect(const APT& pt, const ASize& s);
	/// \brief ���ع��캯��
	/// \param rhs 
	ARect(const ARect& rhs);
	/// \brief ���ع��캯��
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARect(int x, int y, int w, int h);
	/// \brief ���ÿ��
	/// \return 
	int Width()const;
	/// \brief ���ø߶�
	/// \return 
	int Height()const;
	/// \brief ƫ��һ��λ��
	/// \param pt 
	/// \return 
	ARect& Offset(const APT& pt);

	/// \brief ���ĵ�
	/// \return 
	APT Center()const;

	/// \brief �Ƿ��ཻ
	/// \param rhs 
	/// \return 
	bool IsIntersects(const ARect& rhs)const;

	/// \brief ���������ཻ�Ĳ���
	/// \param rhs 
	/// \return 
	ARect Intersects(const ARect& rhs) const;

	/// \brief �Ƿ��������
	/// \param x1 
	/// \param y1 
	/// \param x2 
	/// \param y2 
	/// \return 
	bool IsDisjoin(int x1, int y1, int x2, int y2)const;

	/// \brief �Ƿ�����
	/// \param rhs 
	/// \return 
	bool IsDisjoin(const ARect& rhs)const;

	/// \brief �Ƿ����
	/// \param x 
	/// \param y 
	/// \return 
	bool IsContain(int x, int y)const;

	/// \brief �Ƿ����
	/// \param pt 
	/// \return 
	bool IsContain(const APT& pt)const;

	/// \brief �Ƿ����
	/// \param rhs 
	/// \return 
	bool IsContain(const ARect& rhs)const;

	/// \brief ��λ��
	/// \return 
	APT Location()const;

	/// \brief ��С
	/// \return 
	ASize Size() const;

	/// \brief ���Ͻ�
	/// \return 
	APT LeftTop()const;

	/// \brief ���Ͻ�
	/// \return 
	APT RightTop()const;

	/// \brief ���½�
	/// \return 
	APT LeftBottom()const;

	/// \brief ���½�
	/// \return 
	APT RightBottom()const;

	/// \brief ����ƫ��
	ARect operator + (const APT& pt)const;

	/// \brief ����ƫ��
	ARect operator - (const APT& pt)const;

	/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
	/// \param pt 
	/// \return 
	ARect OffsetTo(const APT& pt)const;

	/// \brief ���κϲ�
	ARect operator + (const ARect& rect)const;
	/// \brief �ϲ�������
	ARect& operator += (const ARect& rect);

	/// \brief ���κϲ�
	/// \param rect 
	/// \return 
	ARect& Union(const ARect& rect);

};

/// \brief ����һ��float���͵ľ���
struct GG_API ARectF
{
	float Left, Right, Top, Bottom;

	/// \brief ȱʡ���캯��
	ARectF();
	/// \brief ���캯������
	/// \param pt 
	/// \param s 
	ARectF(const APTF& pt, const ASizeF& s);
	/// \brief ���캯������
	/// \param pt 
	/// \param s 
	ARectF(const APT& pt, const ASize& s);
	/// \brief ���캯������
	/// \param rhs 
	ARectF(const ARect& rhs);
	/// \brief ���캯������
	/// \param rhs 
	ARectF(const ARectF& rhs);
	/// \brief ���캯������
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARectF(int x, int y, int w, int h);
	/// \brief ���캯������
	/// \param x 
	/// \param y 
	/// \param w 
	/// \param h 
	ARectF(float x, float y, float w, float h);
	/// \brief ���ÿ��
	/// \return 
	float Width()const;
	/// \brief ���ø߶�
	/// \return 
	float Height()const;

	/// \brief 
	/// \param pt 
	/// \return 
	ARectF& Offset(const APTF& pt);

	/// \brief ���ĵ�
	/// \return 
	APTF Center()const;

	/// \brief �Ƿ��ཻ
	/// \param rhs 
	/// \return 
	bool IsIntersects(const ARectF& rhs)const;
	/// \brief ���������ཻ�Ĳ���
	/// \param rhs 
	/// \return 
	ARectF Intersects(const ARectF& rhs) const;

	/// \brief �Ƿ��������
	/// \param x1 
	/// \param y1 
	/// \param x2 
	/// \param y2 
	/// \return 
	bool IsDisjoin(float x1, float y1, float x2, float y2)const;

	/// \brief �Ƿ�����
	/// \param rhs 
	/// \return 
	bool IsDisjoin(const ARectF& rhs)const;

	/// \brief �Ƿ����
	/// \param x 
	/// \param y 
	/// \return 
	bool IsContain(float x, float y)const;

	/// \brief �Ƿ����
	/// \param pt 
	/// \return 
	bool IsContain(const APTF& pt)const;

	/// \brief �Ƿ����
	/// \param rhs 
	/// \return 
	bool IsContain(const ARectF& rhs)const;

	/// \brief ��λ��
	/// \return 
	APTF Location() const;

	/// \brief ��С
	/// \return 
	ASizeF Size()const;

	/// \brief ���Ͻ�
	/// \return 
	APTF LeftTop()const;

	/// \brief ���Ͻ�
	/// \return 
	APTF RightTop()const;

	/// \brief ���½�
	/// \return 
	APTF LeftBottom()const;

	/// \brief ���½�
	/// \return 
	APTF RightBottom()const;

	/// \brief ���κϲ�
	ARectF operator + (const ARectF& rect)const;
	/// \brief �ϲ�������
	ARectF& operator += (const ARectF& rect);

	/// \brief ���κϲ�
	/// \param rect 
	/// \return 
	ARectF& Union(const ARectF& rect);


	/// \brief ����ƫ��
	ARectF operator + (const APTF& pt)const;

	/// \brief ����ƫ��
	ARectF operator - (const APTF& pt)const;

	/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
	/// \param pt 
	/// \return 
	ARectF OffsetTo(const APTF& pt)const;


	/// \brief ����ƫ��
	ARectF operator + (const APT& pt)const;

	/// \brief ����ƫ��
	ARectF operator - (const APT& pt)const;

	/// \brief ����ƫ�Ʒ����µ�ƫ�ƺ�ľ���
	/// \param pt 
	/// \return 
	ARectF OffsetTo(const APT& pt)const;


};

/// \brief RGBA4ͨ����ɫ���ͣ�Ĭ��Ϊ��ȡΪchar *
enum class AColorBandType
{
	eBGRA32,//BGRA��ָ���ڴ��ת��Ϊchar *�����ζ�ȡ�ֱ�ΪBGRA��������int����ͬ��
	eABGR32,

	eRGBA32,
	eARGB32,

	eRGB24,
	eBGR24,
};

/// \brief ͼ���������
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

/// \brief ͼ��ͷ��Ϣ��ͨ������ͼ��ͷ���Եõ�����Ϣ
struct AImageHeaderInfo
{
	/// \brief ͼ��Ŀ�
	int Width;
	/// \brief ͼ��ĸ�
	int Height;
	/// \brief �������, Bits Per Pixel
	int Bpp;

	/// \brief ͼ����뷽ʽ
	AImageEncodeType EncodeType;
	/// \brief ��ɫ���뷽ʽ
	AColorBandType RGBAType;
};
/// \brief ͼ�����
class GG_API AImage
{
protected:
	AGrowByteBuffer m_Buffer;
	AImageHeaderInfo m_Info;
	AImage();
public:
	/// \brief ������һ��ͼ����оֲ����ݲ�����������
	/// \param nWidth ͼ��Ŀ��
	/// \param nHeight ͼ��ĸ߶� 
	/// \param img  ����ͼ��
	/// \param extent	Ҫ���еķ�Χ�������Χ������ͼ����ķ�Χ�����ݷ�Χ�ü�
	AImage(unsigned int nWidth, unsigned int nHeight, AImage* img, const ARect& extent);
	/// \brief ��RGBAԭʼ����ֱ�ӹ���
	/// \param nWidth ͼ��Ŀ��
	/// \param nHeight ͼ��ĸ߶� 
	/// \param raw   ԭʼ����  
	/// \param nBpp  ��ɫλ����  
	AImage(unsigned int nWidth, unsigned int nHeight, const unsigned char* raw, int nBpp = 32);

	/// \brief ����ָ����ɫ��ʽ��λͼ����Ϊ��ɫ��ʽ��֧���޸ģ������Ҫ������32λͼ���ǣ�ͨ���˹��캯��������λͼ
	/// \param nWidth
	/// \param nHeight
	/// \param eColorType
	AImage(unsigned int nWidth, unsigned int nHeight, AColorBandType eColorType = AColorBandType::eRGBA32);

	/// \brief �����ļ���ʽ��ͼ��
	/// \param strFile 
	AImage(const char* strFile);
	/// \brief �����ڴ��е�ͼ��
	/// \param blob 
	/// \param nLen 
	AImage(const unsigned char* blob, int nLen);
	/// \brief ��������
	virtual ~AImage();


	/// \brief ����
	/// \param pImage 
	/// \return 
	virtual bool CopyFrom(AImage* pImage);

	/// \brief �����ⲿ���ݵ�����
	/// \param buff 
	/// \param nLen 
	/// \param eType 
	/// \return 
	virtual bool FillImageData(const unsigned char* buff, int nLen, AColorBandType eType);

	/// \brief ������������ݵ��ⲿӰ��
	/// \param buff 
	/// \param nLen 
	/// \param eType 
	/// \return 
	virtual int CopyImageData(unsigned char* buff, int nLen, AColorBandType eType);


	/// \brief ��ȡͼƬ��ɫ��ʽ
	/// \return 
	virtual AColorBandType RGBAType();

	/// \brief ��ÿ��
	/// \return 
	virtual unsigned int Width();
	/// \brief ��ø߶�
	/// \return 
	virtual unsigned int Height();

	/// \brief ��ȡͼ����ֽ����飬��������ͼ���ܹ��ɹ�
	/// \return 
	virtual const unsigned char* Bit();

	/// \brief ͼ��һ�е��ֽڳ���
	/// \return 
	unsigned int Stride();
	static unsigned int Stride(unsigned int w, AColorBandType  t);
	/// \brief ��ȡÿ������ռ�ֽڴ�С
	static unsigned int BytePerPixcel(AColorBandType t);

	/// \brief ��ȡĳ�е���ָ��
	/// \param r 
	/// \return 
	virtual const unsigned char* Row(int r);


	/// \brief ����ͼ��Ϊ�ļ�
	/// \param strFile ���浽�ļ�
	/// \param type ͼ���������
	/// \param params  ͼ��������
	/// \return true �ɹ� falseʧ��
	virtual bool Save(const char* strFile, AImageEncodeType type );

	/// \brief ����ͼ��buff���ڴ��
	/// \param pBuffer ���浽buff�ڴ��
	/// \param type ͼ���������
	/// \param params  ͼ��������
	/// \return true �ɹ� falseʧ��
	virtual bool Save(AGrowByteBuffer* buff, AImageEncodeType type);

	/// \brief ���ļ���������ͼ��
	/// \param strFile 
	/// \return 
	static AImage* LoadFrom(const char* strFile);

	/// \brief ���ڴ����������ͼ����󣬱��������ݣ�����ֱ�Ӵ�PNG�����ȡ�Ķ���������
	/// \param pBlob 
	/// \param nLen 
	/// \return 
	static AImage* LoadFrom(const unsigned char* pBlob, int nLen);

	/// \brief �ж�һ��Ӱ������ڴ��ı�������
	/// \param buff 
	/// \param nLen 
	/// \return 
	static AImageEncodeType EncodeType(const unsigned char* buff, int nLen);

	/// \brief �ж�һ��Ӱ���ļ��ı�������
	/// \param file 
	/// \return 
	static AImageEncodeType EncodeType(const char* file);

	/// \brief ֻ����ͼ��ͷ��Ϣ
	/// \param file ͼ���ļ�����·��
	/// \info ���ؽ�����ͼ��ͷ��Ϣ
	/// \return �����Ƿ�����ɹ���	
	static bool ParseImageHeaderInfo(const char* file, AImageHeaderInfo* info);

	/// \brief ֻ����ͼ��ͷ��Ϣ
	/// \param blob ͼ������
	/// \param len ͼ�����ݵĳ���
	/// \info ���ؽ�����ͼ��ͷ��Ϣ
	/// \return �����Ƿ�����ɹ���
	static bool ParseImageHeaderInfo(const unsigned char* blob, unsigned int len, AImageHeaderInfo* info);

	static int RGBAIndex(AColorBandType t, char c);

	bool IsValid()const  { return m_LoadOK; };
	protected:
		bool m_LoadOK = false;
		bool Init(const unsigned char* blob, int nLen);
	
};

