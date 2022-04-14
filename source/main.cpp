//***************************************************************************
//
//
// @PROJECT  :	ImageRepair
// @VERSION  :	1.0
// @FILENAME :	main.cpp
// @DATE     :	28.8.2014
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <CHoleList.h>
#include <CFillHole.h>
#include <mvlib/graphics/SPixel24.h>
#include <mvlib/graphics/SPixel32.h>
#include <mvlib/graphics/SDataRect.h>
#include <mvlib/graphics/mv_graphics_tools_swapredblue.h>
#include <mvlib/graphics/mv_graphics_png.h>
#include <mvlib/graphics/SPixel24.h>
#include <mvlib/graphics/SPixel24.h>
#include <mvlib/strings/mv_stringtool.h>


using namespace std;
using namespace mvlib;


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ProcessFillhole
//
//
//---------------------------------------------------------------------------

template<typename T>
static void ProcessFillhole(SDataRect* dr, int SmoothFaktor, int RandomFaktor, T wrongp)
{
    CFillHole<T> FillHole;

    FillHole.Init(dr, wrongp);
    
    FillHole.FillY();

    if (FillHole.mHoleCount > 0)
    {
        FillHole.FillX();

        cout << "  Smoothing: "  << SmoothFaktor<< endl;
        for (int i = 0; i < SmoothFaktor; i++)
        {
            FillHole.ReplacePixels();
            FillHole.Smooth();
        }

        FillHole.ReplacePixels();

        cout << "  Randomize:" << RandomFaktor  << endl;
        FillHole.Randomize(RandomFaktor);

        //cout << "  Writing " << NewFile << endl;
        cout << "  w=" << dr->mWidth << " h=" << dr->mHeight << " b=" << dr->mBits << endl;

        //dr->SwapWords();
        mv_graphics_tools_swapwords(dr);
    }
    FillHole.Finish();
    cout << "OK." << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   CountMaskPixels
//
//
//---------------------------------------------------------------------------

#define WRONG_PIXEL_16BIT    30000

static int CountMaskPixels(const SDataRect* glib)
{
    int m = 0;

    for (int y = 0; y < glib->mHeight; y++)
    {
        for (int x = 0; x < glib->mWidth; x++)
        {
            short* pix = ((short*)glib->mData) + glib->Offset(x, y);

            if (*pix == WRONG_PIXEL_16BIT)
            {
                m++;
            }
        }
    }
    return m;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ProcessMaskFile
//
//---------------------------------------------------------------------------


static bool ProcessMaskFile(const char* MaskFile, SDataRect* glib)
{
    bool r = false;

    int MaskPixels = 0;

    SDataRect mask_png;

    if (mv_graphics_png_read(MaskFile, &mask_png.mData, &mask_png.mWidth, &mask_png.mHeight, &mask_png.mBits, 0))
    {
        cout << "  Graf: w=" << glib->mWidth << " h=" << glib->mHeight << " b=" << glib->mBits << endl;
        cout << "  Mask: w=" << mask_png.mWidth << " h=" << mask_png.mHeight << " b=" << mask_png.mBits << endl;


        if ((mask_png.mWidth == glib->mWidth) &&
            (mask_png.mHeight == glib->mHeight))
        {
            for (int y = 0; y < glib->mHeight; y++)
            {
                //cout << "Line:" << y << endl;

                for (int x = 0; x < glib->mWidth; x++)
                {
                    const SPixel24* mask = ((SPixel24*)mask_png.mData) + mask_png.Offset(x, y);

                    short* pix = ((short*)glib->mData) + glib->Offset(x, y);

                    if ((mask->colors[0] == 0xFF) &&
                        (mask->colors[1] == 0x1) &&
                        (mask->colors[2] == 0xFF))      // Magenta
                    {
                        *pix = WRONG_PIXEL_16BIT;
                        MaskPixels++;
                    }
                }
            }
            delete[] mask_png.mData;

#if 0
            CGraflibPng test;
            test.CopyDataFrom(glib);
            //test.SwapWords();
            test.Write("test.png");
#endif


            cout << "  MaskPixels:" << MaskPixels << endl;
            //CountMaskPixels(glib);
            r = true;
        }
        else
        {
            cout << "***** Groesse der Maske und der Grafik sind nicht gleich!" << endl;
        }
    }
    return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   SeparateLayers
//
//
//---------------------------------------------------------------------------

template <typename Tpixel>
static SDataRect* SeparateLayers(const SDataRect* dr, int& layers)
{
    layers = dr->mBits / 8;
    SDataRect* dr8   = new SDataRect[layers];
    Tpixel*  dr24ptr = (Tpixel*) dr->mData;
    
    cout << "layers=" << layers << endl;
    
    for (int i = 0; i < layers; i++)
    {
     //   cout << "i=" << i << endl;
        dr8[i].mWidth  = dr->mWidth;
        dr8[i].mHeight = dr->mHeight;
        dr8[i].mBits   = 8;
        dr8[i].mData   = new char[dr->mWidth * dr->mHeight];
        char* dr8ptr = dr8[i].mData;
        for (int y = 0; y < dr->mHeight; y++)
        {
            for (int x = 0; x < dr->mWidth; x++)
            {
                unsigned int Offset = dr8->Offset(x, y);
                dr8ptr[Offset] = dr24ptr[Offset].colors[i];
            }
        }
        /*
        char filename[256];
        sprintf(filename, "output-layer-%d.png", i);
        mv_graphics_png_write(filename, dr8[i].mData, dr8[i].mWidth, dr8[i].mHeight, dr8[i].mBits);
        */
    }
     
    
    
    return dr8;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   JoinLayers
//
//
//---------------------------------------------------------------------------

static SDataRect* JoinLayers(const SDataRect* dr8, int layers)
{
    SDataRect* dr24 = new SDataRect; 
    dr24->mWidth  = dr8->mWidth;
    dr24->mHeight = dr8->mHeight;
    dr24->mBits = layers * 8;
    
    dr24->mData = new char[dr24->mWidth * dr24->mHeight * layers];
    SPixel24* dr24ptr = (SPixel24*) dr24->mData; 

    cout << "JoinLayers layers=" << layers << endl;
        
    for (int i = 0; i < layers; i++)
    {
        cout << "  i=" << i << endl;
        char* dr8ptr = dr8[i].mData;
        for (int y = 0; y < dr8->mHeight; y++)
        {
            for (int x = 0; x < dr8->mWidth; x++)
            {
                unsigned int Offset = dr8->Offset(x, y);
                dr24ptr[Offset].colors[i] = dr8ptr[Offset];
            }
        }
    }
    return dr24;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   FillHoles24
//
//
//---------------------------------------------------------------------------

SDataRect* FillHoles24(const SDataRect* dr24, int SmoothFaktor, int RandFaktor)
{
    int layers;
    int outLayers = 3;
    SDataRect* dr8;

    if (dr24->mBits == 24)
    {
        dr8 = SeparateLayers<SPixel24>(dr24, layers);
    }
    else
    if (dr24->mBits == 32)
    {
        dr8 = SeparateLayers<SPixel32>(dr24, layers);
    }
   
    
    cout << "SeparateLayers OK layers=" << layers << endl;
        
    for (int i = 0; i < outLayers; i++)
    {
       ProcessFillhole<unsigned char>(dr8 + i, SmoothFaktor, RandFaktor, 0);
    }
    cout << "ProcessFillhole OK" << endl;
    
    SDataRect* dr24new = JoinLayers(dr8, outLayers);
    
    cout << "JoinLayers OK" << endl;
    
    
    
    for (int i = 0; i < layers; i++)
    {
        delete dr8[i].mData;
    }
    delete dr8;
    return dr24new;
    
}






bool mapHasVal(const std::map<short, int>& vmap, short value)
{
    return vmap.find(value) != vmap.end();
}
//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   FindValues
//
//
//---------------------------------------------------------------------------


struct SValpair
{
  bool operator<(const SValpair& k) const
  {
      return count < k.count;
  }    
  short value;
  unsigned int count;
  
};


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   FindValues
//
//
//---------------------------------------------------------------------------

static unsigned int FindValues(const char* filename, int vmin, int vmax)
{
    std::map<short, int> vmap;
    
    unsigned int count = 0;
    SDataRect dr;
    if (mv_graphics_png_read(filename, &dr.mData, &dr.mWidth, &dr.mHeight, &dr.mBits, 0))
    {
        mv_graphics_tools_swapwords(&dr);
        for (int y = 0; y < dr.mHeight; y++)
        {
            for (int x = 0; x < dr.mWidth; x++)
            {
                unsigned int offset = y * dr.mWidth + x;
                const short* data16 = ((const short*) dr.mData) + offset;
                if ((*data16 >= vmin) && (*data16 <= vmax))
                {
                    if (vmap.find(*data16) != vmap.end())
                    {
                        vmap[*data16] = vmap[*data16] + 1;
                    }
                    else
                    {
                        vmap[*data16] = 1;
                    }
                    count++;
                }                
            }
        }
    }
    std::set<SValpair> vset;
    for (auto it = vmap.begin(); it != vmap.end(); ++it)
    {
        SValpair vp;
        vp.value = it->first;
        vp.count = it->second;
        vset.insert(vp);
    }
    for (auto it = vset.begin(); it != vset.end(); ++it)
    {
       cout << "value=" << it->value << " count=" << it->count << endl;   
    }
    
    return count;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ReplaceValues
//
//
//---------------------------------------------------------------------------

static unsigned int ReplaceValues(const char* filename, int vmin, int vmax, int vrepl)
{
    unsigned int count = 0;
    SDataRect dr;
    if (mv_graphics_png_read(filename, &dr.mData, &dr.mWidth, &dr.mHeight, &dr.mBits, 0))
    {
        mv_graphics_tools_swapwords(&dr);
        for (int y = 0; y < dr.mHeight; y++)
        {
            for (int x = 0; x < dr.mWidth; x++)
            {
                unsigned int offset = y * dr.mWidth + x;
                short* data16 = ((short*) dr.mData) + offset;
                if ((*data16 >= vmin) && (*data16 <= vmax))
                {
                    *data16 = vrepl;
                    count++;
                }
            }
        }                    
    }
    
    if (count >  0)
    {
        cout << "writing output.png.." << endl;
        mv_graphics_tools_swapwords(&dr);
        mv_graphics_png_write("ouput.png", dr.mData, dr.mWidth, dr.mHeight, dr.mBits);   
    }    
    return count;
}

//---------------------------------------------------------------------------
//
// Klasse:    Global
// Methode:   main
//
//
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc >= 2)
    {
        const char* MaskFile = NULL;
        const char* OutFile = "output-fillholes.png";
        int AnzahlFiles;
        int SmoothFaktor = 0;
        int RandFaktor = 1;
        int repvalue = 0;

        for (int i = 2; i < argc; i++)
        {
            std::string cmd = argv[i];

            if (cmd == "-smooth")
            {
                SmoothFaktor = mvStringtool::Cast<int>(argv[i+1]);
            }
            if (cmd == "-findvalues")
            {
                int vmin = mvStringtool::Cast<int>(argv[i+1]);
                int vmax = mvStringtool::Cast<int>(argv[i+2]);
                int values = FindValues(argv[1], vmin, vmax);
                cout << "values found:" << values << endl;
                return 0;
            }            
            else
            if (cmd == "-replacevalues")
            {
                int vmin  = mvStringtool::Cast<int>(argv[i+1]);
                int vmax  = mvStringtool::Cast<int>(argv[i+2]);
                int vrepl = mvStringtool::Cast<int>(argv[i+3]);
                int values = ReplaceValues(argv[1], vmin, vmax, vrepl);
                cout << "values replaced:" << values << endl;
                return 0;
            }                        
            else
            if (cmd == "-random")
            {
                RandFaktor = mvStringtool::Cast<int>(argv[i+1]);
            }
            else
            if (cmd == "-mask")
            {
                MaskFile = argv[i+1];
                cout << "  mask file:" << MaskFile << endl;
                i += 1;
            }
            else
            if (cmd == "-repvalue")
            {
                repvalue = mvStringtool::Cast<int>(argv[i+1]);                
                i += 1;
            }            
            else
            if (cmd == "-outfile")
            {
                OutFile = argv[i+1];
                cout << "  output file:" << OutFile << endl;
                i += 1;
            }
        }

        SDataRect dr;
        if (mv_graphics_png_read(argv[1], &dr.mData, &dr.mWidth, &dr.mHeight, &dr.mBits, 0))
        {
            cout << "File:" << argv[1]  << endl;

            if (MaskFile != NULL)
            {
                ProcessMaskFile(MaskFile, &dr);
            }


            switch (dr.mBits)
            {
                case 8:

                    cout << "8 bit, repvalue=" << repvalue << endl;
                    ProcessFillhole<unsigned char>(&dr, SmoothFaktor, RandFaktor, repvalue);
                    break;

                case 16:

                    cout << "16 bit, repvalue=" << repvalue << endl;
                    mv_graphics_tools_swapwords(&dr);
                    ProcessFillhole<short>(&dr, SmoothFaktor, RandFaktor, repvalue);                    
                    break;
                    
                case 24:
                case 32:
                {
                    SDataRect* drnew =FillHoles24(&dr, SmoothFaktor, RandFaktor);
                    delete[] dr.mData;
                    dr.mData = drnew->mData;
                    dr.mBits = drnew->mBits;
                    delete drnew;       

                    mv_graphics_tools_swapredblue(&dr);
                    
                    cout << "result: " << endl;
                    cout << "  width :" << dr.mWidth << endl;
                    cout << "  height:" << dr.mHeight << endl;
                    cout << "  bits  :" << dr.mBits << endl;
                }
                break;

                default:
     
                  cout << "***** ERROR Bit depth not supported:" << dr.mBits << endl;
                  break;
     
            }
            cout << "writing file " << OutFile << endl;
            mv_graphics_png_write(OutFile, dr.mData, dr.mWidth, dr.mHeight, dr.mBits);

        }
    }
    else
    {
        cout	<< "usage: mvimagerepair <file.png>"
                << "  options:" << endl
                << "  [-smooth n] = number of smooth stages" << endl
                << "  [-random n] = number of randomize stages" << endl
                << "  [-repvalue n] = replace value (only for 8 and 16 bit)" << endl
                << "  [-outfile <file>] = optional output file" << endl
                << "  supported files: 8, 16, 24 or 32 bit PNG" << endl
                << endl
                << "  *** 24/32 bit: mark the errors in the picture with BLACK color (#000000)" 
                << "  *** and run the tool" << endl
                << endl
                << "version 5.0" << endl;
    }

    return 0;
}
