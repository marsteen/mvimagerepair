#ifndef CLIST_H
#define CLIST_H

template<class TListElem>
class CListElem
{
    public:

        TListElem* next;    // Zeiger auf nächstes Element
        TListElem* prev;    // Zeiger auf vorheriges Element
};

template<class TListElem>
class CList
{
    public:

        TListElem* NewListElem();
        TListElem* GetFirst();
        TListElem* GetNext();
        TListElem* GetLast();
        TListElem* GetPrev();
        TListElem* GetElemNr(int nr);
        int GetListNr(TListElem* le);
        TListElem* InsertListElem(TListElem* ie);
        TListElem* InsertListElem(TListElem* ie, TListElem* te);
        TListElem* InsertListElemPrev(TListElem* ie, TListElem* ne);



        void NewListElem(TListElem*);

        void DeleteListElem(TListElem* le);
        void RemoveListElem(TListElem* le);
        bool SwapElem(TListElem* le1, TListElem* le2);
        TListElem* SwapNext(TListElem* le1);
        TListElem* SwapPrev(TListElem* le2);

        int GetOffset(TListElem* le, void* mptr);


        TListElem** CreatePtrList(void);
        int CreatePtrList(TListElem** PtrList);

        TListElem** CreatePtrListSorted(int (* CmpListElem)(const void* v1, const void* v2), int* ListCount);
        void SortList(int (* CmpListElem)(const void* v1, const void* v2));

        int Count(void);

        void SetEmpty()
        {
            root = NULL;
            last = NULL;
        }


        CList()
        {
            SetEmpty();
        }


        void ClearList(void);



        ~CList()
        {
            ClearList();
        }


    protected:

        TListElem* root;
        TListElem* last;
        TListElem* akt;
        TListElem* rnext;
};


// Die Baumstruktur ist Liste und Element zugleich

template<class TTreeElem>
class CTreeElem : public CList<TTreeElem>, public CListElem<TTreeElem>
{
    public:
};

#endif
