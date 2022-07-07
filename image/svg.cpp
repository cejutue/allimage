#include "pch.h"
#include  <image.h>
#include  <filesystem.h>
#include  <tinyxml2.h>

extern "C" {
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"
}
UTILITY_NS
#define SVGIMG  ((NSVGimage*)(m_pSVG))

/// \brief 根据SVG文件或者字符串构造位图
GsSVGImage::GsSVGImage(const char* svg,bool bFileName)
{
	m_pSVG = NULL;
	m_Scale = 1;
	m_DPI = 96;
	SVG(svg,bFileName);
}
GsSVGImage::GsSVGImage()
{
	m_DPI = 96;
	m_pSVG = NULL;
	m_Scale = 1;
}
GsSVGImage::~GsSVGImage()
{
	DestroyHandle();
}
void GsSVGImage::DestroyHandle()
{
	if(m_pSVG)
		nsvgDelete(SVGIMG);
	m_pSVG = NULL;
	m_ptrImage.Release();
}

void replaceElemParams(tinyxml2::XMLElement* pElem,const GsColor &fill, const GsColor &stroke, double strokeWidth)
{
	if (!pElem)
	{
		return;
	}

	// 获取节点属性第一个
	const 	tinyxml2::XMLAttribute* attribute = pElem->FirstAttribute();
	//遍历整个属性列表
	while (attribute)
	{
		//cout << "attr=" << attribute->Name() << ":" << attribute->Value() << ",";
		//e.g. style="fill:param(fill);param(stroke)"
		
		if (GsStringHelp::Compare(attribute->Name(), "style") == 0)
		{
			//entries separated by ';'
			GsString newAttributeString;

			GsVector<GsString>  entryList = GsStringHelp::Split(attribute->Value(),';');
			GsVector<GsString>::const_iterator entryIt = entryList.cbegin();
			for (; entryIt != entryList.cend(); ++entryIt)
			{
				
				GsVector<GsString> keyValueSplit =  GsStringHelp::Split(entryIt->data(),':');
				if (keyValueSplit.size() < 2)
				{
					continue;
				}
				GsString key = keyValueSplit.at(0);
				GsString value = keyValueSplit.at(1);
				
				if (GsStringHelp::StartWith(value.c_str(),"param(fill)"))
				{
					value = fill.ToHtml();
				}
				else if (GsStringHelp::StartWith(value.c_str(), "param(fill-opacity)"))
				{
					value = GsStringHelp::ToString(fill.AlphaF());
				}
				else if (GsStringHelp::StartWith(value.c_str(), "param(outline)"))
				{
					value = stroke.ToHtml();
				}
				else if (GsStringHelp::StartWith(value.c_str(), "param(outline-opacity)"))
				{
					value = GsStringHelp::ToString(stroke.AlphaF());
				}
				else if (GsStringHelp::StartWith(value.c_str(), "param(outline-width)"))
				{
					value = GsStringHelp::ToString(strokeWidth);
				}

				if (entryIt != entryList.cbegin())
				{
					//newAttributeString.append(';');
					newAttributeString += GsString(";");
				}
				newAttributeString += key;
				//newAttributeString.append(':');
				newAttributeString += GsString(":");
				newAttributeString += value;
			}
			pElem->SetAttribute(attribute->Name(), newAttributeString.data());
			//elem.setAttribute(attribute.name(), newAttributeString);
		}
		else
		{
			const char* value = attribute->Value();
			if (GsStringHelp::StartWith(value, "param(fill)"))
			{
				pElem->SetAttribute(attribute->Name(), fill.ToHtml().data());
			}
			else if (GsStringHelp::StartWith(value, "param(fill-opacity)"))
			{
				pElem->SetAttribute(attribute->Name(), fill.AlphaF());
			}
			else if (GsStringHelp::StartWith(value, "param(outline)"))
			{
				pElem->SetAttribute(attribute->Name(), stroke.ToHtml().data());
			}
			else if (GsStringHelp::StartWith(value, "param(outline-opacity)"))
			{
				pElem->SetAttribute(attribute->Name(), stroke.AlphaF());
			}
			else if (GsStringHelp::StartWith(value, "param(outline-width)"))
			{
				pElem->SetAttribute(attribute->Name(), strokeWidth);
			}
		}
		//获取下一个属性节点
		attribute = attribute->Next();
	}
	
	tinyxml2::XMLElement* childElem = pElem->FirstChildElement();
	while (childElem)
	{
		replaceElemParams(childElem, fill, stroke, strokeWidth);
		//获取下一个属性节点
		childElem = childElem->NextSiblingElement();
	}
}
void GsSVGImage::CreateHandle()
{
	DestroyHandle();
	if(m_SVG.empty()) return;

	//原始数据copy一份，因为解析过程会导致原始数据被修改的。
	//GsString strCopy = m_SVG;
	//解析svg文件
	//m_pSVG = nsvgParse((char*)strCopy.data(), "px", m_DPI);


	//替换绘制相关参数，实现参数化绘制
	//replace fill color, stroke color, stroke with in all nodes
	tinyxml2::XMLDocument  svgDoc;
	svgDoc.Parse(m_SVG.c_str());
	tinyxml2::XMLElement* docElem = svgDoc.RootElement();
	if (!docElem)
		return;
	replaceElemParams(docElem, m_svgParams.m_fill, m_svgParams.m_stroke, m_svgParams.m_strokeWidth * m_Scale);
	tinyxml2::XMLPrinter printer;
	svgDoc.Print(&printer);
	//GsString strCopy = printer.CStr();
	//解析svg文件
	m_pSVG = nsvgParse((char*)printer.CStr(), "px", m_DPI);
}
void GsSVGImage::CreateImage()
{
	if(m_ptrImage) return;
	if(!m_pSVG) CreateHandle();
	if(!m_pSVG) return;
	int w = SVGIMG->width * m_Scale;
	int h = SVGIMG->height * m_Scale;

	Utility::GsSimpleBitmapPtr ptrBitmap = new Utility::GsSimpleBitmap(w,h);
	NSVGrasterizer *rast = nsvgCreateRasterizer();
	if(!rast) return;

	nsvgRasterize(rast, SVGIMG, 0,0,m_Scale,(unsigned char*)ptrBitmap->Bit(), w, h, w*4);

	nsvgDeleteRasterizer(rast);

	m_ptrImage = ptrBitmap.p;
}
/// \brief 获取图像显示分辨率
float GsSVGImage::DPI()
{
	return m_DPI;
}
	
