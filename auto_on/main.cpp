/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FDU04 Auto On  main.cpp source code module
 * Copyright(c): 2009 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "sgfplib.h"

int   msg_qid;
key_t key;
struct msgbuf qbuf;


LPSGFPM  sgfplib = NULL;

bool StartAutoOn(LPSGFPM m_sgfplib)
{
  DWORD result;
  bool StartAutoOn = false;
  //////////////////////////////////////////////////////////////////////////
  // Start Message Queue ///////////////////////////////////////////////////
  // Create unique key via call to ftok() 

  printf("Create unique message key\n");
  key = ftok(".", 'a'); //'a' is an arbitrary seed value
  // Open the queue - create if necessary 

  if((msg_qid = msgget(key, IPC_CREAT|0660)) == -1)
    return false;

  printf("Message Queue ID is : %d\n",msg_qid);
  // Start Message Queue ///////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  // EnableAutoOnEvent(true)
  printf("Call sgfplib->EnableAutoOnEvent(true) ... \n");  
  result = m_sgfplib->EnableAutoOnEvent(true,&msg_qid,NULL);
  printf("sgfplib->EnableAutoOnEvent()  returned ... ");  
  if (result != SGFDX_ERROR_NONE)
  {
     printf("FAIL - [%ld]\n",result);  
  }
  else
  {
     StartAutoOn = true;
     printf("SUCCESS - [%ld]\n",result);  
  }
  printf(".............................................................\n");  
  return StartAutoOn;
}

bool StopAutoOn(LPSGFPM m_sgfplib)
{
  DWORD result;
  bool StopAutoOn = false;
  //////////////////////////////////////////////////////////////////////////
  // EnableAutoOnEvent(false)
  printf("Calling ISensor::EnableAutoOnEvent(false) ... \n");  
  result = m_sgfplib->EnableAutoOnEvent(false,&msg_qid,NULL);
  printf("sgfplib->EnableAutoOnEvent(false)  returned ... ");  
  if (result != SGFDX_ERROR_NONE)
  {
     printf("FAIL - [%ld]\n",result);  
  }
  else
  {
     StopAutoOn = true;
     printf("SUCCESS - [%ld]\n",result);  
  }
  printf(".............................................................\n");  

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // Remove Message Queue //////////////////////////////////////////////////
  msgctl(msg_qid, IPC_RMID, 0);
  // Remove Message Queue //////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  return StopAutoOn;

}

long fingerPresent()
{
   int fingerPresent = 0;
   printf("Reading message queue ...\n");

#ifdef _FDU06
 #if defined(_XOPEN_SOURCE)
   #if defined(LINUX3)
           qbuf.mtype = FDU06_MSG;
   #else
           qbuf._mtype = FDU06_MSG;
   #endif
 #else
           qbuf.mtype = FDU06_MSG;
 #endif
#endif
#ifdef _FDU05
 #if defined(_XOPEN_SOURCE)
   #if defined(LINUX3)
           qbuf.mtype = FDU05_MSG;
   #else
           qbuf._mtype = FDU05_MSG;
   #endif
 #else
           qbuf.mtype = FDU05_MSG;
 #endif
#endif
#ifdef _FDU04
 #if defined(_XOPEN_SOURCE)
    #if defined(LINUX3)
           qbuf.mtype = FDU04_MSG;
    #else
           qbuf._mtype = FDU04_MSG;
    #endif
 #else
           qbuf.mtype = FDU04_MSG;
 #endif
#endif
#ifdef _FDU03
 #if defined(_XOPEN_SOURCE)
    #if defined(LINUX3)
            qbuf.mtype = FDU03_MSG;
    #else
            qbuf._mtype = FDU03_MSG;
    #endif
 #else
            qbuf.mtype = FDU03_MSG;
 #endif
#endif

#if defined(_XOPEN_SOURCE)
    #if defined(LINUX3)
	   msgrcv(msg_qid, (struct msgbuf *)&qbuf, MAX_SEND_SIZE, qbuf.mtype, 0);
	   printf("Type: %ld Text: %s\n", qbuf.mtype, qbuf.mtext);
	   if (strlen(qbuf.mtext) > 0)
	   {
	      fingerPresent= atol(qbuf.mtext);
	   }
    #else
	   msgrcv(msg_qid, (struct msgbuf *)&qbuf, MAX_SEND_SIZE, qbuf._mtype, 0);
	   printf("Type: %ld Text: %s\n", qbuf._mtype, qbuf._mtext);
	   if (strlen(qbuf._mtext) > 0)
	   {
	      fingerPresent= atol(qbuf._mtext);
	   }
   #endif
#else
   msgrcv(msg_qid, (struct msgbuf *)&qbuf, MAX_SEND_SIZE, qbuf.mtype, 0);
   printf("Type: %ld Text: %s\n", qbuf.mtype, qbuf.mtext);
   if (strlen(qbuf.mtext) > 0)
   {
      fingerPresent= atol(qbuf.mtext);
   }
#endif
   return fingerPresent;
}


