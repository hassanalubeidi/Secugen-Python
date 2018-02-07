/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : Multidev main.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include "stdio.h"

#include "sgfplib.h"

LPSGFPM  sgfplib = NULL;



// ---------------------------------------------------------------- main() ---
int main(int Ai_argc, char **Aps_argv) 
{

	DWORD ndevs;
  DWORD err;
	DWORD nresult;
	SGDeviceList* devList;
	SGDeviceInfoParam device_info;
	char szFileName[20];
  BYTE *buffer = NULL;
	FILE * pFile;

  printf("\n-------------------------------------\n");
  printf(  "SecuGen Multiple Device Demonstration\n");
  printf(  "-------------------------------------\n");


  nresult = CreateSGFPMObject(&sgfplib);

  if (!sgfplib)
     return false;

  printf("\nCreateFPMObject returned: %ld\n",nresult);

  ///////////////////////////////////////////////
  // Init()
  err = sgfplib->Init(SG_DEV_AUTO);
  printf("Init(SG_DEV_AUTO) returned: %ld\n",err);

  printf("\nEnumerating devices ...\n");
  err = sgfplib->EnumerateDevice(&ndevs, &devList);

  if (err != SGFDX_ERROR_NONE)
  {
     printf("EnumerateDevice: Failed : ErrorCode = %ld\n", err);
     return false;
  }

  for (int i=0; i<ndevs; ++i)
 	  printf("Device %ld: S/N %s\n",devList[i].DevID, devList[i].DevSN);

	printf("\n");

	for (int i=0; i<ndevs; ++i)
	{
		printf("########################################## \n");
		printf("Opening Device %d: \n", i);

		err = sgfplib->OpenDevice(i);
		if (err != SGFDX_ERROR_NONE)
			 printf("\tOpenDevice: Failed : ErrorCode = %ld\n", err);
		else
		{
			 printf("\tOpenDevice: Success\n");

			SGDeviceInfoParam device_info;
			err = sgfplib->GetDeviceInfo(&device_info);
			if (err == SGFDX_ERROR_NONE)
			{
				 printf("\tdevice_info.DeviceID   : %ld\n", device_info.DeviceID);
				 printf("\tdevice_info.DeviceSN   : %s\n",  device_info.DeviceSN);
				 printf("\tdevice_info.ComPort    : %ld\n", device_info.ComPort);
				 printf("\tdevice_info.ComSpeed   : %ld\n", device_info.ComSpeed);
				 printf("\tdevice_info.ImageWidth : %ld\n", device_info.ImageWidth);
				 printf("\tdevice_info.ImageHeight: %ld\n", device_info.ImageHeight);
				 printf("\tdevice_info.Contrast   : %ld\n", device_info.Contrast);
				 printf("\tdevice_info.Brightness : %ld\n", device_info.Brightness);
				 printf("\tdevice_info.Gain       : %ld\n", device_info.Gain);
				 printf("\tdevice_info.ImageDPI   : %ld\n", device_info.ImageDPI);
				 printf("\tdevice_info.FWVersion  : %04X\n", (unsigned int)device_info.FWVersion);
			}
			else
				 printf("GetDeviceInfo: Failed : ErrorCode = %ld\n", err);

			err = sgfplib->SetLedOn(true);
			printf("Place finger on sensor %d [SN: %s] with LED on and press <ENTER>: ",i,device_info.DeviceSN);
			getc(stdin);
			err = sgfplib->SetLedOn(false);

			buffer = new BYTE [device_info.ImageWidth*device_info.ImageHeight*sizeof (BYTE)];

			printf("\tCapturing ...\n");
			err = sgfplib->GetImage(buffer);
			if (err != SGFDX_ERROR_NONE)
				 printf("\tGetImage: Failed : ErrorCode = %ld\n", err);
			else
			{
				printf("\tGetImage: Success\n");
				sprintf(szFileName,"multidev_%d.raw",i);
				printf("\tWriting raw image data for sensor %d to %s\n",i,szFileName);
				pFile = fopen(szFileName,"wb");
				fwrite (buffer , sizeof (BYTE) , device_info.ImageWidth*device_info.ImageHeight , pFile);
				fclose (pFile);
			}

			delete buffer;
      
			err = sgfplib->CloseDevice();
			if (err != SGFDX_ERROR_NONE)
				 printf("\tCloseDevice: Failed : ErrorCode = %ld\n", err);
			else
				 printf("\tCloseDevice: Success\n");

			printf("\n");
		}

	}


    ///////////////////////////////////////////////
    // Destroy SGFPLib object
    //strcpy(function,"DestroySGFPMObject()");
    //printf("\nCall %s\n",function);
    err = DestroySGFPMObject(sgfplib);
    //printf("%s returned: %ld\n",function,err);
/*		
    free(imageBuffer1);
    free(imageBuffer2);
    free(minutiaeBuffer1);
    free(minutiaeBuffer2);
    free(ansiMinutiaeBuffer1);
    free(ansiMinutiaeBuffer2);
    free(isoMinutiaeBuffer1);
    free(isoMinutiaeBuffer2);
    free(finger);
    imageBuffer1 = NULL;
    imageBuffer2 = NULL;
    minutiaeBuffer1 = NULL; 
    minutiaeBuffer2 = NULL; 
    ansiMinutiaeBuffer1 = NULL; 
    ansiMinutiaeBuffer2 = NULL; 
    isoMinutiaeBuffer1 = NULL; 
    isoMinutiaeBuffer2 = NULL; 
    finger = NULL;		
*/

  return 0;
}
