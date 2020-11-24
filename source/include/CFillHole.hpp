#ifndef CFillHole_HPP
#define CFillHole_HPP


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   CreatePtrLists
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::CreatePtrLists()
{
    for (int x = 0; x < mSource->mWidth; x++)
    {
        mHoleList[x].mPtrList = mHoleList[x].CreatePtrList();
        mHoleList[x].mCount = mHoleList[x].Count();
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CHoleList
// Methode:   Finish
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::Finish()
{
    //std::cout << "Finish start" << std::endl;
    for (int x = 0; x < mSource->mWidth; x++)
    {
        mHoleList[x].ClearList();
        delete[] mHoleList[x].mPtrList;
    }
    delete[] mHoleList;
    //std::cout << "Finish ok" << std::endl;
}


/*
 * CHole<Tdepth>* CFillHole<Tdepth>::GetHole(CHoleList<Tdepth>* HoleList, int x, int y)
 * {
 *  return HoleList[x].Search(y);
 * }
 */

//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   FillX
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::FillX()
{
    Tdepth* SrcData = (Tdepth*)mSource->mData;
    int Status = 0;
    CHole<Tdepth>* BeginHole = NULL;
    int LastGoodX = -1;
    int FirstWrongX;

    for (int y = 0; y < mSource->mHeight; y++)
    {
        Status = 0;
        BeginHole = NULL;
        LastGoodX = -1;

        //cout << "FillX y=" << y << endl;

        for (int x = 0; x < mSource->mWidth; x++)
        {
            Tdepth* Pixel = SrcData + mSource->Offset(x, y);
            switch (Status)
            {
                case 0: //

                    if (*Pixel == mWrongPixel)
                    {
                        //std::cout << "Search START" << std::endl;
                        BeginHole = mHoleList[x].Search2(y);
                        FirstWrongX = x;
                        BeginHole->mXcount = 1;

                        if (BeginHole == NULL)
                        {
                            std::cout << "***** Hole not found 1: x=" << x << " y=" << y << std::endl;
                            exit(0);
                        }
                        //else cout << "Hole x=" << x << " y=" << y << endl;


                        //cout << "Search OK" << endl;
                        Status = 1;
                    }
                    else
                    {
                        LastGoodX = x;
                    }
                    break;

                case 1:

                    if (*Pixel == mWrongPixel)
                    {
                        CHole<Tdepth>* hole = mHoleList[x].Search2(y);

                        if (hole == NULL)
                        {
                            std::cout << "***** Hole not found 2: x=" << x << " y=" << y << std::endl;
                            exit(0);
                        }
                        else
                        {
                            BeginHole->mXcount++;
                            hole->mXcount = BeginHole->mXcount;
                        }
                    }
                    else
                    {
                        if (LastGoodX == -1)
                        {
                            //FillPixelValue(BeginHole, *Pixel);
                        }
                        else
                        {
                            Tdepth FromValue = *(SrcData + mSource->Offset(LastGoodX, y));
                            Tdepth ToValue = *Pixel;
                            int DiffX = x - LastGoodX - 1;



                            float AddVal = (float)(ToValue - FromValue) / DiffX;
                            float StartVal = FromValue + AddVal;

                            /*
                             *            cout << "DiffX=" << DiffX
                             *                 << " Startval=" << StartVal
                             *                 << " Xcount=" << BeginHole->mXcount
                             *                 << " FirstWrongX=" << FirstWrongX << endl;
                             */

                            int IndexX = FirstWrongX;

                            for (int i = 0; i < DiffX; i++)
                            {
                                CHole<Tdepth>* hole = mHoleList[IndexX++].Search2(y);

                                int pcnt = hole->mXcount + hole->mYcount;

                                float RatioX = (float)hole->mXcount / pcnt;
                                float RatioY = (float)hole->mYcount / pcnt;

                                if (hole->mPixel != mWrongPixel)
                                {
                                    //cout << "!!!!! hole->mPixel="<< (int) hole->mPixel << " i=" << i << " Ix=" << IndexX << endl;
                                    hole->mPixel = (hole->mPixel * RatioX) + (StartVal * RatioY);
                                }
                                else
                                {
                                    hole->mPixel = StartVal;
                                }
                                StartVal += AddVal;

                                if (hole == NULL)
                                {
                                    std::cout << "Hole not found 3: x=" << IndexX << " y=" << BeginHole->mYval << std::endl;
                                    std::cout << "i=" << i << " DiffX=" << DiffX << std::endl;
                                    std::cout << "IndexX=" << IndexX << std::endl;
                                    std::cout << "mHoleList[IndexX].Count()=" << mHoleList[IndexX].Count() << std::endl;
                                    exit(0);
                                }
                            }
                        }
                        Status = 0;
                        LastGoodX = x;
                    }
                    break;
            }
        }
        if (Status == 1)
        {
            //Tdepth FillValue = *(SrcData + mSource->Offset(LastGoodX, y));
            //FillPixelValue(BeginHole, FillValue);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   Init
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::Init(mvlib::SDataRect* Source, Tdepth& wrongp)
{
    mSource = Source;
    mHoleList = new CHoleList<Tdepth>[Source->mWidth];
    mHoleCount = 0;

    mWrongPixel = wrongp;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   Smooth
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::Smooth()
{
    Tdepth* SrcData = (Tdepth*)mSource->mData;

    for (int x = 0; x < mSource->mWidth; x++)
    {
        for (CHole<Tdepth>* hl = mHoleList[x].GetFirst();
            hl != NULL;
            hl = mHoleList[x].GetNext())
        {
            int PixelSumme = 0;
            int pc = 0;

            for (int y1 = -1; y1 <= 1; y1++)
            {
                int yk = hl->mYval + y1;
                if ((yk >= 0) && (yk < mSource->mHeight))
                {
                    for (int x1 = -1; x1 <= 1; x1++)
                    {
                        int xk = x + x1;
                        if ((xk >= 0) && (xk < mSource->mWidth))
                        {
                            PixelSumme += SrcData[mSource->Offset(xk, yk)];
                            pc += 1;
                        }
                    }
                }
            }
            if (pc > 0)
            {
                hl->mPixel = PixelSumme / pc;
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   Randomize
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::Randomize(int k)
{
    int k2 = k * 2;
    Tdepth* SrcData = (Tdepth*)mSource->mData;



    for (int x = 0; x < mSource->mWidth; x++)
    {
        for (CHole<Tdepth>* hl = mHoleList[x].GetFirst();
            hl != NULL;
            hl = mHoleList[x].GetNext())
        {
            Tdepth* Pixel = SrcData + mSource->Offset(x, hl->mYval);
            *Pixel += (rand() % k2) - k;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   FillPixelValue
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::FillPixelValue(CHole<Tdepth>* BeginHole, Tdepth val)
{
    for (CHole<Tdepth>* hole = BeginHole;
        hole != NULL;
        hole = hole->next)
    {
        hole->mPixel = val;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   FillY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::FillY()
{
    Tdepth* SrcData = (Tdepth*) mSource->mData;

    int Status = 0;
    CHole<Tdepth>* BeginHole = NULL;
    int LastGoodY = -1;

    //std::cout << "FillY()" << std::endl;

    for (int x = 0; x < mSource->mWidth; x++)
    {
        Status = 0;
        BeginHole = NULL;
        LastGoodY = -1;

        for (int y = 0; y < mSource->mHeight; y++)
        {
            //std::cout << "x=" << x << " y=" << y << std::endl;
            ///std::cout << "BeginHole=" << (long) BeginHole << std::endl;

            Tdepth* Pixel = SrcData + mSource->Offset(x, y);


            //std::cout << "p=" << *Pixel << std::endl;

            switch (Status)
            {
                case 0: //

                    if (*Pixel == mWrongPixel)
                    {
                        CHole<Tdepth>* Hole = mHoleList[x].NewListElem();

                        if (Hole == NULL)
                        {
                            std::cout << "***** mem ERROR!!" << std::endl;
                            exit(0);
                        }

                        Hole->mYval = y;
                        Hole->mYcount = 1;
                        Hole->mPixel = mWrongPixel;
                        BeginHole = Hole;
                        mHoleCount++;
                        Status = 1;
                    }
                    else
                    {
                        LastGoodY = y;
                    }
                    break;

                case 1:

                    if (*Pixel == mWrongPixel)
                    {
                        CHole<Tdepth>* Hole = mHoleList[x].NewListElem();
                        Hole->mYval = y;
                        Hole->mPixel = mWrongPixel;
                        BeginHole->mYcount++;
                        Hole->mYcount = BeginHole->mYcount;
                        mHoleCount++;
                    }
                    else
                    {
                        if (LastGoodY == -1)
                        {
                            //FillPixelValue(BeginHole, *Pixel);
                        }
                        else
                        {
                            //cout << "!!!???" << endl;
                            //exit(0);

                            Tdepth FromValue = *(SrcData + mSource->Offset(x, LastGoodY));
                            Tdepth ToValue = *Pixel;
                            int DiffY = y - LastGoodY - 1;

                            //cout << "DiffY=" << DiffY << endl;

                            float AddVal = (float)(ToValue - FromValue) / DiffY;
                            CHole<Tdepth>* hole = BeginHole;
                            float StartVal = FromValue + AddVal;


                            for (CHole<Tdepth>* hole = BeginHole;
                                hole != NULL;
                                hole = hole->next)
                            {
                                hole->mPixel = StartVal;
                                StartVal += AddVal;
                            }
                            //cout << "OK" << endl;
                        }
                        Status = 0;
                        LastGoodY = y;
                    }
                    break;
            }
        } // for y

        if (Status == 1)
        {
            //Tdepth FillValue = *(SrcData + mSource->Offset(x, LastGoodY));
            //FillPixelValue(BeginHole, FillValue);
        }
    }

    CreatePtrLists();
    //memset(SrcData, 0, sizeof(Tdepth) * Source->mWidth * Source->mHeight);

    std::cout << "  FillY ok Holes=" << mHoleCount << std::endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
// Methode:   ReplacePixels
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
void CFillHole<Tdepth>::ReplacePixels()
{
    Tdepth* SrcData = (Tdepth*)mSource->mData;
    Tdepth NotPixel;

    if (sizeof(Tdepth) == 2)
    {
        NotPixel = 25;
    }
    else
    {
        NotPixel = 0x7F;
    }
    for (int x = 0; x < mSource->mWidth; x++)
    {
        for (CHole<Tdepth>* hl = mHoleList[x].GetFirst();
            hl != NULL;
            hl = mHoleList[x].GetNext())
        {
            Tdepth* Pixel = SrcData + mSource->Offset(x, hl->mYval);

            if (hl->mPixel == mWrongPixel)
            {
                *Pixel = NotPixel;
            }
            else
            {
                *Pixel = hl->mPixel;
            }
        }
    }
}


#endif
