//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CList.hpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//           Imagon GmbH
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Methoden der Klasse CList
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 08.01.2002 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   NewListElem
//
//
//---------------------------------------------------------------------------

#ifndef CLIST_HPP
#define CLIST_HPP

template <class TListElem>
TListElem* CList<TListElem>::NewListElem()
{
  if (root == NULL)
  {
    root = new TListElem;
    root->prev = NULL;
    last = root;
  }
  else
  {
    last->next = new TListElem;
    last->next->prev = last;
    last = last->next;
  }
  last->next = NULL;
  return last;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   InsertListElem
//
// Fügt nach dem Element ie ein neues Element ein
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::InsertListElem(TListElem* ie)
{
	TListElem* ne = NULL;

	if (ie != NULL)
	{
		ne = new TListElem;

		ne->prev = ie;
		ne->next = ie->next;

		ie->next = ne;

		if (ne->next != NULL)
		{
			ne->next->prev = ne;
		}
		else
		{
			last = ne;
		}
	}
  return ne;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   InsertListElem
//
// Fügt nach dem Element ie ein das Element ne ein
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::InsertListElem(TListElem* ie, TListElem* ne)
{
	if (ie != NULL)
	{
		ne->prev = ie;
		ne->next = ie->next;

		ie->next = ne;

		if (ne->next != NULL)
		{
			ne->next->prev = ne;
		}
		else
		{
			last = ne;
		}
	}
  return ne;
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   InsertListElem
//
// Fügt VOR dem Element ie ein das Element ne ein
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::InsertListElemPrev(TListElem* ie, TListElem* ne)
{
	if (ie != NULL)
	{
		ne->next = ie;
		ne->prev = ie->prev;

		ie->prev = ne;

		if (ne->prev != NULL)
		{
			ne->prev->next = ne;
		}
		else
		{
			root = ne;
		}
	}
  return ne;
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetFirst
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::GetFirst()
{
	akt = root;

	if (root != NULL)
	{
		rnext = root->next;
	}
	else
	{
		rnext = NULL;
	}


	return root;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetLast
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::GetLast()
{
	akt = last;

	if (root != NULL)
	{
		rnext = root->prev;
	}
	else
	{
		rnext = NULL;
	}
	return akt;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetNext
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::GetNext()
{
	TListElem* NextNow = rnext;

	if (rnext != NULL)
	{
		rnext = rnext->next;
	}

	return NextNow;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetPrev
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::GetPrev()
{
	if (akt != NULL)
	{
		akt = akt->prev;
	}
	return akt;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   DeleteListElem
//
//
//---------------------------------------------------------------------------

template <class TListElem>
void CList<TListElem>::DeleteListElem(TListElem* le)
{
	if (le != NULL)
	{
		RemoveListElem(le);
		delete le;
	}
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   RemoveListElem
//
//
//---------------------------------------------------------------------------

template <class TListElem>
void CList<TListElem>::RemoveListElem(TListElem* le)
{
	if (le == root)
	{
		root = le->next;
		if (root != NULL)
		{
			root->prev = NULL;
		}
	}
	else
	{
		if (le->prev != NULL)
		{
			le->prev->next = le->next;
		}
	}

	if (le == last)
	{
		last = le->prev;
		if (last != NULL)
		{
			last->next = NULL;
		}
	}
	else
	{
		if (le->next != NULL)
		{
			le->next->prev = le->prev;
		}
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   NewListElem
//
//
//---------------------------------------------------------------------------


template <class TListElem>
void CList<TListElem>::NewListElem(TListElem* NeuElem)
{
  if (root == NULL)
  {
    root = NeuElem;
    root->prev = NULL;
    last = root;
  }
  else
  {
    last->next = NeuElem;
    last->next->prev = last;
    last = last->next;
  }
  last->next = NULL;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   ClearList
//
//
//---------------------------------------------------------------------------

template <class TListElem>
void CList<TListElem>::ClearList()
{
	TListElem* NextRecord;
	for (TListElem* se = root; se != NULL; se = NextRecord)
	{
		NextRecord = se->next;
		delete se;
	}
	last = root = NULL;

}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   Count
//
//
//---------------------------------------------------------------------------

template <class TListElem>
int CList<TListElem>::Count()
{
	int Anzahl = 0;

	for (TListElem* se = root; se != NULL; se = se->next)
	{
		Anzahl++;
	}
	return Anzahl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   CreatePtrList
//
// Erzeugen einer Zeigerliste (Array) mit Zeigern auf Listenelemente
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem** CList<TListElem>::CreatePtrList()
{
	int i = 0;
	int ListSize = Count();

	TListElem** PtrList = new TListElem*[ListSize+1];

	for (TListElem* se = root; se != NULL; se = se->next)
	{
		PtrList[i++] = se;
	}
	PtrList[i] = NULL;

	return PtrList;
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   CreatePtrList
//
// Erzeugen einer Zeigerliste (Array) mit Zeigern auf Listenelemente
//
//---------------------------------------------------------------------------

template <class TListElem>
int CList<TListElem>::CreatePtrList(TListElem** PtrList)
{
	int i = 0;

	for (TListElem* se = root; se != NULL; se = se->next)
	{
		PtrList[i++] = se;
	}
	PtrList[i] = NULL;

	return i;
}


//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetElemNr
//
// Liefert das n. Element der Liste
// n = -1 liefert das letzte Element (last)
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::GetElemNr(int nr)
{
	int i = 0;

	if (nr == -1)
	{
		return last;
	}

	for (TListElem* se = root; se != NULL; se = se->next)
	{
		if (i == nr)
		{
			return se;
		}
		i++;
	}
	return NULL;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   GetListNr
//
// Liefert die Nummer des Element der Liste
//
//---------------------------------------------------------------------------

template <class TListElem>
int CList<TListElem>::GetListNr(TListElem* le)
{
	int i = 0;

	for (TListElem* se = root; se != NULL; se = se->next)
	{
		if (se == le)
		{
			return i;
		}
		i++;
	}
	return -1;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   SwapNext
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::SwapNext(TListElem* le1)
{
	if (le1->next != NULL)
	{
		TListElem* le1prev = le1->prev;
		TListElem* le2 = le1->next;

		if (le1->prev != NULL)
		{
			le1->prev->next = le2;
		}
		if (le2->next != NULL)
		{
			le2->next->prev = le1;
		}

		le1->prev = le2;
		le1->next = le2->next;

		le2->next = le1;
		le2->prev = le1prev;

		if (root == le1)
		{
			root = le2;
		}
		else
		if (root == le2)
		{
			root = le1;
		}

		if (last == le1)
		{
			last = le2;
		}
		else
		if (last == le2)
		{
			last = le1;
		}
		return le2;
	}
	return le1;
}

//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   SwapPrev
//
//
//---------------------------------------------------------------------------

template <class TListElem>
TListElem* CList<TListElem>::SwapPrev(TListElem* le2)
{
	if (le2->prev != NULL)
	{
		return SwapNext(le2->prev);
	}
	return le2;
}



//---------------------------------------------------------------------------
//
// Klasse:    CList
// Methode:   SwapElem
//
//
//---------------------------------------------------------------------------

template <class TListElem>
bool CList<TListElem>::SwapElem(TListElem* le1, TListElem* le2)
{
	if ((le1 != NULL) && (le2 != NULL))
	{
		if (le1->next == le2)
		{
			SwapNext(le1);
		}
		else
		if (le2->next == le1)
		{
			SwapNext(le2);
		}
		else
		{
			TListElem sw;

			sw.next = le1->next;
			sw.prev = le1->prev;

			le1->prev->next = le2;
			le1->next->prev = le2;

			le2->prev->next = le1;
			le2->next->prev = le1;

			le1->prev = le2->prev;
			le1->next = le2->next;

			le2->prev = sw.prev;
			le2->next = sw.next;

			if (root == le1) root = le2;
			else if (root == le2) root = le1;

			if (last == le1) last = le2;
			else if (last == le2) last = le1;

		}
	}
    return true;
}


template <class TListElem>
TListElem** CList<TListElem>::CreatePtrListSorted(int (*CmpListElem)(const void* v1, const void* v2), int* ListCount)
{
	int cnt = Count();

	TListElem*   le;
	TListElem**  leList =	CreatePtrList();
	qsort(leList, cnt, 4, CmpListElem);

	if (ListCount != NULL)
	{
		*ListCount = cnt;
	}
	return leList;
}

template <class TListElem>
void CList<TListElem>::SortList(int (*CmpListElem)(const void* v1, const void* v2))
{
	int cnt;
	TListElem** leList = CreatePtrListSorted(CmpListElem, &cnt);
	TListElem*  le;

	root = leList[0];
	last = leList[cnt-1];

	for (int i = 0; i < cnt; i++)
	{
		le = leList[i];

		if (i < cnt-1)
		{
			le->next = leList[i+1];
		}
		else
		{
			le->next = NULL;
		}

		if (i > 0)
		{
			le->prev = leList[i-1];
		}
		else
		{
			le->prev = NULL;
		}
	}
	delete[] leList;
}

#endif
