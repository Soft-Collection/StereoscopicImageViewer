#ifndef __CEXCEPTIONREPORT_H__
#define __CEXCEPTIONREPORT_H__

#include <stdio.h>
#include <time.h>

class CExceptionReport
{
private:
	static FILE* ms_fp;
	static CRITICAL_SECTION* ms_crit;
public:
	static BOOL SetFileName(char* fileName);
	static BOOL WriteExceptionReportToFile(char* functionLocation, char* exceptionDescription);
	static BOOL CloseFile();
};

#endif // __CEXCEPTIONREPORT_H__
