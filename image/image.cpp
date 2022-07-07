#include "pch.h"
#include <image.h>
#include <stringhelp.h>
#include <crthelp.h>
#include <filesystem.h>
#include <mathhelp.h>  
#include <litepng.h>

#include "imageencoder.h"
#include <string.h>
#include <stdio.h>
#include <omp.h>
#include <algorithm>
#include <regex>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "image/stb_image_resize.h"

UTILITY_NS  

GsPT::GsPT()
{
	X = 0;
	Y = 0;
}
GsPT::GsPT(const GsPT& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
} 
GsPT::GsPT(int x,int y)
{
	X = x;
	Y = y;
} 

/// \brief 点Offset点
GsPT GsPT::operator +(const GsPT & pt) const
{
	return GsPT(X + pt.X,Y + pt.Y); 
}
	
/// \brief offset自身
GsPT& GsPT::operator +=(const GsPT & pt)
{
	X+=pt.X;
	Y+=pt.Y;
	return *this;
}


/// \brief 点Offset点
GsPT GsPT::operator -(const GsPT & pt) const
{
	return GsPT(X - pt.X,Y - pt.Y); 
}
	
/// \brief offset自身
GsPT& GsPT::operator -=(const GsPT & pt)
{
	X-=pt.X;
	Y-=pt.Y;
	return *this;
}
GS_BEING_REFLECT_REGISTER(GsPT,4)
GS_REFLECT_FIELD("X", eI4);
GS_REFLECT_FIELD("Y", eI4);
GS_END_REFLECT_REGISTER;



GsPTF::GsPTF()
{
	X = 0;
	Y = 0;
}
GsPTF::GsPTF(const GsPT& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
} 
GsPTF::GsPTF(const GsPTF& rhs)
{
	X = rhs.X;
	Y = rhs.Y;
} 
GsPTF::GsPTF(int x,int y)
{
	X = x;
	Y = y;
} 
GsPTF::GsPTF(float x,float y)
{
	X = x;
	Y = y;
} 

/// \brief 点Offset点
GsPTF GsPTF::operator +(const GsPTF & pt) const
{
	return GsPTF(X + pt.X,Y + pt.Y);
}
	
/// \brief offset自身
GsPTF& GsPTF::operator +=(const GsPTF & pt)
{
	X+=pt.X;
	Y+=pt.Y;
	return *this;
}


/// \brief 点Offset点
GsPTF GsPTF::operator -(const GsPTF & pt) const
{
	return GsPTF(X - pt.X,Y - pt.Y);
}
	
/// \brief offset自身
GsPTF& GsPTF::operator -=(const GsPTF & pt)
{
	X-=pt.X;
	Y-=pt.Y;
	return *this;
}

GS_BEING_REFLECT_REGISTER(GsPTF, 4)
GS_REFLECT_FIELD("X", eR4);
GS_REFLECT_FIELD("Y", eR4);
GS_END_REFLECT_REGISTER;

GsSize::GsSize()
{
	Width = 0;
	Height =0;
}
GsSize::GsSize(const GsSize& s)
{
	Width = s.Width;
	Height = s.Height;
}
GsSize::GsSize(int w,int h)
{
	Width = w;
	Height = h;
}
/// \brief 从点构造
GsSize::GsSize(const GsPT& p)
{
	Width = p.X;
	Height = p.Y;
}
	
