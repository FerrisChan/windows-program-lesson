// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the HOMEWORKDLL2_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// HOMEWORKDLL2_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HOMEWORKDLL2_EXPORTS
#define HOMEWORKDLL2_API __declspec(dllexport)
#else
#define HOMEWORKDLL2_API __declspec(dllimport)
#endif

// This class is exported from the dll
class HOMEWORKDLL2_API Chomeworkdll2 {
public:
	Chomeworkdll2(void);
	// TODO: add your methods here.
};

extern HOMEWORKDLL2_API int nhomeworkdll2;

HOMEWORKDLL2_API int fnhomeworkdll2(void);
