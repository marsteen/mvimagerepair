#ifndef CHoleList_HPP
#define CHoleList_HPP


//---------------------------------------------------------------------------
//
// Klasse:    CHoleList
// Methode:   Search
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
CHole<Tdepth>* CHoleList<Tdepth>::Search(int y)
{
    for (CHole<Tdepth>* hl = CList<CHole<Tdepth> >::GetFirst();
        hl != NULL;
        hl = CList<CHole<Tdepth> >::GetNext())
    {
        if (hl->mYval == y)
        {
            return hl;
        }
    }
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CHoleList
// Methode:   Search
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tdepth>
CHole<Tdepth>* CHoleList<Tdepth>::Search2(int Left, int Right, int y)
{
    CHole<Tdepth>* Hole = NULL;
    int diff = Right - Left;

    if (diff == 1)
    {
        if (mPtrList[Left]->mYval == y)
        {
            Hole = mPtrList[Left];
        }
        else
        if (mPtrList[Right]->mYval == y)
        {
            Hole = mPtrList[Right];
        }
    }
    else
    if (diff == 0)
    {
        if (mPtrList[Left]->mYval == y)
        {
            Hole = mPtrList[Left];
        }
    }
    else
    {
        int m = (Right + Left) / 2;
        int yp = mPtrList[m]->mYval;

        if (y > yp)
        {
            Hole = Search2(m, Right, y);
        }
        else
        if (y < yp)
        {
            Hole = Search2(Left, m, y);
        }
        else
        {
            Hole = mPtrList[m];
        }
    }
    return Hole;
}


template<typename Tdepth>
CHole<Tdepth>* CHoleList<Tdepth>::Search2(int y)
{
    return Search2(0, mCount-1, y);
}


#endif