/// \brief 是否相等
bool GsSize::operator == (const GsSize& s)const
{
	return s.Width == Width && s.Height == Height;
}
/// \brief 复制
GsSize &GsSize::operator = (const GsSize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 是否不等
bool GsSize::operator != (const GsSize& s)const
{
	return s.Width != Width || s.Height != Height;
}
/// \brief 宽高构成的矩形面积
int GsSize::Area()const
{
	return Width * Height;
}

GS_BEING_REFLECT_REGISTER(GsSize, 4)
GS_REFLECT_FIELD("Width", eI4);
GS_REFLECT_FIELD("Height", eI4);
GS_END_REFLECT_REGISTER;


GsSizeF::GsSizeF()
{
	Width = 0;
	Height =0;
}
GsSizeF::GsSizeF(const GsSizeF& s)
{
	Width = s.Width;
	Height = s.Height;
}
GsSizeF::GsSizeF(const GsSize& s)
{
	Width = s.Width;
	Height = s.Height;
} 
GsSizeF::GsSizeF(float w, float h) 
{
	Width = w;
	Height = h;
}

/// \brief 是否相等
bool GsSizeF::operator == (const GsSizeF& s)const
{
	return GsMath::IsEqual(s.Width, Width) &&
		GsMath::IsEqual(s.Height, Height);
} 
GsSizeF& GsSizeF::operator = (const GsSizeF& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
} 
GsSizeF& GsSizeF::operator = (const GsSize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}

/// \brief 是否不等
bool GsSizeF::operator != (const GsSizeF& s)const
{
	return !GsMath::IsEqual(s.Width, Width) ||
		!GsMath::IsEqual(s.Height, Height);
}

GS_BEING_REFLECT_REGISTER(GsSizeF, 4)
GS_REFLECT_FIELD("Width", eR4);
GS_REFLECT_FIELD("Height", eR4);
GS_END_REFLECT_REGISTER;

/// \brief 缺省构造函数
GsSizeD::GsSizeD():Width(0),Height(0)
{

}
/// \brief 重载构造函数
GsSizeD::GsSizeD(const GsSize& s):Width(s.Width), Height(s.Height)
{ 
}
/// \brief 重载构造函数
GsSizeD::GsSizeD(const GsSizeF& s) : Width(s.Width), Height(s.Height)
{

}
/// \brief 重载构造函数
GsSizeD::GsSizeD(const GsSizeD& s) : Width(s.Width), Height(s.Height)
{

}

/// \brief 重载构造函数
GsSizeD::GsSizeD(double w, double h) :Width(w), Height(h)
{

}

/// \brief 是否相等
bool GsSizeD::operator == (const GsSizeD& s)const
{
	return GsMath::IsEqual(s.Width, Width) && GsMath::IsEqual(s.Height, Height);
}
/// \brief 是否不等
bool GsSizeD::operator != (const GsSizeD& s)const
{
	return !GsMath::IsEqual(s.Width, Width) || !GsMath::IsEqual(s.Height, Height);
};
/// \brief 赋值
GsSizeD &GsSizeD::operator = (const GsSizeD& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 赋值
GsSizeD &GsSizeD::operator = (const GsSize& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
/// \brief 赋值
GsSizeD &GsSizeD::operator = (const GsSizeF& s)
{
	Width = s.Width;
	Height = s.Height;
	return *this;
}
GS_BEING_REFLECT_REGISTER(GsSizeD, 4)
GS_REFLECT_FIELD("Width", eR8);
GS_REFLECT_FIELD("Height", eR8);
GS_END_REFLECT_REGISTER;


GsRect::GsRect()
{
	Left =0;
	Right =0;
	Top =0;
	Bottom =0;
}
GsRect::GsRect(const GsPT& pt,const GsSize& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X  + s.Width;
	Bottom = pt.Y + s.Height;
}
GsRect::GsRect(const GsRect& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
GsRect::GsRect(int x,int y,int w,int h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom= y + h;
}
int GsRect::Width()const
{
	return Right - Left;
}
int GsRect::Height()const
{
	return Bottom - Top;
}
GsRect& GsRect::Offset(const GsPT& pt)
{
	Left += pt.X;
	Top += pt.Y;
	Right += pt.X;
	Bottom += pt.Y;
	return *this;
}
/// \brief 中心点
GsPT GsRect::Center()const
{
	return GsPT((Left + Right)/2,(Top + Bottom)/2);
}
/// \brief 是否相交
bool GsRect::IsIntersects(const GsRect& rhs)const
{
	return !IsDisjoin(rhs);
}
/// \brief 计算两个相交的部分
GsRect GsRect::Intersects(const GsRect& rhs) const
{
	if(IsDisjoin(rhs))
		return GsRect();
	GsRect rect;
	rect.Left = GsMath::Max(rhs.Left,Left);
	rect.Top = GsMath::Max(rhs.Top,Top);

	rect.Right = GsMath::Min(rhs.Right,Right);
	rect.Bottom = GsMath::Min(rhs.Bottom,Bottom);

	return rect;
}


//描述：返回me是否和env相离
template<class T>
bool IsDisjoinT(T xmin,T ymin,T xmax,T ymax,T sx ,T sy,T ex,T ey)
{
    //以下是矩形选中线段中部的情况
    double dx,dy,x0,x1,y0,y1;

	dx = ex - sx;
	dy = ey - sy;

	x0 = xmin - sx;
	x1 = xmax - sx;
	y0 = ymin - sy;
	y1 = ymax - sy;
	
	bool a,b;
	a = x0*dy>y0*dx;
	b = x0*dy>y1*dx;
	if(a^b)
		return false;
	b = x1*dy>y0*dx;
	if(a^b)
		return false;
	b = x1*dy>y1*dx;
	if(a^b)
		return false;
	return true;
}

/// \brief 是否和线相交
bool GsRect::IsDisjoin(int x1,int y1,int x2,int y2)const
{
	//外接矩形相离则相离。
	GsRect box(GsMath::Min(x1,x2), GsMath::Min(y1,y2), GsMath::Max(x1,x2), GsMath::Max(y1,y2));
	if(IsDisjoin(box))
		return true;
	return IsDisjoinT(Left,Top,Right,Bottom,x1,y1,x2,y2);
}
/// \brief 是否相离
bool GsRect::IsDisjoin(const GsRect& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top|| Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief 是否包含
bool GsRect::IsContain(int x,int y)const
{
	 return x >=Left && x < Right && y >=Top && y < Bottom;
}

/// \brief 是否包含
bool GsRect::IsContain(const GsPT& pt)const
{
	return IsContain(pt.X,pt.Y);
}

/// \brief 是否包含
bool GsRect::IsContain(const GsRect& rhs)const
{
	return IsContain(rhs.Left, rhs.Top) && IsContain(rhs.Right - 1, rhs.Bottom - 1);
}
/// \brief 定位点
GsPT GsRect::Location()const
{
	return LeftTop();
}
	
/// \brief 大小
GsSize GsRect::Size()const
{
	return GsSize(Width(),Height());
}

/// \brief 左上角
GsPT GsRect::LeftTop()const
{
	return GsPT(Left,Top);
}

	
/// \brief 右上角
GsPT GsRect::RightTop()const
{
	return GsPT(Right,Top);
}

	
/// \brief 左下角
GsPT GsRect::LeftBottom()const
{
	return GsPT(Left,Bottom);
}

	
/// \brief 右下角
GsPT GsRect::RightBottom()const
{
	return GsPT(Right,Bottom);
}
/// \brief 矩形偏移
GsRect GsRect::operator + (const GsPT& pt)const
{
	return GsRect(Left + pt.X,Top + pt.Y,Width(),Height());
}

/// \brief 矩形偏移
GsRect GsRect::operator - (const GsPT& pt)const
{
	return GsRect(Left - pt.X,Top - pt.Y,Width(),Height());
}

/// \brief 矩形偏移返回新的偏移后的矩形
GsRect GsRect::OffsetTo(const GsPT& pt)const
{
	return GsRect(Left + pt.X,Top + pt.Y,Width(),Height());
}
	
/// \brief 矩形合并
GsRect GsRect::operator + (const GsRect& rect)const
{
	GsRect ret(*this);
	ret.Union(rect);
	return ret;
}
/// \brief 合并到自身
GsRect& GsRect::operator += (const GsRect& rect)
{
	return Union(rect);
}
	
/// \brief 矩形合并
GsRect& GsRect::Union(const GsRect& rect)
{
	Left = GsMath::Min(Left,rect.Left);
	Top = GsMath::Min(Top,rect.Top);
	Right = GsMath::Max(Right,rect.Right);
	Bottom = GsMath::Max(Bottom,rect.Bottom);
	return *this;
}

GS_BEING_REFLECT_REGISTER(GsRect, 4)
GS_REFLECT_FIELD("Left", eI4);
GS_REFLECT_FIELD("Top", eI4);
GS_REFLECT_FIELD("Right", eI4);
GS_REFLECT_FIELD("Bottom", eI4);
GS_END_REFLECT_REGISTER;


GsRectF::GsRectF()
{
	Left =0;
	Right =0;
	Top =0;
	Bottom =0;
}

GsRectF::GsRectF(const GsPTF& pt,const GsSizeF& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X  + s.Width;
	Bottom = pt.Y + s.Height;
}

GsRectF::GsRectF(const GsPT& pt,const GsSize& s)
{
	Left = pt.X;
	Top = pt.Y;
	Right = pt.X  + s.Width;
	Bottom = pt.Y + s.Height;
}
GsRectF::GsRectF(const GsRect& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
GsRectF::GsRectF(const GsRectF& rhs)
{
	Left = rhs.Left;
	Right = rhs.Right;
	Top = rhs.Top;
	Bottom = rhs.Bottom;
}
GsRectF::GsRectF(int x,int y,int w,int h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom= y + h;
}
GsRectF::GsRectF(float x,float y,float w,float h)
{
	Left = x;
	Top = y;
	Right = x + w;
	Bottom= y + h;
}
float GsRectF::Width()const
{
	return Right - Left;
}
float GsRectF::Height()const
{
	return Bottom - Top;
}
/// \brief 中心点
GsPTF GsRectF::Center()const
{
	return GsPTF((Left + Right)/2,(Top + Bottom)/2);
}
GsRectF& GsRectF::Offset(const GsPTF& pt)
{
	Left += pt.X;
	Top += pt.Y;
	Right += pt.X;
	Bottom += pt.Y;
	return *this;
}

/// \brief 是否相交
bool GsRectF::IsIntersects(const GsRectF& rhs)const
{
	return !IsDisjoin(rhs);
}

/// \brief 计算两个相交的部分
GsRectF GsRectF::Intersects(const GsRectF& rhs) const
{
	if(IsDisjoin(rhs))
		return GsRect();
	GsRectF rect;
	rect.Left = GsMath::Max(rhs.Left,Left);
	rect.Top = GsMath::Max(rhs.Top,Top);

	rect.Right = GsMath::Min(rhs.Right,Right);
	rect.Bottom = GsMath::Min(rhs.Bottom,Bottom);

	return rect;
}

 
/// \brief 是否和线相交
bool GsRectF::IsDisjoin(float x1,float y1,float x2,float y2)const
{
	//外接矩形相离则相离。
	GsRectF box((float)GsMath::Min(x1,x2), GsMath::Min(y1,y2), GsMath::Max(x1,x2), GsMath::Max(y1,y2));
	if(IsDisjoin(box))
		return true;
	return IsDisjoinT(Left,Top,Right,Bottom,x1,y1,x2,y2);
}
/// \brief 是否相离
bool GsRectF::IsDisjoin(const GsRectF& rhs)const
{
	return rhs.Right <= Left || rhs.Bottom <= Top|| Right <= rhs.Left || Bottom <= rhs.Top;
}

/// \brief 是否包含
bool GsRectF::IsContain(float x,float y)const
{
	
	 return x >=Left && x < Right && y >=Top && y < Bottom;
}

/// \brief 是否包含
bool GsRectF::IsContain(const GsPTF& pt)const
{
	return IsContain(pt.X,pt.Y);
}

/// \brief 是否包含
bool GsRectF::IsContain(const GsRectF& rhs)const
{
	
	return IsContain(rhs.Left,rhs.Top) && IsContain(rhs.Right,rhs.Bottom);
}

/// \brief 定位点
GsPTF GsRectF::Location()const
{
	return LeftTop();
}
	
/// \brief 大小
GsSizeF GsRectF::Size()const
{
	return GsSizeF(Width(),Height());
}

/// \brief 左上角
GsPTF GsRectF::LeftTop()const
{
	return GsPTF(Left,Top);
}

	
/// \brief 右上角
GsPTF GsRectF::RightTop()const
{
	return GsPTF(Right,Top);
}

	
/// \brief 左下角
GsPTF GsRectF::LeftBottom()const
{
	return GsPTF(Left,Bottom);
}

	
/// \brief 右下角
GsPTF GsRectF::RightBottom()const
{
	return GsPTF(Right,Bottom);
}
/// \brief 矩形偏移
GsRectF GsRectF::operator + (const GsPTF& pt)const
{
	return GsRectF(Left + pt.X,Top + pt.Y,Width(),Height());
}

/// \brief 矩形偏移
GsRectF GsRectF::operator - (const GsPTF& pt)const
{
	return GsRectF(Left - pt.X,Top - pt.Y,Width(),Height());
}


/// \brief 矩形偏移返回新的偏移后的矩形
GsRectF GsRectF::OffsetTo(const GsPTF& pt)const
{
	return GsRectF(Left + pt.X,Top + pt.Y,Width(),Height());
}



/// \brief 矩形偏移
GsRectF GsRectF::operator + (const GsPT& pt)const
{
	return GsRectF(Left + pt.X,Top + pt.Y,Width(),Height());
}


/// \brief 矩形偏移
GsRectF GsRectF::operator - (const GsPT& pt)const
{
	return GsRectF(Left - pt.X,Top - pt.Y,Width(),Height());
}

/// \brief 矩形偏移返回新的偏移后的矩形
GsRectF GsRectF::OffsetTo(const GsPT& pt)const
{
	return GsRectF(Left + pt.X,Top + pt.Y,Width(),Height());
}
/// \brief 矩形合并
GsRectF GsRectF::operator + (const GsRectF& rect)const
{
	GsRectF ret (*this);
	ret.Union(rect);
	return ret;
}
/// \brief 合并到自身
GsRectF& GsRectF::operator += (const GsRectF& rect)
{
	return Union(rect);
}
	
/// \brief 矩形合并
GsRectF& GsRectF::Union(const GsRectF& rect)
{
	Left = GsMath::Min(Left,rect.Left);
	Top = GsMath::Min(Top,rect.Top);
	Right = GsMath::Max(Right,rect.Right);
	Bottom = GsMath::Max(Bottom,rect.Bottom);
	return *this;
}

GS_BEING_REFLECT_REGISTER(GsRectF, 4)
GS_REFLECT_FIELD("Left", eR4);
GS_REFLECT_FIELD("Top", eR4);
GS_REFLECT_FIELD("Right", eR4);
GS_REFLECT_FIELD("Bottom", eR4);
GS_END_REFLECT_REGISTER;


GsImageEncoderParameters::GsImageEncoderParameters()
{
}

GsImageEncoderParameters::~GsImageEncoderParameters()
{
}

GsImageEncoderParameters& GsImageEncoderParameters::Add(GsImageEncoderParameterType type, double value)
{
	m_mapParameterValues[type] = value;
	return *this;
}

GsImageEncoderParameters& GsImageEncoderParameters::Add(GsImageEncoderParameterType type, int value)
{
	m_mapParameterValues[type] = value;
	return *this;
}

GsImageEncoderParameters& GsImageEncoderParameters::Add(GsImageEncoderParameterType type, const char* value)
{
	m_mapParameterValues[type] = value;
	return *this;
}

const UTILITY_NAME::GsAny& GsImageEncoderParameters::Value(GsImageEncoderParameterType type) const
{
	auto iter = m_mapParameterValues.find(type);
	if (iter != m_mapParameterValues.end())
		return iter->second;
	static UTILITY_NAME::GsAny anval;
	return anval;
}

bool GsImageEncoderParameters::Remove(GsImageEncoderParameterType type)
{
	m_mapParameterValues.erase(type);
	return true;
}

const UTILITY_NAME::GsStlMap<GsImageEncoderParameterType, UTILITY_NAME::GsAny>* GsImageEncoderParameters::ParameterValues() const
{
	return &m_mapParameterValues;
}

GsImage::GsImage()
{
	m_eRGBAType = GsRGBAType::eRGBA32;
}
GsImage::~GsImage()
{
}

/// \brief 拷贝
bool GsImage::CopyFrom(GsImage* pImage)
{
	return false; 
}

/// \brief 获取图片颜色格式

GsRGBAType GsImage::RGBAType()
{
	//目前统一存储的RGBA，所以固定返回RGBA，以后有需要再扩展
	//20200826，当前已经处于一个历史的变革期，需要扩展
	return m_eRGBAType;
}
/// \brief 影像的宽和高
GsSize GsImage::Size()
{
	return GsSize(Width(), Height());
}


bool CopyImage(unsigned char* pSrc, GsRGBAType srcType, unsigned char* pDsc, GsRGBAType dscType, int width, int height)
{
	if (dscType == srcType)
	{
		memcpy(pDsc, pSrc, GsImage::Stride(width, dscType) * height);
		return true;
	}

	int srcStride = GsImage::Stride(width, srcType);
	int dscStride = GsImage::Stride(width, dscType);
	int srcBPP = GsImage::BytePerPixcel(srcType);
	int dscBPP = GsImage::BytePerPixcel(dscType);

	int srcRIndex = GsImage::RGBAIndex(srcType, 'r');
	int srcGIndex = GsImage::RGBAIndex(srcType, 'g');
	int srcBIndex = GsImage::RGBAIndex(srcType, 'b');
	int srcAIndex = GsImage::RGBAIndex(srcType, 'a');
	int dscRIndex = GsImage::RGBAIndex(dscType, 'r');
	int dscGIndex = GsImage::RGBAIndex(dscType, 'g');
	int dscBIndex = GsImage::RGBAIndex(dscType, 'b');
	int dscAIndex = GsImage::RGBAIndex(dscType, 'a');

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
bool GsImage::FillImageData(const unsigned char* buff, int nLen, GsRGBAType eType)
{
	return CopyImage(const_cast<unsigned char*>(buff), eType, const_cast<unsigned char*>(Bit()), m_eRGBAType, Width(), Height());
}
/// \brief 将自身数据剪裁到目标图像中
/// \param target 要存储数据的图像
/// \param extent 要剪裁的数据范围
/// \param offset 存储到目标数据的偏移位置
bool GsImage::CropTo(GsImage* target, const GsRect& extent, const GsPT& offset)
{
	if (!target)
		return false;
	if (target->RGBAType() != m_eRGBAType)
	{
		GS_E << "Clipping between the same types is now supported only";
		return false;
	}
	if (extent.Width() <= 0 || extent.Height() <= 0)
		return false;
	if (offset.X >= target->Width() || offset.Y >= target->Height())
		return false;

	int w = target->Width() - offset.X;
	int h = target->Height() - offset.Y;

	int startrow = GsMath::Max(extent.Top, 0);
	int startcol = GsMath::Max(extent.Left, 0);
	int endrow = GsMath::Min(GsMath::Min(Height(), extent.Bottom), extent.Top + h);
	w = GsMath::Min(w, extent.Width());

	int bpp = BytePerPixcel(m_eRGBAType);
	int xoff = offset.X * bpp;
	for (int r = startrow; r < endrow; r++)
	{
		const unsigned char* srcRow = Row(r);
		unsigned char* targetRow = (unsigned char*)target->Row(offset.Y + r - startrow);
		memcpy(targetRow + xoff, srcRow + startcol * bpp, w * bpp);
	}

	return true;
}


/// \brief 将自身数据剪裁到目标内存块中
/// \param target 要存储数据的目标内存
/// \param nStride 目标数据行字节长度
/// \param extent 要剪裁的数据范围
bool GsImage::CropTo(unsigned char* target, int nStride, const GsRect& extent, GsRGBAType eType)
{ 
	if (!target)
		return false;
	if (extent.Width() <= 0 || extent.Height() <= 0)
		return false;
	if (eType != m_eRGBAType)
	{
		GS_E << "Clipping between the same types is now supported only";
		return false;
	}

	int startrow = GsMath::Max(extent.Top, 0);
	int startcol = GsMath::Max(extent.Left, 0);
	int endrow = GsMath::Min(Height(), extent.Bottom);
	int nCopy = GsMath::Min(extent.Width() * 4, nStride);
	for (int r = startrow; r < endrow; r++)
	{
		const unsigned char* srcRow = Row(r);
		unsigned char* targetRow = target + nStride * (r - startrow);
		memcpy(targetRow, srcRow + startcol * 4,nCopy);
	}


	return true;
}

/// \brief 将自身数据剪裁到目标内存块中
/// \param target 要存储数据的目标内存
/// \param nStride 目标数据行字节长度
/// \param extent 要剪裁的数据范围
bool GsImage::CropTo(GsByteBuffer* target, int nStride, const GsRect& extent, GsRGBAType eType)
{
	if (!target)
		return false;
	if (extent.Width() <= 0 || extent.Height() <= 0)
		return false;
	if (eType != m_eRGBAType)
	{
		GS_E << "Clipping between the same types is now supported only";
		return false;
	}

	target->Allocate(nStride * extent.Height());
	return CropTo(target->BufferHead(), nStride, extent, eType);
}

int GsImage::RGBAIndex(GsRGBAType t, char c)
{
	static GsString str = "rgbaRGBA";
	if (str.find(c) == GsString::npos)
	{
		return -1;
	}

	static const char r = 'r', g = 'g', b = 'b', a = 'a';
	static const char R = 'R', G = 'G', B = 'B', A = 'A';
	static std::map<const char, int> m = { {r, -1}, {g, -1}, {b, -1}, {a, -1} };
	switch (t)
	{
	case GeoStar::Utility::GsRGBAType::eBGRA32:
		m[b] = m[B] = 0, m[g] = m[G] = 1, m[r] = m[R] = 2, m[a] = m[A] = 3;
		break;
	case GeoStar::Utility::GsRGBAType::eABGR32:
		m[a] = m[A] = 0, m[b] = m[B] = 1, m[g] = m[G] = 2, m[r] = m[R] = 3;
		break;
	case GeoStar::Utility::GsRGBAType::eRGBA32:
		m[r] = m[R] = 0, m[g] = m[G] = 1, m[b] = m[B] = 2, m[a] = m[A] = 3;
		break;
	case GeoStar::Utility::GsRGBAType::eARGB32:
		m[a] = m[A] = 0, m[r] = m[R] = 1, m[g] = m[G] = 2, m[b] = m[B] = 3;
		break;
	case GeoStar::Utility::GsRGBAType::eRGB24:
		m[r] = m[R] = 0, m[g] = m[G] = 1, m[b] = m[B] = 2, m[a] = m[A] = -1;
		break;
	case GeoStar::Utility::GsRGBAType::eBGR24:
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

/// \brief 拷贝自身的数据到外部影像
int GsImage::CopyImageData(unsigned char* pBuff, int nLen, GsRGBAType eType)
{
	if (CopyImage(const_cast<unsigned char*>(Bit()), m_eRGBAType, pBuff, eType, Width(), Height()))
		return nLen;
	return 0;
}


/// \brief 图像一行的字节长度
unsigned int GsImage::Stride()
{
	return Width() * BytePerPixcel(m_eRGBAType);
}

unsigned int GsImage::Stride(unsigned int w, GsRGBAType t)
{
	return w * BytePerPixcel(t);
}

unsigned int GsImage::BytePerPixcel(GsRGBAType t)
{
	int size = 4;
	switch (t)
	{
	case GeoStar::Utility::GsRGBAType::eBGRA32:
	case GeoStar::Utility::GsRGBAType::eABGR32:
	case GeoStar::Utility::GsRGBAType::eRGBA32:
	case GeoStar::Utility::GsRGBAType::eARGB32:
		break;
	case GeoStar::Utility::GsRGBAType::eRGB24:
	case GeoStar::Utility::GsRGBAType::eBGR24:
		size = 3;
		break;
	default:
		break;
	}
	return size;
}

bool GsImage::ParseImageHeaderInfo(const char* file, GsImageHeaderInfo* info)
{
	if(!info || !file)
		return false;

	GsImageDecoderPtr ptrDecode = GsImageDecoder::CreateImageDecoder(file);
	if (!ptrDecode)
		return false;
	return ptrDecode->Decode(info);
}

bool GsImage::ParseImageHeaderInfo(const unsigned char* blob, unsigned int len, GsImageHeaderInfo* info)
{
	if (!blob || !info)
		return false;

	GsImageDecoderPtr ptrDecode = GsImageDecoder::CreateImageDecoder(blob,len);
	if (!ptrDecode)
		return false;
	return ptrDecode->Decode(info);
}

/// \brief 获取图像的字节数组，并非所有图像都能够成功
const unsigned char* GsImage::Row(int r)
{
	if(r <0 || r >=Height())
		return 0;
	const unsigned char* pHead = Bit();
	if(NULL == pHead)
		return 0;
	return pHead + Stride() * r;
}

GsPNGColorType RGBAType2PNGColor(GeoStar::Utility::GsRGBAType type)
{
	switch (type)
	{
	case GeoStar::Utility::GsRGBAType::eBGRA32:
		return eBGRA8;
	case GeoStar::Utility::GsRGBAType::eABGR32:
		return eABGR8;
	case GeoStar::Utility::GsRGBAType::eRGBA32:
		return eRGBA8;
	case GeoStar::Utility::GsRGBAType::eARGB32:
		return eARGB8;
	case GeoStar::Utility::GsRGBAType::eRGB24:
		return eRGB8;
	case GeoStar::Utility::GsRGBAType::eBGR24:
		return eBGR8;
	default:
		return eUnknownPNGColorType;
	}
}
/// \brief 保存图像为PNG格式文件
bool GsImage::SavePNG(const char* strFile)
{
	auto h = Height();
	GsFilePNG png(GsFile(strFile).Path(), Width(), h, RGBAType2PNGColor(m_eRGBAType));
	if (!png.BeginSave())
		return false;
	for (int i = 0; i < h; i++)
	{
		const unsigned char* pRow = Row(i);
		if (pRow)
			png.SaveRow(pRow);
	}
	png.FinishSave();
	return true;
}
	
/// \brief 保存图像为PNG格式的内存块。
bool GsImage::SavePNG(GsByteBuffer* pBuffer)
{
	auto h = Height();
	GsMemoryPNG png(pBuffer, Width(), h, RGBAType2PNGColor(m_eRGBAType));
	png.BeginSave();
	for (int i = 0; i < h; i++)
	{
		const unsigned char* pRow = Row(i);
		if (pRow)
			png.SaveRow(pRow);
	}
	png.FinishSave();
	return true;
}


/// \brief 保存图像为JPEG格式文件
bool GsImage::SaveJPEG(const char* strFile,int nQuality)
{
	ImageFile file(GsFile(strFile).Path());
	if (!file.IsOpen())
		return false;
	JpegImageEncoder encoder(&file);
	encoder.Quality(nQuality);
	auto h = Height();
	encoder.Begin(Width(),h,Stride(),BytePerPixcel(m_eRGBAType) * 8);
	for (int i = 0; i < h; i++)
		encoder.WriteRow(Row(i), Stride());
	encoder.End();
	return true;

}
	
/// \brief 保存图像为JPEG格式的内存块。
bool GsImage::SaveJPEG(GsByteBuffer* pBuffer,int nQuality)
{
	ImageBuffer file(pBuffer);
	JpegImageEncoder encoder(&file);
	encoder.Quality(nQuality);
	auto h = Height();
	encoder.Begin(Width(), h, Stride(), BytePerPixcel(m_eRGBAType) * 8);
	for (int i = 0; i < h; i++)
		encoder.WriteRow(Row(i), Stride());
	encoder.End(); 
	return true;
}

bool GsImage::Save(const char* strFile, UTILITY_NAME::GsImageEncodeType type, const GsImageEncoderParameters& params)
{
	if (!strFile)
		return false;

	switch (type)
	{
	case GeoStar::Utility::eUnknownImage:
		break;
	case GeoStar::Utility::ePNG:
		return GsImage::SavePNG(strFile);
	case GeoStar::Utility::eJPG:
	{
		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 100)
			return GsImage::SaveJPEG(strFile, any.AsInt());

		return GsImage::SaveJPEG(strFile);
	}
	case GeoStar::Utility::eBMP:
		break;
	case GeoStar::Utility::eDNG:
		break;
	case GeoStar::Utility::eGIF:
		break;
	case GeoStar::Utility::eTGA:
		break;
	case GeoStar::Utility::eKTX:
	case GeoStar::Utility::eDDS:
	case GeoStar::Utility::eCRN:
	case GeoStar::Utility::eKTX2:
	{
		GsSize size = { (int)Width(), (int)Height() };
		GsTextureEncoderPtr ptrEncoder = new GsTextureEncoder(strFile, size, type);
		
		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 255)
		{
			ptrEncoder->QualityLevel() = any.AsInt();
		}
		const GsAny& any2 = params.Value(GsImageEncoderParameterType::eCompression);
		if (any2.Type == eI4)
		{
			GsTexturePixelFormat type = (GsTexturePixelFormat)(any2.AsInt());
			ptrEncoder->PixelFormat() = type;
		}
		bool b = ptrEncoder->BeginEncode();
		if (!b) return false;
		b = ptrEncoder->Write(this);  
		if (!b) return false;
		b = ptrEncoder->FinishEncode();
		if (!b) return false;
		return b;
	}
	case GeoStar::Utility::eTIFF:
		break;
	case GeoStar::Utility::ePAM:
		break;
	case GeoStar::Utility::eWEBP:
	{
		ImageFile file(GsFile(strFile).Path());
		if (!file.IsOpen())
			return false;
		WEBPImageEncoder encoder(&file);
		encoder.RGBAType(m_eRGBAType);

		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 100)
			encoder.Quality(any.AsInt());

		const GsAny& any2 = params.Value(GsImageEncoderParameterType::eSpeed);
		if (any2.Type == eI4 && any2.AsInt() >= 1 && any2.AsInt() <= 6)
			encoder.Speed(any2.AsInt());
		encoder.Begin(Width(), Height(), Stride(), BytePerPixcel(m_eRGBAType) * 8);
		for (int i = 0; i < Height(); i++)
			encoder.WriteRow(Row(i), Stride(), i);
		encoder.End();
		return true;
	}
	default:
		break;
	}

	return false;
}

bool GsImage::Save(GsByteBuffer* pBuffer, UTILITY_NAME::GsImageEncodeType type, const GsImageEncoderParameters& params)
{
	if (!pBuffer)
		return false;

	switch (type)
	{
	case GeoStar::Utility::eUnknownImage:
		break;
	case GeoStar::Utility::ePNG:
		return GsImage::SavePNG(pBuffer);
	case GeoStar::Utility::eJPG:
	{
		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 100)
			return GsImage::SaveJPEG(pBuffer, any.AsInt());

		return GsImage::SaveJPEG(pBuffer);
	}
	case GeoStar::Utility::eBMP:
		break;
	case GeoStar::Utility::eDNG:
		break;
	case GeoStar::Utility::eGIF:
		break;
	case GeoStar::Utility::eTGA:
		break;
	case GeoStar::Utility::eKTX:
	case GeoStar::Utility::eDDS:
	case GeoStar::Utility::eCRN:
	case GeoStar::Utility::eKTX2:
	{
		GsSize size = { (int)Width(),(int)Height() };
		GsTextureEncoderPtr ptrEncoder = new GsTextureEncoder(pBuffer, size, type);

		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 255)
		{
			ptrEncoder->QualityLevel() = any.AsInt();
		}
		const GsAny& any2 = params.Value(GsImageEncoderParameterType::eCompression);
		if (any2.Type == eI4)
		{
			GsTexturePixelFormat type = (GsTexturePixelFormat)(any2.AsInt());
			ptrEncoder->PixelFormat() = type;
		}
		bool b = ptrEncoder->BeginEncode();
		if (!b) return false;
		b = ptrEncoder->Write(this);
		if (!b) return false;
		b = ptrEncoder->FinishEncode();
		if (!b) return false;
		return b;
	}
	case GeoStar::Utility::eTIFF:
		break;
	case GeoStar::Utility::ePAM:
		break;
	case GeoStar::Utility::eWEBP:
	{
		ImageBuffer mem(pBuffer);
		WEBPImageEncoder encoder(&mem);
		encoder.RGBAType(m_eRGBAType);

		const GsAny& any = params.Value(GsImageEncoderParameterType::eQuality);
		if (any.Type == eI4 && any.AsInt() >= 0 && any.AsInt() <= 100)
			encoder.Quality(any.AsInt());

		const GsAny& any2 = params.Value(GsImageEncoderParameterType::eSpeed);
		if (any2.Type == eI4 && any2.AsInt() >= 1 && any2.AsInt() <= 6)
			encoder.Speed(any2.AsInt());
		encoder.Begin(Width(), Height(), Stride(), BytePerPixcel(m_eRGBAType) * 8);
		for (int i = 0; i < Height(); i++)
			encoder.WriteRow(Row(i), Stride(), i);
		encoder.End();
		return true;
	}
	default:
		break;
	}

	return false;
}


/// \brief 从文件载入生成图像
GsSmarterPtr<GsImage> GsImage::LoadFrom(const char* strFile)
{ 
	GsFile f(strFile);
	const GsString& ext = f.Extension();
	//判断svg文件的情况。
	if(GsCRT::_stricmp(ext.c_str(),"svg") ==0)
	{
		GsSVGImagePtr ptrSVG = new GsSVGImage(strFile,true);
		if(ptrSVG->IsValid())
			return ptrSVG;
	}
	//判断svg文件的情况。
	else if (GsCRT::_stricmp(ext.c_str(), "pam") == 0)
	{
		GsPAMBitmapPtr ptrPAM = new GsPAMBitmap(strFile);
		return ptrPAM;
	}
	else if (GsCRT::_stricmp(ext.c_str(), "ktx2") == 0)
	{
		GsTextureDecoderPtr ptrDec = new GsTextureDecoder(strFile);
		ptrDec->Decode();
		return ptrDec->Image();
	}

	GsSimpleBitmapPtr ptrBitmap = new GsSimpleBitmap(strFile);
	if(ptrBitmap->Width() == 0 ||ptrBitmap->Height() ==0)
		return 0;
	return ptrBitmap;
}
/// \brief 从内存块载入生成图像对象
GsSmarterPtr<GsImage> GsImage::LoadFrom(const unsigned char *pBlob,int nLen)
{
	if(nLen > 6)
	{
		if(GsStringHelp::StartWith((const char*)pBlob,"<?xml ",true) ||
			GsStringHelp::StartWith((const char*)pBlob,"<svg",true) )
		{
            GsGrowByteBuffer buffer;
            static const unsigned char end = 0;
            buffer.Append(pBlob, nLen);
            buffer.Append(&end, 1);

			GsSVGImagePtr ptrSVG = new GsSVGImage((const char*)buffer.BufferHead(),false);
			if(ptrSVG->IsValid())
				return ptrSVG;
		}
	}
	//如果内容的头是PAM的头。
	if (nLen > 3 && strncmp((const char*)pBlob, "P7\n", 3) == 0)
	{
		GsPAMBitmapPtr ptrBmp = new GsPAMBitmap(pBlob, nLen);
		if (ptrBmp->Width() && ptrBmp->Height())
			return ptrBmp;
		return 0;
	}

 	GsSimpleBitmapPtr ptrBitmap = new GsSimpleBitmap(pBlob,nLen);
	if(ptrBitmap->Width() == 0 ||ptrBitmap->Height() ==0)
		return 0;
	return ptrBitmap;
}
/// \brief 判断一个影像编码内存块的编码类型
GsImageEncodeType GsImage::EncodeType(const unsigned char* buff, int nLen)
{
	if (!buff || nLen <=0)
		return eUnknownImage;

	//如果内容的头是PAM的头。
	if (nLen > 3 && strncmp((const char*)buff, "P7\n", 3) == 0)
		return ePAM;

	ReadImageBuffer io(buff, nLen);
	return (GsImageEncodeType)io.ImageType();
}
/// \brief 解析Image/xxx的mimetype为影像类型
GsImageEncodeType GsImage::ParseImageMIMEType(const char* file)
{
	if (!file)
		return eUnknownImage;

	if (GsCRT::_stricmp(file, "image/png") == 0)
		return ePNG;
	if (GsCRT::_stricmp(file, "image/jpeg") == 0)
		return eJPG;
	if (GsCRT::_stricmp(file, "image/bmp") == 0)
		return eBMP;
	if (GsCRT::_stricmp(file, "image/dng") == 0)
		return eDNG;
	if (GsCRT::_stricmp(file, "image/gif") == 0)
		return eGIF;
	if (GsCRT::_stricmp(file, "image/tga") == 0)
		return eTGA;

	if (GsCRT::_stricmp(file, "image/ktx") == 0)
		return eKTX;
	if (GsCRT::_stricmp(file, "image/dds") == 0)
		return eDDS;
	if (GsCRT::_stricmp(file, "image/crn") == 0)
		return eCRN;
	if (GsCRT::_stricmp(file, "image/pam") == 0)
		return ePAM;

	return eUnknownImage;
}

/// \brief 根据文件扩展名解析影像编码类型
GsImageEncodeType GsImage::ParseImageType(const char* ext)
{
	if (GsStringHelp::IsNullOrEmpty(ext))
		return eUnknownImage;
	if (GsStringHelp::StartWith(ext, "."))
		ext++;

	if (GsCRT::_stricmp(ext, "png") == 0)
		return ePNG;
	if (GsCRT::_stricmp(ext, "jpeg") == 0 || GsCRT::_stricmp(ext, "jpg") == 0 || GsCRT::_stricmp(ext, "jpe") == 0)
		return eJPG;
	if (GsCRT::_stricmp(ext, "bmp") == 0)
		return eBMP;
	if (GsCRT::_stricmp(ext, "dng") == 0)
		return eDNG;
	if (GsCRT::_stricmp(ext, "gif") == 0)
		return eGIF;
	if (GsCRT::_stricmp(ext, "tga") == 0)
		return eTGA;

	if (GsCRT::_stricmp(ext, "ktx") == 0)
		return eKTX;
	if (GsCRT::_stricmp(ext, "dds") == 0)
		return eDDS;
	if (GsCRT::_stricmp(ext, "crn") == 0)
		return eCRN;
	if (GsCRT::_stricmp(ext, "pam") == 0)
		return ePAM;

	return eUnknownImage;
}


/// \brief 根据影像的编码类型获取对应影像文件扩展名
GsString GsImage::FileExtension(GsImageEncodeType eType)
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
GsString GsImage::MIMEType(GsImageEncodeType eType)
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

/// \brief 判断一个影像文件的编码类型
GsImageEncodeType GsImage::EncodeType(const char* file)
{
	if (!file)
		return GsImageEncodeType::eUnknownImage;
	GsFile f(file);
	if (!f.Exists())
		return eUnknownImage;

	ImageFile io(f.Path(), true);
	return (GsImageEncodeType)io.ImageType();
}


GsCachedImage::GsCachedImage(GsImage* primitiveImage)
{
	m_ptrPrimitiveImage = primitiveImage;
}
GsCachedImage::~GsCachedImage()
{

}

/// \brief 获取缓存的图像
GsImage* GsCachedImage::CachedImage(int nCache)
{
	GsStlMap<int, GsImagePtr>::iterator it = m_Cache.find(nCache);
	if (it == m_Cache.end())
		return 0;
	return it->second.p;
}

/// \brief 设置缓存的图像
void GsCachedImage::CachedImage(int nCache, GsImage* img)
{
	m_Cache[nCache] = img;
}


/// \brief 获取图片颜色格式
GsRGBAType GsCachedImage::RGBAType()
{
	return m_ptrPrimitiveImage ? m_ptrPrimitiveImage->RGBAType() : GsRGBAType::eBGRA32;
}



/// \brief 获得宽度
unsigned int GsCachedImage::Width()
{
	return m_ptrPrimitiveImage ? m_ptrPrimitiveImage->Width() : 0;
}

/// \brief 获得高度
unsigned int GsCachedImage::Height()
{
	return m_ptrPrimitiveImage ? m_ptrPrimitiveImage->Height() : 0;
}

/// \brief 获取图像的字节数组，并非所有图像都能够成功
const unsigned char* GsCachedImage::Bit()
{
	return m_ptrPrimitiveImage ? m_ptrPrimitiveImage->Bit() : NULL;
}

GsBrightnessImageProcess::GsBrightnessImageProcess(float scale)
{
	if (scale <= 0)
		scale = 1;
	m_Brightness = 100 * scale;
}
void ProcessPixelBrightness(unsigned char& c, float brightness)
{
	int a = 0;
	a = int(c *brightness / 100);   //调整当前点的亮度
	c = a;
	//判断范围，取得合理的值
	if (a<0) c = 0;
	if (a>255)c = 255;
}
/// \brief 处理单个像素
/// \param r 行
/// \param c 列
/// \param pixel  像素指针
/// \return 
int GsBrightnessImageProcess::Process(int r, int c, unsigned  int* pixel)
{
	if (!pixel)
		return -1;
	GsColor* color = (GsColor*)pixel;
	ProcessPixelBrightness(color->R, m_Brightness);
	ProcessPixelBrightness(color->G, m_Brightness);
	ProcessPixelBrightness(color->B, m_Brightness);
	return 0;
}
/// \brief 从RGBA原始数据直接构造
/// \param nWidth 图像的宽度
/// \param nHeight 图像的高度 
/// \param raw   原始数据  
GsSimpleBitmap::GsSimpleBitmap(unsigned int nWidth, unsigned int nHeight, const unsigned char* raw, int nBpp)
{ 
	m_nBpp = nBpp;
	if (nBpp != 32)
		return;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_Buffer.Append(raw, nWidth * nHeight * nBpp / 8);
}
/// \brief 从另外一个图像裁切局部数据并采样到自身
/// \param nWidth 图像的宽度
/// \param nHeight 图像的高度 
/// \param GsImage  其他图像
/// \param extent	要裁切的范围，如果范围超出了图像本身的范围则会根据范围裁剪
GsSimpleBitmap::GsSimpleBitmap(unsigned int nWidth, unsigned int nHeight, GsImage* img, const GsRect& extent)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	int nBpp = 32;
	m_nBpp = nBpp;
	if (nBpp != 32)
		return;

	m_Buffer.Allocate(nWidth * nHeight * (nBpp / 8));
	if (m_Buffer.BufferSize() != nWidth * nHeight * (nBpp / 8))
		return;
	if (!img)
		return;
	GsRect rectImg(0, 0, img->Width(), img->Height());
	//如果裁切范围无效的话则不干活
	if (extent.IsDisjoin(rectImg))
		return;

	//计算两个相交的范围。
	GsRect rect = rectImg.Intersects(extent);
	const unsigned char* head = img->Bit();
	head += rect.Left * 4 + rect.Top * img->Stride();
	stbir_resize_uint8(head, rect.Width(), rect.Height(), img->Stride(), m_Buffer.BufferHead(),
		m_nWidth, m_nHeight, Stride(), 4);
}

GsSimpleBitmap::GsSimpleBitmap(unsigned int nWidth, unsigned int nHeight, GsRGBAType eColorType)
{
	m_eRGBAType = eColorType;
	m_nBpp = GsImage::BytePerPixcel(eColorType) * 8;
	m_Buffer.Allocate(nWidth * nHeight * (m_nBpp / 8));
	if (m_Buffer.BufferSize() != nWidth * nHeight * (m_nBpp / 8))
		return;
	m_Buffer.SetBufferValue(0);
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}


class DecodeOneFrameGIF :public GsGIFImageDecoder
{
	GsGrowByteBuffer &m_Buffer;
public:
	DecodeOneFrameGIF(GsGrowByteBuffer &buff, const char* file):m_Buffer(buff), GsGIFImageDecoder(file)
	{

	}
	DecodeOneFrameGIF(GsGrowByteBuffer &buff, const unsigned char* blob,int n) :m_Buffer(buff), GsGIFImageDecoder(blob,n)
	{

	}

	/// \brief 开始一帧
	virtual GsColor* BeginFrame(int i, int Duration, int nTotal, int width, int height)
	{
		if (i > 0)
			return NULL;
		m_Buffer.AllocateT<GsColor>(width * height);
		return m_Buffer.PtrT<GsColor>();
	}
	/// \brief 帧数据
	virtual void FrameData(int i)
	{

	}

};
/// \brief 解码文件格式的图像
GsSimpleBitmap::GsSimpleBitmap(const char* strFile)
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	GsFile file(strFile);
	if(!file.Exists()) return;

	ImageFile io(file.Path(),true);
	GsImageEncodeType eType = io.ImageType();
	io.Rewind();
	if(eType == eUnknownImage) return;
	if(eType == ePNG)
	{
		GsFilePNG png(file.Path());
		if (!png.BeginRead()) return;
		m_nWidth = png.Width();
		m_nHeight = png.Height();
		m_Buffer.Allocate(m_nWidth * m_nHeight * 4);

		int nScanCount = png.ScanCount();
		for (int s = 0; s < nScanCount; ++s)
		{
			for (int i = 0; i < m_nHeight; i++)
			{
				png.ReadRGBA8Row(m_Buffer.Ptr() + i * m_nWidth * 4);
			}
		}

		png.FinishRead();
		m_nBpp = 32;
	}
	else if(eType == eJPG)
	{
		JpegImageDecoder decoder(&io);
		MemRGBAVisitor buff(&m_Buffer,&m_nWidth,&m_nHeight);
		decoder.ImageBuffer(&buff);
		decoder.Decode();
		m_nBpp = 32;
	}
	else if(eType == eBMP)
	{
		BmpImageDecoder decoder(&io);
		MemRGBAVisitor buff(&m_Buffer,&m_nWidth,&m_nHeight);
		decoder.ImageBuffer(&buff);
		decoder.Decode();
		m_nBpp = 32;
	}
	else if (eType == eGIF)
	{
		DecodeOneFrameGIF decoder(m_Buffer,strFile);
		if (decoder.Decode())
		{
			GsSize size = decoder.Size();
			m_nWidth = size.Width;
			m_nHeight = size.Height;
			m_nBpp = 32;
		}
	}
	else if (eType == eTGA)
	{
		STBImageDecoder decoder(&io);
		decoder.Decode(m_nWidth,m_nHeight,m_nBpp,&m_Buffer);
	}
	else if (eType >= eKTX && eType <= eCRN)
	{
		GsTextureDecoder decoder(strFile);
		decoder.Decode();

		GsSimpleBitmap* bmp = dynamic_cast<GsSimpleBitmap*>(decoder.Image());
		if(bmp)
			Swap(*bmp);
	}
	else if (eType == eTIFF)
	{
		TIFFImageDecoder decoder(new GsFileInputStream(file.FullPath().c_str(),true));
		decoder.Decode(m_nWidth, m_nHeight, m_nBpp, &m_Buffer);
	}
	else if (eType == eWEBP)
	{
		WEBPImageDecoder decoder(strFile);
		decoder.Decode(m_nWidth, m_nHeight, m_nBpp, &m_Buffer);
	}

}  
/// \brief 交换两个影像的数据
void GsSimpleBitmap::Swap(GsSimpleBitmap& bmp)
{
	std::swap(m_nWidth, bmp.m_nWidth);
	std::swap(m_nHeight, bmp.m_nHeight);
	std::swap(m_nBpp, bmp.m_nBpp);
	std::swap(m_eRGBAType, bmp.m_eRGBAType);
	m_Buffer.Swap(bmp.m_Buffer);
}

/// \brief 遍历影像中各个像素
void GsSimpleBitmap::ForEachPixel(GsImageProcess* pImageProcess, bool bParallel)
{	
	int iHeight = Height();
	int iWidth = Width();
#ifdef _OPENMP
omp_set_nested((int)bParallel);
#endif // !ANDROID
#pragma omp parallel for
	for (int r = 0; r < iHeight; r++)
	{
		unsigned char* pRow = const_cast<unsigned char*>(Row(r));
		for(int c = 0; c < iWidth; c++)
		{
			// 每4个组成一个像素颜色
			unsigned int* pColor = (unsigned int*)(&(pRow[c*4]));
			pImageProcess->Process(r, c, pColor);
		}
	}
}
  
/// \brief 解码内存中的图像
GsSimpleBitmap::GsSimpleBitmap(const unsigned char* blob,int nLen)
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	if(blob == NULL || nLen <=0) return;

	ReadImageBuffer io(blob,nLen);
	GsImageEncodeType eType = io.ImageType();
	io.Rewind();
	if(eType == eUnknownImage) return;
	if(eType == ePNG)
	{
		GsMemoryPNG png(blob,nLen);
		if (!png.BeginRead()) return;
		m_nWidth = png.Width();
		m_nHeight = png.Height();
		m_Buffer.Allocate(m_nWidth * m_nHeight * 4);
		int nScanCount = png.ScanCount();
		for (int s = 0; s < nScanCount; ++s)
		{
			for (int i = 0; i < m_nHeight; i++)
			{
				png.ReadRGBA8Row(m_Buffer.Ptr() + i * m_nWidth * 4);
			}
		}
		png.FinishRead();
		m_nBpp = 32;
	}
	else if(eType == eJPG)
	{
		JpegImageDecoder decoder(&io);
		MemRGBAVisitor buff(&m_Buffer,&m_nWidth,&m_nHeight);
		decoder.ImageBuffer(&buff);
		decoder.Decode();
		m_nBpp = 32;
	}
	else if(eType == eBMP)
	{
		BmpImageDecoder decoder(&io);
		MemRGBAVisitor buff(&m_Buffer,&m_nWidth,&m_nHeight);
		decoder.ImageBuffer(&buff);
		decoder.Decode();
		m_nBpp = 32;
	}
	else if (eType == eGIF)
	{
		DecodeOneFrameGIF decoder(m_Buffer, blob,nLen);
		if (decoder.Decode())
		{
			GsSize size = decoder.Size();
			m_nWidth = size.Width;
			m_nHeight = size.Height;
			m_nBpp = 32;
		}
	}
	else if (eType == eTGA)
	{
		STBImageDecoder decoder(&io);
		decoder.Decode(m_nWidth, m_nHeight, m_nBpp, &m_Buffer);
	}
	else if (eType >= eKTX && eType <= eCRN)
	{
		GsTextureDecoder decoder(blob,nLen);
		decoder.Decode();

		GsSimpleBitmap* bmp = dynamic_cast<GsSimpleBitmap*>(decoder.Image());
		if (bmp)
			Swap(*bmp);
	}
	else if (eType == eTIFF)
	{
		TIFFImageDecoder decoder(new GsMemoryInputStream(blob,nLen));
		decoder.Decode(m_nWidth, m_nHeight, m_nBpp, &m_Buffer);
	}
	else if (eType == eWEBP)
	{
		WEBPImageDecoder decoder(blob, nLen);
		decoder.Decode(m_nWidth, m_nHeight, m_nBpp, &m_Buffer);
	}
}


GsSimpleBitmap::~GsSimpleBitmap()
{

}

bool GsSimpleBitmap::CopyFrom( GsImage* pImage )
{
	if (0 == pImage) return false;

	const unsigned char* blob = 0;
	blob = pImage->Bit();
	if (!blob)
		return false;
	int nLen = pImage->Width() * pImage->Height() * m_nBpp/8;
	m_Buffer.Copy(pImage->Bit(), nLen);

	return true;
}

unsigned int GsSimpleBitmap::Width()
{
	return m_nWidth;
}
unsigned int GsSimpleBitmap::Height() 
{
	return m_nHeight;
}
const unsigned char* GsSimpleBitmap::Bit() 
{
	return m_Buffer.BufferHead();
}

/// \brief 图像一行的字节长度
unsigned int GsSimpleBitmap::Stride()
{
	return m_nWidth * m_nBpp / 8;
}


GsPAMBitmap::GsPAMBitmap(const char* strFile)
{
	
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 32;

	if (!strFile)
		return;

	//读写文件
	m_fStream.open(strFile,std::ios::in |std::ios::out | std::ios::binary );
	char buff[1024];
	m_fStream.getline(buff,1024);
	if(strcmp(buff,"P7") != 0)
		return;

	while(true)
	{
		m_fStream.getline(buff,1024);
		if(strcmp(buff,"ENDHDR") ==0)
			break;
		//字符串长度，太长肯定有错。
		int nLen = strlen(buff);
		if(nLen >=1024 || nLen <=0)
			break;
		
		GsString str = GeoStar::Utility::GsStringHelp::Trim(buff);
		//获取字符串
		GsVector<GeoStar::Utility::GsString> vec = GeoStar::Utility::GsStringHelp::Split(str.c_str(),' ');
		if(GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(),"WIDTH") ==0)
			m_nWidth = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str());
		else if(GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(),"HEIGHT") ==0)
			m_nHeight = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str());
		else if(GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(),"DEPTH") ==0)
		{
			m_nBpp = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str()) * 8;
			if (m_nBpp != 32)
			{
				m_nWidth = 0;
				m_nHeight = 0;
				return;
			}
		}
	}
	if(m_nWidth ==0 || m_nHeight ==0)
		return;
	 
	m_nPixelBegin = m_fStream.tellp();
}
GsPAMBitmap::GsPAMBitmap(const char* strFile,int w,int h)
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 32;

	if (!strFile)
		return;

	//新建文件
	m_fStream.open(strFile,std::ios::out| std::ios::binary);
	m_fStream.close();

	//读写文件
	m_fStream.open(strFile,std::ios::in |std::ios::out | std::ios::binary );

	m_fStream<<"P7\n";
	m_fStream<<"WIDTH "<<w<<"\n";
	m_fStream<<"HEIGHT "<<h<<"\n";
	m_fStream<<"DEPTH 4\n";
	m_fStream<<"MAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
	
	m_nPixelBegin = m_fStream.tellp();
	m_nWidth = w;
	m_nHeight = h; 

	std::vector<unsigned char> vec;
	vec.resize(Stride());
	for(int i =0;i<Height();i++)
		m_fStream.write((char*)&vec[0],vec.size());

	m_fStream.flush();
}
/// \brief 构建内存中的PAM图像
/// \param w 宽度
/// \param h  高度
/// \param bits 初始化的像素值，长度为w * h * 4;
GsPAMBitmap::GsPAMBitmap(int w, int h, const unsigned char* bits)
{
	m_nWidth = w;
	m_nHeight = h;
	m_nBpp = 32;
	std::stringstream ss;

	ss << "P7\n";
	ss << "WIDTH " << w << "\n";
	ss << "HEIGHT " << h << "\n";
	ss << "DEPTH 4\n";
	ss << "MAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";

	auto hdr = ss.str();
	m_nPixelBegin = hdr.size();

	m_Bits.Allocate(hdr.size() + m_nHeight * Stride());
	m_Bits.SetBufferValue(0);
	//存储头数据
	memcpy(m_Bits.BufferHead(), hdr.data(), hdr.size());
	//如果给了初始数据则覆盖初始数据
	if (bits)
		memcpy(m_Bits.BufferHead() + m_nPixelBegin, bits, m_nHeight * Stride());
}
/*
P7
WIDTH 227
HEIGHT 149
DEPTH 4
MAXVAL 255
TUPLTYPE RGB_ALPHA
ENDHDR
*/
//使用strchr读取一行。
const unsigned char* ReadLine(const unsigned char* hdr, char* line,int nBufferLen)
{
	auto end = (const unsigned char*)strchr((const char*)hdr, '\n');
	int len = end - hdr;
	if (len > nBufferLen)
		return NULL;
	memcpy(line, hdr, len);
	line[len] = 0;
	return end + 1;
}
/// \brief 从内存块中构造PAM位图，如果格式错误则宽度和高度为0
GsPAMBitmap::GsPAMBitmap(const unsigned char* buffer, int nLen)
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	if (!buffer || strncmp((const char*)buffer, "P7\n", 3) != 0)
		return;
	auto hdr = buffer;
	
	char line[1024];
	//最多读取7行
	int n = 7; 
	while (n-- > 0)
	{
		hdr = ReadLine(hdr, line,1024);
		if (!hdr)
			return;
		if (strcmp(line, "ENDHDR") == 0)
			break;

		GsString str = GeoStar::Utility::GsStringHelp::Trim(line);
		//获取字符串
		GsVector<GeoStar::Utility::GsString> vec = GeoStar::Utility::GsStringHelp::Split(str.c_str(), ' ');
		if (GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(), "WIDTH") == 0)
			m_nWidth = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str());
		else if (GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(), "HEIGHT") == 0)
			m_nHeight = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str());
		else if (GeoStar::Utility::GsCRT::_stricmp(vec[0].c_str(), "DEPTH") == 0)
		{
			m_nBpp = GeoStar::Utility::GsCRT::_atoi64(vec[1].c_str()) * 8;
			if (m_nBpp != 32)
			{
				m_nWidth = 0;
				m_nHeight = 0;
				return;
			}
		}
	}

	//校验获取的信息是否齐全
	if (m_nWidth <= 0 || m_nHeight < 0 || m_nBpp <= 0) {
		m_nBpp = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		return;
	}

	//剩余数据长度
	int nLeftLen = nLen  - (hdr - buffer);
	//如果剩下的长度不等于像素大小则认为数据不合规。
	if (nLeftLen != m_nHeight * Stride())
	{
		m_nBpp = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		return;
	}

	m_Bits.Append(buffer, nLen);
}
/// \brief 设置宽度
unsigned int GsPAMBitmap::Width()
{
	return m_nWidth;
}
/// \brief 设置高度
unsigned int GsPAMBitmap::Height() 
{
	return m_nHeight;
}
/// \brief 获取图像的字节数组，并非所有图像都能够成功
const unsigned char* GsPAMBitmap::Bit() 
{
	//如果是内存模式则返回地址，否则返回NULL
	if (m_Bits.BufferSize() > 0)
		return m_Bits.BufferHead() + m_nPixelBegin;
	return 0;
}
	
