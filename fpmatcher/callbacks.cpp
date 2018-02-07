/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher callbacks.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/
#include <gtk/gtk.h>
#include <string.h>
#include "callbacks.h"
#include "interface.h"
#include "configure.h"
extern GtkWidget* ConfigDlg;

// Local Data
LPSGFPM   g_Fpm = (LPSGFPM)NULL;
DWORD   m_ComSpeed = 0, m_Comport=0, m_ImageWidth=0, m_ImageHeight=0;
DWORD   m_nBrightness = 40, m_nContrast = 40, m_nGain = 2;
int     m_inputmode = -1;
int     m_SecurityL = SL_NORMAL;
bool    bRegistered = false;
DWORD     m_MatchingScore = 0;
BOOL g_EnableSmartCapture = TRUE;

// Image buffers
BYTE m_ucImgBuf[3][MAX_WIDTH*MAX_HEIGHT] = {{0},{0},{0}} ;

// Minutia buffer
BYTE m_ucFeatBuf[3][400] = {{0},{0},{0}} ;

// Pixmap for drawing
GdkPixmap       *RegPix = (GdkPixmap*)NULL, *MatchPix = (GdkPixmap*)NULL;

// Toobar widget
extern  GtkWidget *icon[5];

extern  GtkWidget *cb_SecurityLevel;
extern  GList *cb_SecurityLevel_items;
extern  GtkWidget *en_SecurityLevel;
extern  GtkWidget *cb_UserList;
extern  GList *cb_UserList_items;
extern  GtkWidget *en_UserList;
extern    GtkWidget *label2;

// StatusBar widget
extern  GtkWidget *memo;

// Button Widget
extern  GtkWidget *RegisterBtn;
extern  GtkWidget *MatchBtn;

extern  GtkWidget *frame1;
extern  GtkWidget *frame2;

// Drawing Area
extern  GtkWidget *RegView;
extern  GtkWidget *MatchView;

// Security Lists
extern gchar *szSecurityLevels[];

// definition of colors
enum __CL_LIST__ {
        clRed = 1,
        clBlue,
        clGreen,
        clYellow,
        clPurple,
        clOrange,
        clCyan,
        clBlack,
        clWhite
};
GdkColor        clList[] = { { 0, 0, 0, 0 },  // dummy
        { 0, 0xFFFF, 0, 0 },                    // red
        { 0, 0, 0, 0xFFFF },                    // blue
        { 0, 0, 0xFFFF, 0 },                    // green
        { 0, 0xFFFF, 0xFFFF, 0 },          // yellow
        { 0, 0xFFFF, 0, 0xFFFF },          // purple
        { 0, 0xFFFF, 0x9999 , 0 },          // orange
        { 0, 0, 0xFFFF, 0xFFFF },          // cyan
        { 0, 0, 0, 0 },                             // black
        { 0, 0xFFFF, 0xFFFF, 0xFFFF }  // white
};

//=============  Functinos of device access  =============
void log (char *szMsg)
{
#ifdef _DEBUG
   printf("DEBUG(FPMATCHER) : %s\n",szMsg);
#endif
}

bool InitDevice(void)
{
   DWORD err;

   log((char*)"In InitDevice");
   log((char*)"CreateSGFPMObject");
   CreateSGFPMObject(&g_Fpm);


   if (g_Fpm) 
   {
      err = g_Fpm->Init(SG_DEV_AUTO);
      if (err != SGFDX_ERROR_NONE)
      {
         fprintf(stderr,"Init: Failed : ErrorCode = %ld\n", err);
         return false;
      }

      log((char*)"OpenDevice");
      if (g_Fpm->OpenDevice(0) != SGFDX_ERROR_NONE)
      {
         DisplayMsg( memo, (char*)"Fail to Initialize...");
         return false;
      }
      log((char*)"SetBrightness");
      g_Fpm->SetBrightness(m_nBrightness);

      SGDeviceInfoParam device_info;
      g_Fpm->GetDeviceInfo(&device_info);
      m_ImageWidth = device_info.ImageWidth;
      m_ImageHeight = device_info.ImageHeight;
      m_ComSpeed = device_info.ComSpeed;
      m_Comport = device_info.ComPort;


      log((char*)"leaving InitDevice TRUE");
      return true;
   }
   log((char*)"leaving InitDevice FALSE");
   return false;
}

