#include "stdafx.h"
#include "CCriticalSectionPool.h"

CCriticalSectionPool::CCriticalSectionPool(INT numOfCriticalSections)
{
	mCriticalSectionArray = NULL;
	mCriticalSectionArrayLength = numOfCriticalSections;
	mCriticalSectionArray = new CRITICAL_SECTION[mCriticalSectionArrayLength];
	//------------------------------------------------------------------------
	for (INT i = 0; i < mCriticalSectionArrayLength; i++)
	{
		InitializeCriticalSection(&mCriticalSectionArray[i]);
	}
}
CCriticalSectionPool::~CCriticalSectionPool()
{
	for (INT i = 0; i < mCriticalSectionArrayLength; i++)
	{
		DeleteCriticalSection(&mCriticalSectionArray[i]);
	}
	//------------------------------------------------------------------------
	if (mCriticalSectionArray != NULL)
	{
		delete[] mCriticalSectionArray;
		mCriticalSectionArray = NULL;
	}
}
void CCriticalSectionPool::Enter(INT criticalSectionNumber)
{
	EnterCriticalSection(&mCriticalSectionArray[criticalSectionNumber]);
}
BOOL CCriticalSectionPool::TryEnter(INT criticalSectionNumber)
{
	return(TryEnterCriticalSection(&mCriticalSectionArray[criticalSectionNumber]));
}
void CCriticalSectionPool::Leave(INT criticalSectionNumber)
{
	LeaveCriticalSection(&mCriticalSectionArray[criticalSectionNumber]);
}