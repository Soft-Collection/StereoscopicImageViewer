#ifndef __CCRITICALSECTION_H__
#define __CCRITICALSECTION_H__

class CCriticalSectionPool
{
private:
	CRITICAL_SECTION* mCriticalSectionArray;
	INT               mCriticalSectionArrayLength;
public:
	CCriticalSectionPool(INT numOfCriticalSections);
	~CCriticalSectionPool();
	void Enter(INT criticalSectionNumber);
	BOOL TryEnter(INT criticalSectionNumber);
	void Leave(INT criticalSectionNumber);
};
#endif // __CCRITICALSECTION_H__