bool CaptureImage(int nMode)
{
   GtkWidget *widget = (GtkWidget*)NULL;
   GdkPixmap **pix = (GdkPixmap**)NULL;
   log((char*)"CaptureImage");
   if (!g_Fpm)
   {
      if(!InitDevice())
         return false;
   }
   log((char*)"CaptureImage : g_Fpm->GetImage()");
   if (g_Fpm->GetImage(m_ucImgBuf[nMode]) == SGFDX_ERROR_NONE) 
   {
      log((char*)"CaptureImage : g_Fpm->GetImage() returned ERROR_NONE");
      switch(nMode)
      {
      case FIRST_FINGER:
      case SECOND_FINGER:
         widget = RegView;
         pix = &RegPix;
         break;
                case MATCH_FINGER:
      default:
         widget = MatchView;
         pix = &MatchPix;
      }
      log((char*)"CaptureImage : DrawImage()");
      DrawImage( widget, 
                 (char*)m_ucImgBuf[nMode], 
                 pix, 
                 m_ImageWidth, 
                 m_ImageHeight);
      return true;
   }
   return false;
}

bool Register(void)
{
   bool bResult = false;
   BOOL matched = false;
   int result;

   if(!g_Fpm)
      return false;
   if (m_ucFeatBuf[FIRST_FINGER] && m_ucFeatBuf[SECOND_FINGER])
   {
      result = g_Fpm->MatchTemplate((BYTE*)m_ucFeatBuf[FIRST_FINGER] , 
                                    (BYTE*)m_ucFeatBuf[SECOND_FINGER], 
                                    m_SecurityL,
                                    &matched);
      if (result == SGFDX_ERROR_NONE && matched == true)
			{
         bResult = true;
         g_Fpm->GetMatchingScore(m_ucFeatBuf[FIRST_FINGER],m_ucFeatBuf[SECOND_FINGER],&m_MatchingScore);
			}
   }
   return bResult;
}

bool Matching(void)
{
   bool bMatched = false;
   BOOL matched = false;
   int result = g_Fpm->MatchTemplate(m_ucFeatBuf[FIRST_FINGER] , 
                                     m_ucFeatBuf[MATCH_FINGER], 
                                     m_SecurityL,
                                     &matched);
   if (result == SGFDX_ERROR_NONE && matched == true)
      bMatched = true;
   else
       result = g_Fpm->MatchTemplate(m_ucFeatBuf[SECOND_FINGER] , 
                                     m_ucFeatBuf[MATCH_FINGER], 
                                     m_SecurityL,
                                     &matched);


   if (result == SGFDX_ERROR_NONE && matched == true)
      bMatched = true;

   return bMatched;
}

