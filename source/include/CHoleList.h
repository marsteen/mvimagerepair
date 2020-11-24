#ifndef CHoleList_H
#define CHoleList_H

#include <CHole.h>

//---------------------------------------------------------------------------
//
// Klasse:    CHoleList
//
//---------------------------------------------------------------------------

template<typename Tdepth>
class CHoleList : public CList<CHole<Tdepth> >
{
    public:

        CHoleList()
        {
            mPtrList = NULL;
        }


        CHole<Tdepth>*  Search(int y);
        CHole<Tdepth>*  Search2(int y);

        CHole<Tdepth>** mPtrList;
        int mCount;

    protected:

        CHole<Tdepth>* Search2(int Left, int Right, int y);
};


#include <CHoleList.hpp>

#endif
