//----------------------------------------------------------------------------
// AudioIO.hpp - bcbdcc32 generated hdr (DO NOT EDIT) rev: 0
// From: AudioIO.pas
//----------------------------------------------------------------------------
#ifndef AudioIOHPP
#define AudioIOHPP
//----------------------------------------------------------------------------
#include <UAFDefs.hpp>
#include <MMSystem.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Messages.hpp>
#include <Windows.hpp>
#include <System.hpp>
namespace Audioio
{
//-- type declarations -------------------------------------------------------
typedef bool __fastcall (__closure *TBufferEvent)(char * Buffer, int &Size);

class __declspec(delphiclass) TAudioIO;
typedef TAudioIO* *PAudioIO;

class __declspec(delphiclass) TAudioOut;
typedef TAudioOut* *PAudioOut;

class __declspec(delphiclass) TAudioIn;
typedef TAudioIn* *PAudioIn;

class __declspec(delphiclass) TCallBackWinOut;
class __declspec(pascalimplementation) TCallBackWinOut : public Controls::TWinControl
{
	typedef Controls::TWinControl inherited;
	
private:
	TAudioOut* *AudioComponent;
	MESSAGE void __fastcall BufferDone(Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveOpen(Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveClose(Messages::TMessage &Msg);
public:
	/* TWinControl.Create */ __fastcall virtual TCallBackWinOut(Classes::TComponent* AOwner) : Controls::
		TWinControl(AOwner) { }
	/* TWinControl.CreateParented */ __fastcall TCallBackWinOut(HWND ParentWindow) : Controls::TWinControl(
		ParentWindow) { }
	/* TWinControl.Destroy */ __fastcall virtual ~TCallBackWinOut(void) { }
	
};

class __declspec(delphiclass) TCallBackWinIn;
class __declspec(pascalimplementation) TCallBackWinIn : public Controls::TWinControl
{
	typedef Controls::TWinControl inherited;
	
private:
	TAudioIn* *AudioComponent;
	MESSAGE void __fastcall BufferFinished(Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveOpenIn(Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveCloseIn(Messages::TMessage &Msg);
public:
	/* TWinControl.Create */ __fastcall virtual TCallBackWinIn(Classes::TComponent* AOwner) : Controls::
		TWinControl(AOwner) { }
	/* TWinControl.CreateParented */ __fastcall TCallBackWinIn(HWND ParentWindow) : Controls::TWinControl(
		ParentWindow) { }
	/* TWinControl.Destroy */ __fastcall virtual ~TCallBackWinIn(void) { }
	
};

class __declspec(pascalimplementation) TAudioIO : public Classes::TComponent
{
	typedef Classes::TComponent inherited;
	
private:
	int FBufferSize;
	int FRequestedBufferSize;
	int FNumBuffers;
	bool FPaused;
	tWAVEFORMATEX FWaveFmtEx;
	Classes::TNotifyEvent FonOpen;
	Classes::TNotifyEvent FonClose;
	int FWaveDevice;
	int hWaveHeader[4];
	wavehdr_tag *WaveHdr[4];
	char *WaveBuffer[4];
	int hWaveBuffer[4];
	int BufIndex;
	bool ContinueProcessing;
	void __fastcall SetNumBuffers(int Value);
	void __fastcall SetBufferSize(int Value);
	void __fastcall SetFrameRate(int Value);
	void __fastcall SetStereo(bool Value);
	void __fastcall SetBits(Word Value);
	int __fastcall GetFrameRate(void);
	bool __fastcall GetStereo(void);
	void __fastcall MakeWaveFmtConsistent(void);
	
protected:
	bool __fastcall InitWaveHeaders(void);
	bool __fastcall AllocPCMBuffers(void);
	bool __fastcall FreePCMBuffers(void);
	bool __fastcall AllocWaveHeaders(void);
	void __fastcall FreeWaveHeaders(void);
	
public:
	System::AnsiString ErrorMessage;
	bool Active;
	int FilledBuffers;
	int QueuedBuffers;
	int ProcessedBuffers;
	__fastcall virtual TAudioIO(Classes::TComponent* AOwner);
	__fastcall virtual ~TAudioIO(void);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	
__published:
	__property int BufferSize = {read=FBufferSize, write=SetBufferSize, default=8192};
	__property int NumBuffers = {read=FNumBuffers, write=SetNumBuffers, default=4};
	__property int FrameRate = {read=GetFrameRate, write=SetFrameRate, default=22055};
	__property bool Stereo = {read=GetStereo, write=SetStereo, default=0};
	__property Word Quantization = {read=FWaveFmtEx.wBitsPerSample, write=SetBits, default=16};
	__property int WaveDevice = {read=FWaveDevice, write=FWaveDevice, default=-1};
	__property Classes::TNotifyEvent OnStart = {read=FonOpen, write=FonOpen};
	__property Classes::TNotifyEvent OnStop = {read=FonClose, write=FonClose};
};

class __declspec(pascalimplementation) TAudioOut : public TAudioIO
{
	typedef TAudioIO inherited;
	
private:
	bool WaveDeviceOpen;
	TCallBackWinOut* CallBackWin;
	TBufferEvent FOnFillBuffer;
	bool __fastcall QueueBuffer(void);
	virtual bool __fastcall ReadBuffer(int Idx, int N);
	void __fastcall SetPaused(bool Value);
	virtual void __fastcall CloseWaveDevice(void);
	virtual bool __fastcall Setup(TAudioOut* &TS);
	bool __fastcall StartIt(void);
	
public:
	int WaveHandle;
	bool __fastcall Start(TAudioOut* &TS);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	float __fastcall ElapsedTime(void);
	
__published:
	__property bool Paused = {read=FPaused, write=SetPaused, default=0};
	__property TBufferEvent OnFillBuffer = {read=FOnFillBuffer, write=FOnFillBuffer};
public:
	/* TAudioIO.Create */ __fastcall virtual TAudioOut(Classes::TComponent* AOwner) : Audioio::TAudioIO(
		AOwner) { }
	/* TAudioIO.Destroy */ __fastcall virtual ~TAudioOut(void) { }
	
};

class __declspec(pascalimplementation) TAudioIn : public TAudioIO
{
	typedef TAudioIO inherited;
	
private:
	bool WaveDeviceOpen;
	TCallBackWinIn* CallBackWin;
	TBufferEvent FOnBufferFilled;
	bool __fastcall QueueBuffer(void);
	virtual bool __fastcall ProcessBuffer(char * B, int N);
	virtual void __fastcall CloseWaveDevice(void);
	virtual bool __fastcall Setup(TAudioIn* &TS);
	bool __fastcall StartIt(void);
	
public:
	int WaveHandle;
	bool __fastcall Start(TAudioIn* &TS);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	float __fastcall ElapsedTime(void);
	
__published:
	__property TBufferEvent OnBufferFilled = {read=FOnBufferFilled, write=FOnBufferFilled};
public:
	/* TAudioIO.Create */ __fastcall virtual TAudioIn(Classes::TComponent* AOwner) : Audioio::TAudioIO(
		AOwner) { }
	/* TAudioIO.Destroy */ __fastcall virtual ~TAudioIn(void) { }
	
};

//-- var, const, procedure ---------------------------------------------------
#define MAXBUFFERS (Byte)(4)
extern void __fastcall Register(void);

}	/* namespace Audioio */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Audioio;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// AudioIO
