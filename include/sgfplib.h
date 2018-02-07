#ifndef SGFPLIB_H
#define SGFPLIB_H

#if (defined(WIN32) || defined(_WIN32_WCE))
   #include <windows.h>
  
   #ifdef SGFPLIB_EXPORTS
     #define SGFPM_DLL_DECL __declspec(dllexport)
   #else
     //#define SGFPM_DLL_DECL __declspec(dllimport)
     #define SGFPM_DLL_DECL
   #endif

#else
   #define SGFPM_DLL_DECL
   #define WINAPI
   #define FAR

   #ifndef NULL
   #define NULL 0
   #endif

   typedef void*           HWND;
   typedef void*           HDC;
   typedef unsigned long   DWORD;
   typedef int             BOOL;
   typedef unsigned char   BYTE;
   typedef unsigned short  WORD;

   typedef struct
   {   
      int left, top, right, bottom;
   } RECT;

   typedef RECT* LPRECT;

#endif/* WIN32 */

#ifndef __cplusplus //2015-04-10
typedef BOOL bool;
#endif //__cplusplus//2015-04-10

#ifdef __cplusplus
extern "C" {
#endif

#define FDU03_MSG   1001
#define FDU04_MSG   4001
#define FDU05_MSG   5001
#define FDU06_MSG   6001
#define MAX_SEND_SIZE 80


enum SGFDxDeviceName
{
   SG_DEV_UNKNOWN = 0,
   SG_DEV_FDP02 = 0x01,
   SG_DEV_FDU02 = 0x03,
   SG_DEV_FDU03 = 0x04,
   SG_DEV_FDU04 = 0x05,
   SG_DEV_FDU05 = 0x06,
   SG_DEV_FDU06 = 0x07,
	SG_DEV_AUTO = 0xFF,
};

enum SGPPPortAddr {
   AUTO_DETECT = 0,
   LPT1        = 0x378,
   LPT2        = 0x278,
   LPT3        = 0x3BC,
   USB_AUTO_DETECT = 0x3BC+1,
};

enum SGFDxSecurityLevel 
{
   SL_NONE = 0,
   SL_LOWEST = 1,
   SL_LOWER = 2,
   SL_LOW = 3,
   SL_BELOW_NORMAL = 4,   
   SL_NORMAL = 5,
   SL_ABOVE_NORMAL = 6,
   SL_HIGH = 7,
   SL_HIGHER = 8,
   SL_HIGHEST = 9,
};


enum SGFDxTemplateFormat 
{
   TEMPLATE_FORMAT_ANSI378 = 0x0100,
   TEMPLATE_FORMAT_SG400   = 0x0200,
   TEMPLATE_FORMAT_ISO19794 = 0x0300,
};

enum SGCallBackFuntion {
   CALLBACK_LIVE_CAPTURE = 1,
   CALLBACK_AUTO_ON_EVENT = 2,
};


enum SGFDxErrorCode {
   // General error
   SGFDX_ERROR_NONE = 0,
   SGFDX_ERROR_CREATION_FAILED = 1,
   SGFDX_ERROR_FUNCTION_FAILED = 2,
   SGFDX_ERROR_INVALID_PARAM = 3,
   SGFDX_ERROR_NOT_USED = 4,
   SGFDX_ERROR_DLLLOAD_FAILED = 5,
   SGFDX_ERROR_DLLLOAD_FAILED_DRV = 6,
   SGFDX_ERROR_DLLLOAD_FAILED_ALGO = 7,
   SGFDX_ERROR_NO_LONGER_SUPPORTED = 8,

   // Device error
   SGFDX_ERROR_SYSLOAD_FAILED = 51, // system file load fail
   SGFDX_ERROR_INITIALIZE_FAILED = 52,   // chip initialize fail
   SGFDX_ERROR_LINE_DROPPED = 53,        // image data drop
   SGFDX_ERROR_TIME_OUT = 54,            // getliveimage timeout error
   SGFDX_ERROR_DEVICE_NOT_FOUND = 55,    // device not found
   SGFDX_ERROR_DRVLOAD_FAILED = 56,      // dll file load fail
   SGFDX_ERROR_WRONG_IMAGE = 57,         // wrong image
	SGFDX_ERROR_LACK_OF_BANDWIDTH = 58,   // USB Bandwith Lack Error
   SGFDX_ERROR_DEV_ALREADY_OPEN = 59,    // Device Exclusive access Error
   SGFDX_ERROR_GETSN_FAILED = 60,         // Fail to get Device Serial Number
   SGFDX_ERROR_UNSUPPORTED_DEV = 61,      // Unsupported device
   
   // Extract &Matching error
   SGFDX_ERROR_FEAT_NUMBER = 101,  // too small number of minutiae
   SGFDX_ERROR_INVALID_TEMPLATE_TYPE = 102,      // wrong template type
   SGFDX_ERROR_INVALID_TEMPLATE1 = 103,         //error in decoding template 1
   SGFDX_ERROR_INVALID_TEMPLATE2 = 104,         //error in decoding template 2
   SGFDX_ERROR_EXTRACT_FAIL = 105,      
   SGFDX_ERROR_MATCH_FAIL = 106,

   // Licensing error
   SGFDX_ERROR_LICENSE_LOAD = 501,
   SGFDX_ERROR_LICENSE_KEY = 502,
   SGFDX_ERROR_LICENSE_EXPIRED = 503,
   SGFDX_ERROR_LICENSE_WRITE = 504
};

enum SGImpressionType 
{
   SG_IMPTYPE_LP =	0x00,		// Live-scan plain
   SG_IMPTYPE_LR =	0x01,		// Live-scan rolled
   SG_IMPTYPE_NP =	0x02,		// Nonlive-scan plain
   SG_IMPTYPE_NR =	0x03,		// Nonlive-scan rolled
};

enum SGFingerPosition
{ 
   SG_FINGPOS_UK = 0x00,		// Unknown finger
   SG_FINGPOS_RT = 0x01,		// Right thumb
   SG_FINGPOS_RI = 0x02,		// Right index finger
   SG_FINGPOS_RM = 0x03,		// Right middle finger
   SG_FINGPOS_RR = 0x04,		// Right ring finger
   SG_FINGPOS_RL = 0x05,		// Right little finger
   SG_FINGPOS_LT = 0x06,		// Left thumb
   SG_FINGPOS_LI = 0x07,		// Left index finger
   SG_FINGPOS_LM = 0x08,		// Left middle finger
   SG_FINGPOS_LR = 0x09,		// Left ring finger
   SG_FINGPOS_LL = 0x0A,		// Left little finger
};


#define SGDEV_SN_LEN          15   // Device Serial Number Length
// For AutoOn event
#ifndef WM_APP_SGAUTOONEVENT
#define WM_APP_SGAUTOONEVENT    0x8100  // From ISensor.h
#define SGDEVEVNET_FINGER_OFF   0
#define SGDEVEVNET_FINGER_ON    1

#endif

// EnumerateDevice()
typedef struct tagSGDeviceList
{
   DWORD DevName;
   DWORD DevID;
   WORD  DevType;   
   BYTE  DevSN[SGDEV_SN_LEN+1];
} SGDeviceList;
typedef SGDeviceList *LPDeviceList;

// GetDeviceInfo()
typedef struct tagSGDeviceInfoParam
{
   DWORD DeviceID;         // 0 - 9
   BYTE  DeviceSN[SGDEV_SN_LEN+1];         // Device Serial Number, Length of SN = 15    
   DWORD ComPort;          // Parallel device=>PP address, USB device=>USB(0x3BC+1)
   DWORD ComSpeed;         // Parallel device=>PP mode, USB device=>0 
   DWORD ImageWidth;       // Image Width
   DWORD ImageHeight;      // Image Height
   DWORD Contrast; 	      // 0 ~ 100
   DWORD Brightness;       // 0 ~ 100
   DWORD	Gain;             // Dependent on each device
   DWORD ImageDPI;         // DPI
   DWORD FWVersion;        // FWVersion
} SGDeviceInfoParam;
typedef SGDeviceInfoParam* LPSGDeviceInfoParam;

typedef struct tagSGFingerInfo {
    WORD FingerNumber;			// FingerNumber. 
    WORD ViewNumber;           // Sample number
    WORD ImpressionType;       // impression type. Should be 0
    WORD ImageQuality;         // Image quality
} SGFingerInfo;

typedef struct tagSGANSITemplateInfo {
    DWORD TotalSamples; 
    SGFingerInfo SampleInfo[225];		//max finger number 15 x max view number 15 = 225
} SGANSITemplateInfo, SGISOTemplateInfo;

typedef struct tagSGCBLiveCaptureParam {
   DWORD  ImageWidth;
   DWORD  ImageHeight;
   DWORD  Quality;
   DWORD  ErrorCode;
} SGCBLiveCaptureParam;

// 2006.10.9, Used in CreateTemplate2()
typedef struct tagSGFPImageInfo {
	WORD CaptureEquip;			// capture equipment ID
	WORD ImageSizeInX;            // in pixels
	WORD ImageSizeInY;            // in pixels
	WORD XResolution;             // in pixels per cm 500dpi = 197
	WORD YResolution;             // in pixels per cm
	WORD FingerNumber;			// capture equipment ID
	WORD ViewNumber;            // in pixels
	WORD ImpressionType;            // in pixels
	WORD ImageQuality;             // in pixels per cm 500dpi = 197
} SGFPImageInfo;

typedef struct tagSGANSITemplateInfoEx {
    DWORD TotalSamples; 
    SGFPImageInfo SampleInfo[225];		//max finger number 15 x max view number 15 = 225
} SGANSITemplateInfoEx, SGISOTemplateInfoEx;

#ifdef __cplusplus //2015-04-10

///////////////////////////////////////////////
// For direcrect access for SGFPM Class 
///////////////////////////////////////////////
struct SGFPM
{
   virtual ~SGFPM(){};
   virtual DWORD WINAPI  GetLastError() = 0;
   virtual DWORD WINAPI  Init(DWORD devName) = 0;
   //virtual DWORD WINAPI  Init(DWORD width, DWORD height, DWORD dpi) = 0; // Some compiler can not recognize fuction overloading..
   virtual DWORD WINAPI  InitEx(DWORD width, DWORD height, DWORD dpi) = 0;
   virtual DWORD WINAPI  SetTemplateFormat(WORD format) = 0; // default is SG400

   // Image sensor API
   virtual DWORD WINAPI  EnumerateDevice(DWORD* ndevs, SGDeviceList** devList) = 0;
   virtual DWORD WINAPI  OpenDevice(DWORD devId) = 0;
   virtual DWORD WINAPI  CloseDevice() = 0;
   virtual DWORD WINAPI  GetDeviceInfo(SGDeviceInfoParam* pInfo)= 0;
   virtual DWORD WINAPI  Configure(HWND hwnd) = 0;
   virtual DWORD WINAPI  SetBrightness(DWORD brightness) = 0;
   virtual DWORD WINAPI  SetLedOn(bool on) = 0;

   virtual DWORD WINAPI  GetImage(BYTE* buffer)= 0;
   virtual DWORD WINAPI  GetImageEx(BYTE* buffer, DWORD timeout, HWND dispWnd, DWORD quality)= 0;
   virtual DWORD WINAPI  GetImageEx2(BYTE* buffer, DWORD timeout, HDC dispDC, LPRECT dispRect, DWORD quality)= 0;
   virtual DWORD WINAPI  GetImageQuality(DWORD width, DWORD height, BYTE* imgBuf, DWORD* quality)= 0;
   virtual DWORD WINAPI  SetCallBackFunction(DWORD selector, DWORD (WINAPI*)(void* pUserData, void* pCallBackData), void* pUserData) = 0;
   
   // FDU03 Only APIs
	virtual DWORD WINAPI  EnableAutoOnEvent(BOOL enable, HWND hwnd, void* reserved)= 0;

   // Algorithm: Extraction API
   virtual DWORD WINAPI  GetMaxTemplateSize(DWORD* size) = 0;
   virtual DWORD WINAPI  CreateTemplate(SGFingerInfo* fpInfo, BYTE *rawImage, BYTE* minTemplate)= 0;
   virtual DWORD WINAPI  GetTemplateSize(BYTE* buf, DWORD* size) = 0;

   // Algorithm: Matching API
   virtual DWORD  WINAPI  MatchTemplate(BYTE *minTemplate1, BYTE *minTemplate2, DWORD secuLevel, BOOL* matched) = 0;
   virtual DWORD  WINAPI  GetMatchingScore(BYTE* minTemplate1, BYTE* minTemplate2, DWORD* score) = 0;

   // Algorithim: Only work with ANSI378 Template
   virtual DWORD  WINAPI  GetTemplateSizeAfterMerge(BYTE* ansiTemplate1, BYTE* ansiTemplate2, DWORD* size) = 0;
   virtual DWORD  WINAPI  MergeAnsiTemplate(BYTE* ansiTemplate1, BYTE* ansiTemplate2, BYTE* outTemplate) = 0;
   virtual DWORD  WINAPI  MergeMultipleAnsiTemplate(BYTE* inTemplates, DWORD nTemplates, BYTE* outTemplate) = 0;
   virtual DWORD  WINAPI  GetAnsiTemplateInfo(BYTE* ansiTemplate, SGANSITemplateInfo* templateInfo) = 0;
   virtual DWORD  WINAPI  MatchAnsiTemplate(BYTE*  ansiTemplate1, DWORD  sampleNum1, BYTE*  ansiTemplate2, DWORD sampleNum2, DWORD secuLevel, BOOL*  matched) = 0;
   virtual DWORD  WINAPI  GetAnsiMatchingScore(BYTE*  ansiTemplate1, DWORD    sampleNum1, BYTE* ansiTemplate2, DWORD sampleNum2, DWORD* score) = 0;


   // Algorithim: Only work with ISO19794 Template
   virtual DWORD  WINAPI  GetIsoTemplateSizeAfterMerge(BYTE* isoTemplate1, BYTE* isoTemplate2, DWORD* size) = 0;
   virtual DWORD  WINAPI  MergeIsoTemplate(BYTE* isoTemplate1, BYTE* isoTemplate2, BYTE* outTemplate) = 0;
   virtual DWORD  WINAPI  MergeMultipleIsoTemplate(BYTE* inTemplates, DWORD nTemplates, BYTE* outTemplate) = 0;
   virtual DWORD  WINAPI  GetIsoTemplateInfo(BYTE* isoTemplate, SGISOTemplateInfo* templateInfo) = 0;
   virtual DWORD  WINAPI  MatchIsoTemplate(BYTE*  isoTemplate1, DWORD sampleNum1, BYTE*  isoTemplate2, DWORD sampleNum2, DWORD secuLevel, BOOL*  matched) = 0;
   virtual DWORD  WINAPI  GetIsoMatchingScore(BYTE*  isoTemplate1, DWORD sampleNum1, BYTE* isoTemplate2, DWORD sampleNum2, DWORD* score) = 0;

   // Algorithim: 
   virtual DWORD  WINAPI  MatchTemplateEx(BYTE*  minTemplate1, WORD tempateType1,  DWORD sampleNum1, BYTE* minTemplate2, WORD tempateType2,  DWORD sampleNum2, DWORD  secuLevel, BOOL*  matched) = 0;
   virtual DWORD  WINAPI  GetMatchingScoreEx(BYTE* minTemplate1, WORD tempateType1, DWORD sampleNum1, BYTE* minTemplate2, WORD tempateType2, DWORD sampleNum2, DWORD* score) = 0;

   // 2006.6.5, Device Driver
   virtual  DWORD	WINAPI  SetAutoOnIRLedTouchOn(BOOL iRLed, BOOL touchOn) = 0;
   // Algorithm, 2006.10.9, jkang
   virtual  DWORD  WINAPI  GetMinexVersion(DWORD *extractor, DWORD* matcher) = 0;

   // Algorithm, 2006.10.9, Can set Image width, height, xRes, yRes separately 
   virtual  DWORD  WINAPI  CreateTemplateEx(SGFPImageInfo* fpImageInfo, BYTE *rawImage, BYTE* minTemplate)= 0;
   // Algorithm, 2006.10.9, have Image width, height, xRes, yRes separately 
   virtual DWORD  WINAPI   GetAnsiTemplateInfoEx(BYTE* ansiTemplate, SGANSITemplateInfoEx* templateInfo) = 0;

   // 06/08/2009, Enable/disable the check of finger liveness
   virtual DWORD WINAPI		EnableCheckOfFingerLiveness(bool enable) = 0;

   // 05/19/2011, Adjust fake detection level
   virtual DWORD WINAPI		SetFakeDetectionLevel(int level) = 0;

   // 05/27/2011, Get fake detection level
   virtual DWORD WINAPI		GetFakeDetectionLevel(int *level) = 0;

   // 09/09/2011, Send commands to device
   virtual DWORD WINAPI		WriteData(unsigned char index, unsigned char data) = 0;

   // 11/10/2015, InitEx2()
   virtual DWORD WINAPI  InitEx2(DWORD width, DWORD height, DWORD dpi, char *licenseFilePath) = 0;

   // 11/18/2015, NFIQ
   virtual DWORD WINAPI  ComputeNFIQ(BYTE* imgBuf, DWORD width, DWORD height) = 0;
   virtual DWORD WINAPI  ComputeNFIQEx(BYTE*  imgBuf, DWORD width, DWORD height, DWORD ppi) = 0;

};


//struct SGFPM;
typedef SGFPM  FAR* LPSGFPM;
typedef DWORD (WINAPI* SGFPM_CreateFunc)(LPSGFPM* ppFPM);
typedef DWORD (WINAPI* SGFPM_DestroyFunc)(SGFPM* pFPM);
 
SGFPM_DLL_DECL DWORD WINAPI  CreateSGFPMObject(LPSGFPM* ppFPM = NULL);
SGFPM_DLL_DECL DWORD WINAPI  DestroySGFPMObject(SGFPM* pFPM = NULL);

#endif //__cplusplus //2015-04-10

//------------------------------------------------------------------------------
typedef void*  HSGFPM;

SGFPM_DLL_DECL DWORD WINAPI  SGFPM_Create(HSGFPM* phFPM);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_Terminate(HSGFPM hFpm);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_Init(HSGFPM hFpm, DWORD devName);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_InitEx(HSGFPM hFpm, DWORD width, DWORD height, DWORD dpi);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_SetTemplateFormat(HSGFPM hFpm, WORD format); // default is ANSI378
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetLastError(HSGFPM hFpm);

// Image sensor API
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_EnumerateDevice(HSGFPM hFpm, DWORD* ndevs, SGDeviceList** devList);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_OpenDevice(HSGFPM hFpm, DWORD devId);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_CloseDevice(HSGFPM hFpm);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetDeviceInfo(HSGFPM hFpm, SGDeviceInfoParam* pInfo);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_Configure(HSGFPM hFpm, HWND hwnd);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_SetBrightness(HSGFPM hFpm, DWORD brightness);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_SetLedOn(HSGFPM hFpm, bool on);

SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetImage(HSGFPM hFpm, BYTE* buffer);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetImageEx(HSGFPM hFpm, BYTE* buffer, DWORD time, HWND dispWnd, DWORD quality);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetImageEx2(HSGFPM hFpm, BYTE* buffer, DWORD time, HDC dispDC, LPRECT dispRect, DWORD quality);

SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetImageQuality(HSGFPM hFpm, DWORD width, DWORD height, BYTE* imgBuf, DWORD* quality);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_SetCallBackFunction(HSGFPM hFpm, DWORD selector, DWORD (WINAPI*)(void* pUserData, void* pCallBackData), void* pUserData);

// FDU03 or Later device Only APIs
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_EnableAutoOnEvent(HSGFPM hFpm, BOOL enable, HWND hwnd, void* reserved);

// Algorithm: Extraction API
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetMaxTemplateSize(HSGFPM hFpm, DWORD* size);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_CreateTemplate(HSGFPM hFpm, SGFingerInfo* fpInfo, BYTE *rawImage, BYTE* minTemplate);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetTemplateSize(HSGFPM hFpm, BYTE* minTemplate, DWORD* size);

// Algorithm: Matching API
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_MatchTemplate(HSGFPM hFpm, BYTE *minTemplate1, BYTE *minTemplate2, DWORD secuLevel, BOOL* matched);
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_GetMatchingScore(HSGFPM hFpm, BYTE* minTemplate1, BYTE* minTemplate2, DWORD* score);

// Algorithim: Only work with ANSI378 Template
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetTemplateSizeAfterMerge(HSGFPM hFpm, BYTE* ansiTemplate1, BYTE* ansiTemplate2, DWORD* size);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MergeAnsiTemplate(HSGFPM hFpm, BYTE* ansiTemplate1, BYTE* ansiTemplate2, BYTE* outTemplate);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MergeMultipleAnsiTemplate(HSGFPM hFpm, BYTE* inTemplates, DWORD nTemplates, BYTE* outTemplate);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetAnsiTemplateInfo(HSGFPM hFpm, BYTE* ansiTemplate, SGANSITemplateInfo* templateInfo);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MatchAnsiTemplate(HSGFPM hFpm, BYTE* ansiTemplate1, DWORD sampleNum1, BYTE* ansiTemplate2, DWORD  sampleNum2, DWORD  secuLevel, BOOL*  matched);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetAnsiMatchingScore(HSGFPM hFpm, BYTE* ansiTemplate1, DWORD  sampleNum1, BYTE* ansiTemplate2, DWORD    sampleNum2, DWORD* score);

SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MatchTemplateEx(HSGFPM hFpm, BYTE* minTemplate1, WORD   tempateType1,  DWORD  sampleNum1, BYTE* minTemplate2, WORD   tempateType2,  DWORD sampleNum2, DWORD  secuLevel, BOOL*  matched);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetMatchingScoreEx(HSGFPM hFpm, BYTE* minTemplate1, WORD tempateType1, DWORD sampleNum1, BYTE* minTemplate2, WORD tempateType2, DWORD sampleNum2, DWORD* score);

SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_SetAutoOnIRLedTouchOn(HSGFPM hFpm, BOOL iRLed, BOOL touchOn);

SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetMinexVersion(HSGFPM hFpm, DWORD *extractor, DWORD* matcher);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_CreateTemplateEx(HSGFPM hFpm, SGFPImageInfo* fpImageInfo, BYTE *rawImage, BYTE* minTemplate);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetAnsiTemplateInfoEx(HSGFPM hFpm, BYTE* ansiTemplate, SGANSITemplateInfoEx* templateInfo);

// Algorithim: Only work with ISO19794 Template
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetIsoTemplateSizeAfterMerge(HSGFPM hFpm, BYTE* isoTemplate1, BYTE* isoTemplate2, DWORD* size);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MergeIsoTemplate(HSGFPM hFpm, BYTE* isoTemplate1, BYTE* isoTemplate2, BYTE* outTemplate);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MergeMultipleIsoTemplate(HSGFPM hFpm, BYTE* inTemplates, DWORD nTemplates, BYTE* outTemplate);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetIsoTemplateInfo(HSGFPM hFpm, BYTE* isoTemplate, SGISOTemplateInfo* templateInfo);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_MatchIsoTemplate(HSGFPM hFpm, BYTE* isoTemplate1, DWORD sampleNum1, BYTE* isoTemplate2, DWORD  sampleNum2, DWORD  secuLevel, BOOL*  matched);
SGFPM_DLL_DECL DWORD  WINAPI  SGFPM_GetIsoMatchingScore(HSGFPM hFpm, BYTE* isoTemplate1, DWORD  sampleNum1, BYTE* isoTemplate2, DWORD sampleNum2, DWORD* score);

// 06/08/2009, Enable/disable the check of finger liveness
SGFPM_DLL_DECL DWORD WINAPI	  SGFPM_EnableCheckOfFingerLiveness(HSGFPM hFpm, BOOL enable);

// 05/19/2011, Adjust fake detection level
SGFPM_DLL_DECL DWORD WINAPI	  SGFPM_SetFakeDetectionLevel(HSGFPM hFpm, int level);	

// 05/27/2011, Get fake detection level
SGFPM_DLL_DECL DWORD WINAPI	  SGFPM_GetFakeDetectionLevel(HSGFPM hFpm, int *level);

// 09/09/2011, Send commands to device
SGFPM_DLL_DECL DWORD WINAPI	  SGFPM_WriteData(HSGFPM hFpm, unsigned char index, unsigned char data);

// 11/10/2015, InitEx2
SGFPM_DLL_DECL DWORD WINAPI  SGFPM_InitEx2(HSGFPM hFpm, DWORD width, DWORD height, DWORD dpi, char *licenseFilePath);

// 11/18/2015, NFIQ
SGFPM_DLL_DECL DWORD WINAPI  ComputeNFIQ(HSGFPM hFpm, BYTE* imgBuf, DWORD width, DWORD height);
SGFPM_DLL_DECL DWORD WINAPI  ComputeNFIQEx(HSGFPM hFpm, BYTE*  imgBuf, DWORD width, DWORD height, DWORD ppi);


#ifdef __cplusplus
};
#endif

#endif

