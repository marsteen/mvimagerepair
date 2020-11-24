#ifndef CFillHole_H
#define CFillHole_H

#include <mvlib/graphics/SDataRect.h>

//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
//
//---------------------------------------------------------------------------

template<typename Tdepth>
class CFillHole
{
    public:


        CFillHole()
        {
            mHoleCount = 0;
            mSource = NULL;
        }


        void Init(mvlib::SDataRect* Source, Tdepth& wrongp);
        void FillY();
        void FillX();
        void ReplacePixels();
        void Smooth();
        void Randomize(int k);
        void Finish();
        void CreatePtrLists();

        int mHoleCount;

    protected:

        void FillPixelValue(CHole<Tdepth>* BeginHole, Tdepth val);

        mvlib::SDataRect* mSource;
        Tdepth mWrongPixel;
        CHoleList<Tdepth>* mHoleList;
};


#include <CFillHole.hpp>

#endif