void FingerprintCaptured(int nMode)
{
   int nResult = SGFDX_ERROR_NONE;
   char szBuffer[200];
   SGFingerInfo fingerInfo;

   CaptureImage(nMode);
   fingerInfo.FingerNumber = SG_FINGPOS_UK;
   fingerInfo.ViewNumber = 1;
   fingerInfo.ImpressionType = SG_IMPTYPE_LP;
   fingerInfo.ImageQuality = 99; //0 to 100

   nResult = g_Fpm->CreateTemplate(&fingerInfo, (BYTE*)m_ucImgBuf[nMode], (BYTE*)m_ucFeatBuf[nMode]);
   sprintf(szBuffer,"Finger %d GetMinutiae returned %d",nMode,nResult);
   log(szBuffer);

   if (nResult != SGFDX_ERROR_NONE )
   {
      char msg[100] = {0,};
      sprintf(msg,"Extraction error... code :%d", nResult);
      DisplayMsg(memo,msg);
      SetWidgetColor(frame1, clRed);
      return;
   }
   else            // Success
   {
      switch(nMode)
      {
      case FIRST_FINGER :
         DisplayMsg(memo, (char*)"Place your finger on the sensor again");
         m_inputmode = FIRST_FINGER;
#ifdef __GTK12
         gtk_label_set_text (GTK_LABEL(GTK_BIN(RegisterBtn)->child), (char*)"Register again");//GTK1.2
#else
         gtk_label_set_text (GTK_LABEL(GTK_BUTTON(RegisterBtn)->child),(char*)"Register again");
#endif
         gtk_widget_set_sensitive(icon[2], FALSE);
         gtk_widget_set_sensitive(MatchBtn, FALSE);
         SetWidgetColor(frame1, clGreen);
         SetWidgetColor(frame2 );
         break;
      case SECOND_FINGER :
         if(Register())
         {
            sprintf(szBuffer,"Fingerprint data were registered. Quality: %ld",m_MatchingScore);
            DisplayMsg(memo, szBuffer);
            //DisplayMsg(memo, "Fingerprint data were registered");
            m_inputmode = NONE;
            bRegistered = true;
#ifdef __GTK12
         gtk_label_set_text (GTK_LABEL(GTK_BIN(RegisterBtn)->child), (char*)"Register");//GTK1.2
#else
            gtk_label_set_text (GTK_LABEL(GTK_BUTTON(RegisterBtn)->child), (char*)"Register");
#endif
            gtk_widget_set_sensitive(icon[2], TRUE);
            gtk_widget_set_sensitive(MatchBtn, TRUE);
            SetWidgetColor(frame1, clGreen);
         }
         else
         {
            DisplayMsg(memo, (char*)"Fingerprint data were NOT registered");
            m_inputmode = FIRST_FINGER;
            SetWidgetColor(frame1, clRed);
         }
         break;
      case MATCH_FINGER :
         if(Matching())
         {
            DisplayMsg(memo, (char*)"****  Verification OK.  ****");
            SetWidgetColor(frame2, clGreen);
         }
         else
         {
            DisplayMsg(memo, (char*)"****  Failed.  ****");
            SetWidgetColor(frame2, clRed);
         }
         break;
      }
   }
}

void FormLoad(void)
{
   FILE* fpINIFile;
   if((fpINIFile =  fopen("./venus.ini","rt")) != NULL)
   {
      fscanf(fpINIFile, "%ld\n",&m_nBrightness);
      fscanf(fpINIFile, "%ld\n",&m_nContrast);
      fscanf(fpINIFile, "%ld\n",&m_nGain);
      fclose(fpINIFile);
   }
   else
   {
      m_nBrightness = 30;
      m_nContrast = 30;
      m_nGain = 1;
   }

   gtk_widget_set_sensitive(icon[2], FALSE);
}

void MatchDlgQuit()
{
   FILE* fpINIFile;
   if((fpINIFile =  fopen("./venus.ini","wt")) != NULL)
   {
      fprintf(fpINIFile, "%ld\n",m_nBrightness);
      fprintf(fpINIFile, "%ld\n",m_nContrast);
      fprintf(fpINIFile, "%ld\n",m_nGain);
      fclose(fpINIFile);
   }

   if(g_Fpm)
   {
      g_Fpm->CloseDevice();;
      g_Fpm = (LPSGFPM)NULL;
   }
   gtk_main_quit();
}

//=============  Definition of Common library  =============

void DisplayMsg( GtkWidget *widget, char *text)
{
   static  guint     context_id = 0;
   static  guint     message_id = 0;

   if( context_id == 0)
      context_id = gtk_statusbar_get_context_id ( GTK_STATUSBAR( widget), "");

   if( message_id != 0)
      gtk_statusbar_remove( GTK_STATUSBAR( widget), context_id, message_id);

   message_id = gtk_statusbar_push( GTK_STATUSBAR(widget), context_id, text);

   gtk_widget_draw (widget, (GdkRectangle *) &widget->allocation);
}

