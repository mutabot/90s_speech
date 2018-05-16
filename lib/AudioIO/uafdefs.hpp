//----------------------------------------------------------------------------
// UAFDefs.hpp - bcbdcc32 generated hdr (DO NOT EDIT) rev: 0
// From: UAFDefs.pas
//----------------------------------------------------------------------------
#ifndef UAFDefsHPP
#define UAFDefsHPP
//----------------------------------------------------------------------------
#include <System.hpp>
namespace Uafdefs
{
//-- type declarations -------------------------------------------------------
enum UAF_FILE_TYPE { UAF_TYPE_UNKNOWN, UAF_TYPE_ADF, UAF_TYPE_WAV, UAF_TYPE_AU, UAF_TYPE_AIFF, UAF_TYPE_RAW 
	};

struct UAF_File
{
	double FrameRate;
	Word Channels;
	Word Quantization;
	long Frames;
	Word FrameSize;
	void *FileRecord;
	UAF_FILE_TYPE FileType;
	int SubType;
	Word FrameSizeIn;
} ;

//-- var, const, procedure ---------------------------------------------------
extern "C" bool __fastcall UAF_open(UAF_File &uaf,  System::AnsiString fname, char mode, UAF_FILE_TYPE 
	ForceType);
extern "C" bool __fastcall UAF_create(UAF_File &uaf,  System::AnsiString fname, UAF_FILE_TYPE ftype, 
	Word subformat, double srate, Word nchannels, Word bits);
extern "C" bool __fastcall UAF_close(UAF_File &uaf);
extern "C" long __fastcall UAF_read(UAF_File &uaf, void * buffer, long nFrames, long lpos);
extern "C" long __fastcall UAF_write(UAF_File &uaf, void * buffer, long nFrames, long lpos);
extern "C" bool __fastcall UAF_CreateFromUAF(UAF_File &uafin, UAF_File &uafout,  System::AnsiString 
	fname);
extern "C" bool __fastcall UAF_SaveSection(UAF_File &uafin, UAF_File &uafout, long lstart, long lend
	);
extern "C" void __fastcall UAF_Copy_Marks(UAF_File &uafin, UAF_File &uafout);
extern "C" bool __fastcall FindMinMaxUAF( System::AnsiString FileIn, float &Min, float &Max, UAF_FILE_TYPE 
	OverrideType);
extern "C" UAF_FILE_TYPE __fastcall UAFTypeFromExtension( System::AnsiString fname);
extern "C" char * __fastcall UAF_ErrorMessage(void);
extern "C" char * __fastcall UAF_Identity(const UAF_File &UAFIn);
extern "C" char * __fastcall UAF_Description(const UAF_File &UAFIn);

}	/* namespace Uafdefs */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Uafdefs;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// UAFDefs