/// \brief 设置图像显示分辨率
void GsSVGImage::DPI(float dpi)
{
	dpi = fabs(dpi);
	if(dpi ==0) dpi = 96;
	m_DPI = dpi;
}
	

/// \brief 设置宽度
unsigned int GsSVGImage::Width()
{
	if(!m_ptrImage) CreateImage();
	if(!m_ptrImage) return 0;
	return m_ptrImage->Width();
}
/// \brief 设置高度
unsigned int GsSVGImage::Height()
{
	if(!m_ptrImage) CreateImage();
	if(!m_ptrImage) return 0;
	return m_ptrImage->Height();
}
	
/// \brief 获取图像的字节数组，并非所有图像都能够成功
const unsigned char* GsSVGImage::Bit()
{
	if(!m_ptrImage) CreateImage();
	if(!m_ptrImage) return 0;
	return m_ptrImage->Bit();
}
/// \brief 获取SVG的xml字符串
GsString GsSVGImage::SVG()
{
	return m_SVG;
}

/// \brief 设置SVG字符串或者SVG文件
void GsSVGImage::SVG(const char* svg,bool bFileName)
{
	m_SVG =  svg; 
	if(bFileName)
		m_SVG = GsFile(svg).ReadAll();

	DestroyHandle();
}

/// \brief 是否是有效的SVG
bool GsSVGImage::IsValid()
{
	if(m_pSVG) return true;
	if(m_SVG.empty()) return false;
	CreateHandle();
	return m_pSVG?true:false;
}
/// \brief SVG图像的宽和高
GsSize GsSVGImage::SVGSize()
{
	if(!IsValid()) return GsSize(0,0);
	return GsSize(SVGIMG->width,SVGIMG->height);
}
/// \brief 获取SVG绘制时的缩放比率
float GsSVGImage::SVGScale()
{
	return m_Scale;
}

/// \brief 设置SVG绘制时的缩放比率
void GsSVGImage::SVGScale(float scale)
{
	scale = fabs(scale);
	if(scale ==0) scale =1;
	m_Scale = scale;
	m_ptrImage = 0;
}

GsSVGImage::GsSvgDrawParams& GsSVGImage::SvgDrawParams()
{
	return m_svgParams;
}

void GsSVGImage::SvgDrawParams(const GsSvgDrawParams& params)
{
	
	//如果绘制参数不同，则进行重新解析渲染处理
	if (m_svgParams.m_fill != params.m_fill || m_svgParams.m_stroke != params.m_stroke
		|| m_svgParams.m_strokeWidth != params.m_strokeWidth)
	{
		DestroyHandle();
	}
	m_svgParams = params;
}



	 
UTILITY_ENDNS