void DrawImage( GtkWidget *widget, \
                        char *buffer, \
                        GdkPixmap **img, \
                        gint m_ImageWidth, \
                        gint m_ImageHeight)
{
   GdkBitmap      *mask = (GdkBitmap*)NULL;
   GtkStyle        *style = new GtkStyle();
   GtkAllocation *rc = &widget->allocation;
   char szbuffer[200];
   gint d_x = 0, d_y = 0;
   log((char*)"Entering DrawImage()");
   sprintf(szbuffer,"rc->width:[%d] m_ImageWidth[%d]",rc->width, m_ImageWidth);
   log(szbuffer);
   if(rc->width > m_ImageWidth)
      d_x = (gint)( rc->width - m_ImageWidth ) / 2;

   sprintf(szbuffer,"rc->height:[%d] m_ImageHeight[%d]",rc->height, m_ImageHeight);
   log(szbuffer);
   if(rc->height > m_ImageHeight)
   {
      d_y = (gint)( rc->height - m_ImageHeight ) / 2;
   }

   log((char*)"Before CreateFingerorintBitmap()");
   gchar **pixmap_d = CreateFingerprintBitmap(m_ImageWidth, m_ImageHeight, 
                      buffer);
   log((char*)"Before gdk_pixmap_create_from_xpm_d()");
   *img = gdk_pixmap_create_from_xpm_d(widget->window, &mask, \
                         &style->bg[GTK_STATE_NORMAL], (gchar**)pixmap_d);
   log((char*)"Before FreeBarBitmap()");

   FreeBarBitmap(pixmap_d);
   log((char*)"Before gdk_draw_pixmap()");

   gdk_draw_pixmap( widget->window, 
        widget->style->fg_gc[GTK_WIDGET_STATE (widget)], 
        *img, 0, 0, d_x,d_y,m_ImageWidth, m_ImageHeight);

}

gchar ** CreateFingerprintBitmap( gint width,
                                  gint height,
                                  gpointer imgbuffer)
{
   char* img;
   img = (char*)imgbuffer;
   int    i, j, count=0;
   char   **sBitmap;
   char   buffer[800];

   sBitmap = (char**)g_malloc((height + 17 + 1 + 1) * sizeof(gchar *));

   sprintf( buffer, "%d %d 17 1",width, height);
   sBitmap[count++] = g_strdup(buffer);

   for(i=0 ; i < 16 ; i++)
   {
      sprintf( buffer, "%c \tc #%02X%02X%02X",'A'+i, i<<4, i<<4, i<<4);
      sBitmap[count++] = g_strdup(buffer);
   }
   sprintf( buffer, "%c \tc #FFFFFF",'A'+i);
   sBitmap[count++] = g_strdup(buffer);

   for(j = 0 ; j < height ; j++)
   {
      for(i = 0 ; i < width ; i++)
      {
          buffer[i]=((img[ j*width + i ] >> 4) & 15) + 'A';
      }
      buffer[i] = '\0';
      sBitmap[count++] = g_strdup(buffer);
   }
   sBitmap[count] = (char*)NULL;
   return(sBitmap);
}

void FreeBarBitmap(char **bitmap)
{
   if(!bitmap)
      return;
   int i=0;
   while(bitmap[i] != NULL)
   {
      g_free(bitmap[i]);
      i++;
      if(i>318)
      break;
   }
   g_free(bitmap);
}

int GetIndexFromCombo(GtkWidget *wEntry, gchar **pszList, int nMax)
{
   int i;
   gchar *szWidgetText = (gchar*)NULL;
   szWidgetText = (gchar*) gtk_entry_get_text(GTK_ENTRY(wEntry));

   for( i = 0 ; i < nMax ; i++)
   {
      if(strcmp(pszList[i], szWidgetText)  == 0)
         break;
   }
   if(nMax > i )
      return i;
   else
      return -1;      // not found
}

void SetWidgetColor(GtkWidget *widget, int fg, int text, int bg, int base)
{
   GtkStyle *defstyle, *style;
   defstyle = gtk_widget_get_default_style();
   style = gtk_style_copy(defstyle);
   for(int i =0 ; i<5 ;i++)
   {
      if(fg)
      {
          style->fg[i] = clList[fg];
          style->text[i] = clList[fg];
          style->bg[i] = clList[fg];
          style->base[i] = clList[fg];
      }
      if(text)
         style->text[i] = clList[text];
      if(bg)
         style->bg[i] = clList[bg];
      if(base)
         style->base[i] = clList[base];
   }
   gtk_widget_set_style(widget,style);
}

//=============  Definition of Signal  =============