/// \brief 图像一行的字节长度
unsigned int GsPAMBitmap::Stride()
{
	return Width() * m_nBpp / 8;
}

/// \brief 获取某行的首指针
const unsigned char* GsPAMBitmap::Row(int r,GsVector<unsigned char> *vec)
{
	if(!vec)
		return nullptr;
	if (r < 0 || r >= Height())
		return  NULL;

	vec->resize(Stride());
	//如果是内存
	if (m_Bits.BufferSize() > 0)
	{
		auto h = Bit() + r * Stride();
		vec->insert(vec->end(), h, h + Stride());
		return vec->data();
	}
	std::fstream::pos_type nPos = m_nPixelBegin;
	nPos+= Stride() * r;
	std::fstream::pos_type nNow = m_fStream.tellg();
	if(nNow != nPos)
		m_fStream.seekg(nPos);
	m_fStream.read((char*)&vec->at(0),Stride()); 
	return &vec->at(0);
}
/// \brief 获取某行的首指针
const unsigned char* GsPAMBitmap::Row(int r)
{
	return Row(r,&m_RowData);
}

/// \brief 写入某行的数据
void GsPAMBitmap::Row(int r,const unsigned char* pRowData,int nLen)
{
	if(r <0 || r >= Height())
		return;

	//如果是内存
	if (m_Bits.BufferSize() > 0)
	{
		auto h = Bit() + r * Stride();
		memcpy((void*)h, pRowData, Stride());
		return;
	}

	std::fstream::pos_type nPos = m_nPixelBegin;
	nPos+= Stride() * r;
	std::fstream::pos_type nNow = m_fStream.tellg();
	if(nNow != nPos)
		m_fStream.seekg(nPos);
	m_fStream.write((char*)pRowData,nLen);
}