// ---------------------------------------------------------------- main() ---
int main(int argc, char **argv) 
{

     long err;
     BYTE* imageBuffer1;
     int   msg_qid;
     SGDeviceInfoParam deviceInfo;


     printf("\n-------------------------------------\n");
     printf(  "SecuGen Auto-On Test\n");
     printf(  "-------------------------------------\n");

     ///////////////////////////////////////////////
     // Instantiate SGFPLib object
     err = CreateSGFPMObject(&sgfplib);
     if (!sgfplib)
     {
     	printf("ERROR - Unable to instantiate FPM object\n");
     	return false;
     }
     printf("CreateSGFPMObject returned: %ld\n",err);

	
     if (err == SGFDX_ERROR_NONE)
     {

        ///////////////////////////////////////////////
        // Init()
        printf("Call sgfplib->Init(SG_DEV_AUTO)\n");
        err = sgfplib->Init(SG_DEV_AUTO);
        printf("Init returned: %ld\n",err);

        ///////////////////////////////////////////////
        // OpenDevice()
        printf("Call sgfplib->OpenDevice(0)\n");
        err = sgfplib->OpenDevice(0);
        printf("OpenDevice returned : [%ld]\n\n",err);

        ///////////////////////////////////////////////
        // getDeviceInfo()
        err = sgfplib->GetDeviceInfo(&deviceInfo);
        printf("GetDeviceInfo returned: %ld\n\n",err);

 	imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageWidth*deviceInfo.ImageHeight);

        if (StartAutoOn(sgfplib))
        {  
         while (1)
         {
           if (fingerPresent())
           {
             printf("Finger Present\n");
	     if (!StopAutoOn(sgfplib))
             {
                 printf("StopAutoOn() returned false.\n");
                 break;                
             }
             printf("Call ISensor::GetImage()\n");
             err = sgfplib->GetImage(imageBuffer1);
             printf("ISensor::GetImage() returned ... ");  
             if (err != SGFDX_ERROR_NONE)
             {
                printf("FAIL - [%ld]\n",err);
             }
             else
             {
                printf("SUCCESS - [%ld]\n",err);
                FILE *fp = fopen("test_auto_on_finger.raw","wb");
                fwrite (imageBuffer1 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
                fclose(fp);
                fp = NULL;
             }
             printf(".............................................................\n");
             printf("Press 'X' to exit, any other key to continue >> ");
             if (getc(stdin) == 'X')
               break;
  	     if(!StartAutoOn(sgfplib))
             {
                 printf("StartAutoOn() returned false.\n");
                 break;
             }
           }
         }
        }

        //////////////////////////////////////////////////////////////////////////
        // EnableAutoOnEvent(false)
        printf("Call sgfplib->EnableAutoOnEvent(false) ... \n");
        err = sgfplib->EnableAutoOnEvent(false,&msg_qid,NULL);
        printf("EnableAutoOnEvent returned : [%ld]\n", err);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        // Remove Message Queue //////////////////////////////////////////////////
        msgctl(msg_qid, IPC_RMID, 0);
        // Remove Message Queue //////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////
        // closeDevice()
        printf("\nCall fplib->CloseDevice()\n");
        err = sgfplib->CloseDevice();
        printf("CloseDevice returned : [%ld]\n",err);

        ///////////////////////////////////////////////
        // Destroy FPLib object
        printf("\nCall DestroySGFPMObject(fplib)\n");
        err = DestroySGFPMObject(sgfplib);
        printf("DestroySGFPMObject returned : [%ld]\n",err);

        free(imageBuffer1);
        imageBuffer1 = NULL;
		
     }
     return 0;
}
