#ifndef CFillHole_H
#define CFillHole_H

//---------------------------------------------------------------------------
//
// Klasse:    CFillHole
//
//---------------------------------------------------------------------------

template<typename Tdepth>
class CFillHole
{
    public:

        void Init(CDataRect* Source);
        void FillY(void);
        void FillX(void);
        void ReplacePixels(void);
        void Smooth(void);
        void Randomize(int k);
        void Finish(void);
        void CreatePtrLists(void);

        int mHoleCount;

    protected:

        void FillPixelValue(CHole<Tdepth>* BeginHole, Tdepth val);

        CDataRect* mSource;
        Tdepth mWrongPixel;
        CHoleList<Tdepth>* mHoleList;
};

#endif