/// \brief PAM内存数据
const GsByteBuffer* GsPAMBitmap::Buffer()const
{
	if (m_Bits.BufferSize() > 0)
		return &m_Bits;
	return NULL;
}


GsTransparentImageProcess::GsTransparentImageProcess()
{
}

void GsTransparentImageProcess::PixelType(GsRGBAType eType)
{
	m_RGBAType = eType;
}

GsRGBAType GsTransparentImageProcess::PixelType()
{
	return m_RGBAType;
}

GsStlMap<unsigned int, double>& GsTransparentImageProcess::ColorTransparentMap()
{
	return m_ColorTransparentMap;
}

int GsTransparentImageProcess::Process(int r, int c, unsigned int * pixel)
{
	if (m_ColorTransparentMap.find(*pixel) == m_ColorTransparentMap.end())
	{
		return 0;
	}
	int R = 0, G = 1, B = 2, A = 3;
	bool hasA = true;
	switch (m_RGBAType)
	{
		case	GsRGBAType::eBGRA32: {R = 0; G = 1; B = 2; A = 3; } break;
		case	GsRGBAType::eABGR32: {R = 3; G = 2, B = 1, A = 0; } break;
		case	GsRGBAType::eRGBA32: {R = 2; G = 1; B = 0; A = 3; } break;
		case	GsRGBAType::eARGB32: {R = 1; G = 2; B = 3; A = 0; } break;
		case	GsRGBAType::eRGB24: { R = 2; G = 1; B = 0; hasA = false; }		break;
		case	GsRGBAType::eBGR24: { R = 0; G = 1; B = 2; hasA = false; }		break;
		default:
			break;
	}
	if (!hasA)
		return 0;
	unsigned char* apixel = (unsigned char*)pixel;
	double aa = m_ColorTransparentMap[*pixel];

	if (aa > 1. || aa < 0.)
	{
		return 0;
	}
	apixel[R] = (char)0;
	apixel[G] = (char)0;
	apixel[B] = (char)0;
	apixel[A] = (char)(aa * 255);
	return 1;
}
GsGammaImageProcess::GsGammaImageProcess(float fGamma)
{
	Gamma(fGamma);
}

