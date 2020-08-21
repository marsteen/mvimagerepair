#ifndef CHole_H
#define CHole_H

#include <CList.h>
#include <CList.hpp>


//class CHole;
//typedef CListElem<CHole> THoleListElem;

//---------------------------------------------------------------------------
//
// Klasse:    CHole
//
//---------------------------------------------------------------------------

template <typename Tdepth>
class CHole : public CListElem<CHole<Tdepth> >
{
	public:

		CHole()
		{
			mXcount = 0;
			mYcount = 0;
		}

		int           mYval;
		Tdepth        mPixel;

		int           mXcount;
		int           mYcount;
};



#endif
