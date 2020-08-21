//***************************************************************************
//
//
// @PROJECT  :	Map Profiles
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

template <typename T>
static void ProcessFillhole(SDataRect* dr, const char* NewFile, int SmoothFaktor, int RandomFaktor)
{
	CFillHole<T> FillHole;
	FillHole.Init(dr);
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

		cout << "  Writing " << NewFile << endl;
		cout << "  w=" << dr->mWidth << " h=" << dr->mHeight << " b=" << dr->mBits << endl;

		//dr->SwapWords();
        mv_graphics_tools_swapwords(dr);
        mv_graphics_png_write(NewFile, dr->mData, dr->mWidth, dr->mHeight, dr->mBits);
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

#define WRONG_PIXEL_16BIT 30000

static int CountMaskPixels(SDataRect* glib)
{
  int m = 0;
	for (int y = 0; y < glib->mHeight; y++)
	for (int x = 0; x < glib->mWidth; x++)
	{
	  short* pix =  ((short*) glib->mData) + glib->Offset(x, y);

	  if (*pix == WRONG_PIXEL_16BIT)
	  {
	    m++;
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
					const SPixel24* mask = ((SPixel24*) mask_png.mData) + mask_png.Offset(x, y);

					short* pix =  ((short*) glib->mData) + glib->Offset(x, y);

					if ((mask->colors[0] == 0xFF) &&
							(mask->colors[1] == 0x1) &&
							(mask->colors[2] == 0xFF))  // Magenta
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
// Klasse:    Global
// Methode:   main
//
//
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{

	if (argc >= 2)
	{

		const char*  MaskFile = NULL;
		const char*  OutFile = "output-fillholes.png";
		int          AnzahlFiles;
		int          SmoothFaktor = 0;
		int          RandFaktor = 2;


   	for (int i = 2; i < argc; i++)
   	{
   	   std::string cmd = argv[i];

   	  if (cmd == "-smooth")
   	  {
   	    SmoothFaktor = mvStringtool::Cast<int>(argv[i+1]);
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

		       ProcessFillhole<unsigned char>(&dr, OutFile, SmoothFaktor, RandFaktor);
		       break;

		     case 16:

		       ProcessFillhole<short>(&dr, OutFile, SmoothFaktor, RandFaktor);
 		       break;

 		     case 24:

		       ProcessFillhole<SPixel24>(&dr, OutFile, SmoothFaktor, RandFaktor);
 		       break;

 		     default:

 		       cout << "***** ERROR Bit depth not supported:" << dr.mBits << endl;
 		       break;

			}
		}
	}
	else
	{
		cout << "usage: fillholes <file.png>"
		     << "  options:" << endl
		     << "  [-smooth n] = number of smooth stages" << endl
		     << "  [-random n] = number of randomize stages" << endl
		     << "  [-mask <maskfile.png>] = optional mask file" << endl
		     << "  [-outfile <file>] = optional output file" << endl
		     << endl
		     << "  supported files (source): 8 bit PNG, 16 bit PNG" << endl
		     << "                  (mask)  : 24 bit PNG" << endl
		     << "version 3.0 / ksti GmbH 2020" << endl;
	}

	return 0;
}