float GsGammaImageProcess::Gamma()
{
	return 1 / m_fPrecompensation;
}
/// \brief  获取Gamma值
void GsGammaImageProcess::Gamma(float dblGamma)
{
	if (dblGamma <= 0)
		dblGamma = 1;
	m_fPrecompensation = 1 / dblGamma;
	int i;
	float f;
	for (i = 0; i < 256; i++)
	{
		f = (i + 0.5F) / 256;//归一化
		f = (float)pow(f, m_fPrecompensation);
		m_GammaLUT[i] = (unsigned char)(f * 256 - 0.5F);//反归一化
	}
}

int GsGammaImageProcess::Process(int r, int c, unsigned int * pixel)
{
	if (!pixel)
		return 0;
	if (GsMath::IsEqual(m_fPrecompensation, 1.0f))
		return 1;

	unsigned char* apixel = (unsigned char*)pixel;
	GsColor* color = (GsColor*)pixel;
	color->R = m_GammaLUT[color->R];
	color->G = m_GammaLUT[color->G];
	color->B = m_GammaLUT[color->B];
	return 1;
}

#define WEBPENCODER_IMP ((WEBPImageEncoder*)m_WEBPEncoder)

GsWEBPImageEncoder::GsWEBPImageEncoder(GsByteBuffer* bytebuff)
{
	m_ImageIO = new ImageBuffer(bytebuff);
	m_WEBPEncoder = new WEBPImageEncoder((ImageIO *)m_ImageIO);
}