void icon0_event (GtkWidget *widget, gpointer data)
{
   InitDevice();
}

void icon1_event (GtkWidget *widget, gpointer data)     // Register
{
   if(m_inputmode == NONE)
   {
      m_inputmode = FIRST_FINGER;
      FingerprintCaptured(m_inputmode);
   }
   else if(m_inputmode == FIRST_FINGER)
   {
      m_inputmode = SECOND_FINGER;
      FingerprintCaptured(m_inputmode);
      m_inputmode = NONE;
   }
}

void icon2_event (GtkWidget *widget, gpointer data)     // Matching
{
   if(m_inputmode == NONE && bRegistered)
   {
      FingerprintCaptured(MATCH_FINGER);
   }
   m_inputmode = NONE;
}

void icon3_event (GtkWidget *widget, gpointer data)     // Configure
{
   if (!g_Fpm)
   {
      if(!InitDevice())
          return ;
    }
    ConfigDlg = create_ConfigDlg ();
    gtk_widget_show (ConfigDlg);
}

void icon4_event (GtkWidget *widget, gpointer data)     // Close
{
   MatchDlgQuit();
}

gboolean on_MatchDlg_delete_event (GtkWidget   *widget,
                                  GdkEvent    *event,
                                  gpointer    user_data)
{
   MatchDlgQuit();
   return FALSE;
}

void on_RegisterBtn_clicked  (GtkButton  *button,
                              gpointer  user_data)
{
   if(m_inputmode == NONE)
   {
      m_inputmode = FIRST_FINGER;
      FingerprintCaptured(m_inputmode);
   }
   else if(m_inputmode == FIRST_FINGER)
   {
      m_inputmode = SECOND_FINGER;
      FingerprintCaptured(m_inputmode);
      m_inputmode = NONE;
   }
}

void on_MatchBtn_clicked (GtkButton       *button,
                          gpointer         user_data)
{
   if (!g_Fpm)
   {
      if(!InitDevice())
          return ;
   }
   if(m_inputmode == NONE && bRegistered)
   {
      m_inputmode = MATCH_FINGER;
      FingerprintCaptured(m_inputmode);
   }
   m_inputmode = NONE;
}

void on_en_SecurityLevel_changed (GtkEditable     *editable,
                                  gpointer         user_data)
{
   m_SecurityL = GetIndexFromCombo( en_SecurityLevel,  
                                    szSecurityLevels, MAX_SECURITY);
   return;
}

/* Create a new backing pixmap of the appropriate size */
gboolean on_RegView_configure_event  (GtkWidget       *widget,
                                     GdkEventConfigure *event,
                                     gpointer         user_data)
{
   if (RegPix)
   {
      gdk_pixmap_unref(RegPix);

      RegPix = gdk_pixmap_new(widget->window,
                              widget->allocation.width,
                              widget->allocation.height,
                              -1);
  }
  return TRUE;
}

gboolean on_MatchView_configure_event (GtkWidget       *widget,
                                       GdkEventConfigure *event,
                                       gpointer         user_data)
{
   if (MatchPix)
   {
      gdk_pixmap_unref(MatchPix);

      MatchPix = gdk_pixmap_new(widget->window,
                                widget->allocation.width,
                                widget->allocation.height,
                                -1);
  }
  return TRUE;
}

/* Redraw the screen from the backing pixmap */
gboolean on_RegView_expose_event (GtkWidget       *widget,
                                  GdkEventExpose  *event,
                                  gpointer         user_data)
{
   if (RegPix)
   {
      gdk_draw_pixmap(widget->window,
                   widget->style->bg_gc[GTK_STATE_NORMAL],
                   RegPix,
                   event->area.x, event->area.y,
                   event->area.x, event->area.y,
                   event->area.width, event->area.height);
   }
   return FALSE;
}

gboolean on_MatchView_expose_event (GtkWidget       *widget,
                                    GdkEventExpose  *event,
                                    gpointer         user_data)
{
   if (MatchPix)
   {
      gdk_draw_pixmap(widget->window,
                      widget->style->bg_gc[GTK_STATE_NORMAL],
                      MatchPix,
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height);
  }
  return FALSE;
}
