// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "aimage.h"

#ifdef _WIN32  
#ifdef _DEBUG
#pragma comment(lib,"allimaged.lib")
#else
#pragma comment(lib,"allimage.lib")
#endif // _DEBUG
#endif // WIN32


int main()
{

    AImage* imgpng = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\1.jpg");
    imgpng->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\1.ktx2", AImageEncodeType::eKTX2);
    //AImage* imgpng2 = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\1.ktx2");
    //imgpng2->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\1aaa.png", AImageEncodeType::ePNG);
        //AImage* imgpng =  AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.png");
        //imgpng->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.ktx2", AImageEncodeType::eKTX2);

        //AImage* imgpng2 = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.png");
        //imgpng2->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001.ktx2", AImageEncodeType::eKTX2);
 /*   AImage* imgpng =  AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001.png");
    AImage* imgtga = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001.tga");
    AImage* imgjpg = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\world.jpg");
    AImage* imggif = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\transgif.gif");
    AImage* imgktx = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.ktx");
    AImage* imgdds = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.dds");
    AImage* imgcrn = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktx.crn");
    AImage* imgktx2 = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\0318_img0.ktx2");
    AImage* imgwebp = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\baby_tux.webp"); 
    AImage* imgtiff = AImage::LoadFrom("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\quad-lzw-compat.tiff");
   
    imgpng->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.png", AImageEncodeType::ePNG);
    imgtga->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\z001a.tga", AImageEncodeType::eTGA);
    imgjpg->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\worlda.jpg", AImageEncodeType::eJPG);
    imggif->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\transgifa.gif", AImageEncodeType::eGIF);
    imgktx->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.ktx", AImageEncodeType::eKTX);
    imgdds->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.dds", AImageEncodeType::eDDS);
    imgcrn->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\kernel_ktxa.crn", AImageEncodeType::eCRN);
    imgktx2->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\0318_img0a.ktx2", AImageEncodeType::eKTX2);
    
    imgwebp->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\baby_tuxa.png", AImageEncodeType::ePNG);
    imgwebp->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\baby_tuxa.webp", AImageEncodeType::eWEBP);
    imgtiff->Save("D:\\opensource\\cjimage\\Debugx64\\imgtestdata\\quad-lzw-compata.tiff", AImageEncodeType::eTIFF);*/
    //AGrowByteBuffer buff;
    //imgpng->Save(&buff, AImageEncodeType::ePNG);
    //imgtga->Save(&buff, AImageEncodeType::eTGA);
    //imgjpg->Save(&buff, AImageEncodeType::eJPG);
    //imggif->Save(&buff, AImageEncodeType::eGIF);
    //imgktx->Save(&buff, AImageEncodeType::eKTX);
    //imgdds->Save(&buff, AImageEncodeType::eDDS);
    //imgcrn->Save(&buff, AImageEncodeType::eCRN);
    //imgktx2->Save(&buff, AImageEncodeType::eKTX2);
    //imgwebp->Save(&buff, AImageEncodeType::eWEBP);
    //imgtiff->Save(&buff, AImageEncodeType::eTIFF);

}