GsWEBPImageEncoder::GsWEBPImageEncoder(const char* file)
{
	m_ImageIO = new ImageFile(file);
	m_WEBPEncoder = new WEBPImageEncoder((ImageIO*)m_ImageIO);
}

GsWEBPImageEncoder::~GsWEBPImageEncoder()
{
	if (m_ImageIO)
		delete ((ImageIO*)m_ImageIO);
	if (m_WEBPEncoder)
		delete WEBPENCODER_IMP;
}

void GsWEBPImageEncoder::Quality(int nQuality)
{
	WEBPENCODER_IMP->Quality(nQuality);
}

void GsWEBPImageEncoder::Speed(int speed)
{
	WEBPENCODER_IMP->Speed(speed);
}

bool GsWEBPImageEncoder::BeginEncode()
{
	return m_WEBPEncoder != nullptr;
}

bool GsWEBPImageEncoder::Write(GsImage* img)
{
	if (!img)
		return false;

	WEBPENCODER_IMP->RGBAType(img->RGBAType());

	int stride = img->Stride();
	WEBPENCODER_IMP->Begin(img->Width(), img->Height(), stride, img->BytePerPixcel(img->RGBAType()) * 8);
	for (int i = 0; i < img->Height(); i++)
		WEBPENCODER_IMP->WriteRow(img->Row(i), stride, i);
	return true;
}

bool GsWEBPImageEncoder::FinishEncode()
{
	WEBPENCODER_IMP->End();
	return true;
}

GsPNGImageEncoder::GsPNGImageEncoder(GsByteBuffer* buff)
{
	m_ByteBuffer = buff;
}

GsPNGImageEncoder::GsPNGImageEncoder(const char* file)
{
	m_strFilePath = file;
}

GsPNGImageEncoder::~GsPNGImageEncoder()
{
}

bool GsPNGImageEncoder::BeginEncode()
{
	return m_ByteBuffer || !m_strFilePath.empty();
}

bool GsPNGImageEncoder::Write(GsImage* img)
{
	if (!img)
		return false;
	GsPNG* png = nullptr;

	if (m_ByteBuffer)
	{
		png = new GsMemoryPNG(m_ByteBuffer,img->Width(), img->Height(), RGBAType2PNGColor(img->RGBAType()));
	}
	else if (!m_strFilePath.empty())
	{
		png = new GsFilePNG(GsFile(m_strFilePath).Path(), img->Width(), img->Height(), RGBAType2PNGColor(img->RGBAType()));
	}
	if (!png)
		return false;
	if (!png->BeginSave())
	{
		delete png;
		return false;
	}
	for (int i = 0; i < img->Height(); i++)
	{
		const unsigned char* pRow = img->Row(i);
		if (pRow)
			png->SaveRow(pRow);
	}
	png->FinishSave();

	delete png;
	return true;
}

bool GsPNGImageEncoder::FinishEncode()
{
	return true;
}

#define JPEGENCODER_IMP ((JpegImageEncoder*)m_JpegEncoder)
GsJpegImageEncoder::GsJpegImageEncoder(GsByteBuffer* buff)
{
	m_ImageIO = new ImageBuffer(buff);
	m_JpegEncoder = new JpegImageEncoder((ImageIO*)m_ImageIO);
}

GsJpegImageEncoder::GsJpegImageEncoder(const char* file)
{
	m_ImageIO = new ImageFile(file);
	m_JpegEncoder = new JpegImageEncoder((ImageIO*)m_ImageIO);
}

GsJpegImageEncoder::~GsJpegImageEncoder()
{
	if (m_ImageIO)
		delete ((ImageIO*)m_ImageIO);
	if (m_JpegEncoder)
		delete JPEGENCODER_IMP;
}

void GsJpegImageEncoder::Quality(int nQuality)
{
	JPEGENCODER_IMP->Quality(nQuality);
}

bool GsJpegImageEncoder::BeginEncode()
{
	return m_JpegEncoder == nullptr;
}

bool GsJpegImageEncoder::Write(GsImage* img)
{
	if (!img)
		return false;

	auto stride = img->Stride();

	JPEGENCODER_IMP->Begin(img->Width(), img->Height(), stride, GsImage::BytePerPixcel(img->RGBAType()) * 8);
	for (int i = 0; i < img->Height(); i++)
		JPEGENCODER_IMP->WriteRow(img->Row(i), stride);
	JPEGENCODER_IMP->End();

	return true;
}

bool GsJpegImageEncoder::FinishEncode()
{
	JPEGENCODER_IMP->End();
	return true;
}





GsTypicalColorImageProcess::GsTypicalColorImageProcess(GsTypicalColorMode mode) :m_mode(mode)
{
}


GsColor GsTypicalColorImageProcess::Color() const
{
	GsColor color = GsColor::Black;
	auto sumr = 0, sumg = 0, sumb = 0;

	for (auto a : m_RedArr)
	{
		sumr += a;
	}
	for (auto a : m_GreenArr)
	{
		sumg += a;
	}

	for (auto a : m_BlueArr)
	{
		sumb += a;
	}
	
	
	unsigned char r = std::max_element(m_RedArr+1, m_RedArr + 256) - m_RedArr -1;
	unsigned char g = std::max_element(m_GreenArr+1, m_GreenArr + 256) - m_GreenArr-1;
	unsigned char b = std::max_element(m_BlueArr+1, m_BlueArr + 256) - m_BlueArr-1;

	if (m_mode == GsTypicalColorMode::eModeRGB)
		return GsColor(r, g, b);

	if (m_mode == GsTypicalColorMode::eModeHSV)
	{
		float h, s, v;
		h = r * 2.0;
		s = g / 255.0;
		v = b / 255.0;

		return color.FromHSV(h, s, v);
	}
	
	return color;
}

int GsTypicalColorImageProcess::Process(int r, int c, unsigned int* pixel)
{

	GsColor color = *(GsColor*)pixel;
	

	if (m_mode == GsTypicalColorMode::eModeHSV)
	{
		float h, s, v;
		color.ToHSV(&h, &s, &v);
		//opencv中的 H分量是 0~180， S分量是0~255， V分量是0~255
		//可是HSV颜色空间却规定的是。H范围0~360。S范围0~1。V范围0~1
		//内核中hsv空间
		unsigned char cH, cS, cV;

		cH = h / 2;
		cS = s * 255;
		cV = v * 255;

		color.SetARGB(cH, cS, cV, color.A);
	}
	
	m_RedArr[color.R]++;
	m_GreenArr[color.G]++;
	m_BlueArr[color.B]++;

	return 0;
}


UTILITY_ENDNS
