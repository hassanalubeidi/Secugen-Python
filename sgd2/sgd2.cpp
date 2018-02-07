/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : SGD2 sgd.cpp source code module
 * Copyright(c): 2009 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/
#include <gtk/gtk.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgfplib.h"

LPSGFPM           g_Fpm = (LPSGFPM)NULL;

// External functions
long int  getclock();
void      DrawImage( GtkWidget *preview1, char *buffer, GdkPixmap **pixmap, gint m_ImageWidth, gint m_ImageHeight);
gchar     **CreateFingerprintBitmap(gint width, gint height, gpointer img);
void      FreeBarBitmap(char **bitmap);
void      DisplayMsg( GtkWidget *widget, char *text);

GtkWidget *CapDlg;
GtkWidget *vbox1;
GtkWidget *frame1;
GtkWidget *preview1;
GtkWidget *hbuttonbox1;
GtkWidget *InitBtn;
GtkWidget *CaptureBtn;
GtkWidget *StartBtn;
GtkWidget *TestBtn; // jkang
GtkWidget *memo;
GtkPreview *Panel1;

BYTE *buffer = (BYTE*)NULL;
DWORD           m_ComSpeed = 0, m_Comport=0, m_ImageWidth=0, m_ImageHeight=0;
DWORD          m_nBrightness = 50, m_nContrast = 30, m_nGain = 1;
GdkPixmap       *pixmap = (GdkPixmap*)NULL;
static gint timer = 0;
static int bTimerRunning = FALSE;
static BOOL g_EnableSmartCapture = TRUE;

// jkang, to display capture time 
/*long int getclock()
{
   //Time capture
   static long int n=0;
   int hour,min, sec,milisec;
   long int ldTimeOffset;
   struct tm when;
   time_t now, result;
   struct timeb timebuffer;
   char *timeline;
   time( &now );
   when = *localtime( &now );
   hour = when.tm_hour;
   min = when.tm_min;
   sec = when.tm_sec;

   ftime( &timebuffer ); 
   timeline = ctime( & ( timebuffer.time ) );
   milisec = timebuffer.millitm; 
   ldTimeOffset = ((((hour * 60) +min) * 60) + sec)  *1000  + milisec;

   return ldTimeOffset;
}
*/

bool InitDevice(void)
{
   char szMsg[128] = {0,};
   DWORD err;

   CreateSGFPMObject(&g_Fpm);

   if (!g_Fpm)
      return false;

   err = g_Fpm->Init(SG_DEV_AUTO);

   if (err != SGFDX_ERROR_NONE)
   {
      fprintf(stderr,"Init: Failed : ErrorCode = %ld\n", err);
      return false;
   }

   err = g_Fpm->OpenDevice(0);

   if (err != SGFDX_ERROR_NONE)
   {
      fprintf(stderr,"OpenDevice: Failed : ErrorCode = %ld\n", err);
      return false;
   }

   g_Fpm->SetBrightness(m_nBrightness);

   SGDeviceInfoParam device_info;
   err = g_Fpm->GetDeviceInfo(&device_info);
   if (err == SGFDX_ERROR_NONE)
   {
      m_ImageWidth = device_info.ImageWidth;
      m_ImageHeight = device_info.ImageHeight;
      m_ComSpeed = device_info.ComSpeed;
      m_Comport = device_info.ComPort;
   }
   else
   {
      fprintf(stderr,"GetDeviceInfo: Failed : ErrorCode = %ld\n", err);
      return false;
   }

   return true;
}


bool CaptureImage(void)
{
   DWORD err;


   if (!g_Fpm)
   {
      fprintf(stderr,"Cannot find FPM Object\n");
      return false;
   }

   buffer = new BYTE [m_ImageWidth*m_ImageHeight*sizeof (BYTE)];
   err = g_Fpm->GetImage(buffer);

   if (err != SGFDX_ERROR_NONE)
   {
      if (err != SGFDX_ERROR_WRONG_IMAGE)
         fprintf(stderr,"GetImage(): Failed : ErrorCode = %ld\n", err);
      return false;
   }


   DrawImage( preview1, (char*)buffer, &pixmap, m_ImageWidth, m_ImageHeight);

   return true;

}


gint TimerCallback(gpointer data)
{
   gtk_timeout_remove(timer);
   bTimerRunning = FALSE;
   int nResult = 0;

   if(!g_Fpm)
      nResult = InitDevice();
   if(!nResult)
      CaptureImage();

   timer = gtk_timeout_add(10, TimerCallback, NULL);
   bTimerRunning = TRUE;

   return 0;
}

void on_InitBtn_clicked(GtkButton   *button, gpointer   user_data)
{
   char message[30];
   if (InitDevice())
   {
      g_Fpm->WriteData(5, g_EnableSmartCapture); // 1: Enable, 0:Disable
      sprintf(message,"Initialization Success:");
      if (g_EnableSmartCapture)
         strcat(message," - SC ON");
      else
         strcat(message," - SC OFF");
         
      DisplayMsg(memo, (char*)message);
   }
   else
      DisplayMsg(memo, (char*)"Initialization failed");

}


void on_CaptureBtn_clicked(GtkButton *button, gpointer user_data)
{
   int start_millisec, end_millisec;
   start_millisec = getclock();

   if (CaptureImage())
   {
      char temp[100];
      end_millisec = getclock();
      sprintf(temp, "Capture Time=%d", end_millisec - start_millisec);
      if (g_EnableSmartCapture)
         strcat(temp," - SC ON");
      else
         strcat(temp," - SC OFF");

      DisplayMsg(memo, temp);
   }
   else
      DisplayMsg(memo, (char*)"Capture failed");

}


void on_StartBtn_clicked(GtkButton *button, gpointer user_data)
{
   if (g_EnableSmartCapture)
        DisplayMsg(memo, (char*)"SC ON");
   else
        DisplayMsg(memo, (char*)"SC OFF");

   if(!bTimerRunning)
   {
      timer = gtk_timeout_add(10, TimerCallback, NULL);
      bTimerRunning = TRUE;
#ifdef __GTK12
      gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child),"Stop");//GTK 1.2
#else
      gtk_label_set_text(GTK_LABEL(GTK_BUTTON(button)->child),"Stop");
#endif
   }
   else
   {
      gtk_timeout_remove(timer);
      bTimerRunning = FALSE;
#ifdef __GTK12
      gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child),"Continous");//GTK 1.2
#else
      gtk_label_set_text(GTK_LABEL(GTK_BUTTON(button)->child),"Continous");
#endif
   }
}

// jkang, For SmartCapture Test
void on_TestBtn_clicked(GtkButton *button, gpointer user_data)
{ 
   if (!g_Fpm)
   {
      fprintf(stderr,"Cannot find FPM Object\n");
      return;
   }

   
   g_EnableSmartCapture = !g_EnableSmartCapture;

   g_Fpm->WriteData(5, g_EnableSmartCapture); // 1: Enable, 0:Disable

   if (g_EnableSmartCapture)
       DisplayMsg(memo, (char*)"SmartCapure Enabled");
   else
       DisplayMsg(memo, (char*)"SmartCapure Disabled");

}


/* Create a new backing pixmap of the appropriate size */
static gint configure_event( GtkWidget         *widget,
                             GdkEventConfigure *event )
{
   if (pixmap)
      gdk_pixmap_unref(pixmap);

   pixmap = gdk_pixmap_new(widget->window,
                           widget->allocation.width,
                           widget->allocation.height,
                           -1);
   gdk_draw_rectangle (pixmap,
                       widget->style->bg_gc[GTK_STATE_NORMAL],//white_gc,
                       TRUE,
                       0, 0,
                       widget->allocation.width,
                       widget->allocation.height);

   return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gint expose_event( GtkWidget      *widget,
                          GdkEventExpose *event )
{
   gdk_draw_pixmap(widget->window,
   widget->style->bg_gc[GTK_STATE_NORMAL],//fg_gc[GTK_WIDGET_STATE (widget)],
                  pixmap,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height);

   return FALSE;
}


gboolean on_CapDlg_delete_event(GtkWidget *widget,
                                GdkEvent  *event,
                                gpointer  user_data)
{
   if(bTimerRunning)
   {
      gtk_timeout_remove(timer);
      bTimerRunning = FALSE;
   }

   if (buffer)
      delete [] buffer;
   buffer = 0;

   if(g_Fpm)
      delete g_Fpm;//DestroyISensor( g_Sensor);
   g_Fpm = (LPSGFPM)NULL;

   gtk_exit(0);

   return FALSE;
}


GtkWidget* create_CapDlg (void)
{

   CapDlg = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_name (CapDlg, "CapDlg");
   gtk_object_set_data (GTK_OBJECT (CapDlg), "CapDlg", CapDlg);
#ifdef __SOLARIS
   gtk_window_set_title (GTK_WINDOW (CapDlg), "SecuGen SUN Solaris USB");
#else
   gtk_window_set_title (GTK_WINDOW (CapDlg), "SecuGen Linux USB");
#endif

   vbox1 = gtk_vbox_new (FALSE, 0);
   gtk_widget_set_name (vbox1, "vbox1");
   gtk_widget_ref (vbox1);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox1);
   gtk_container_add (GTK_CONTAINER (CapDlg), vbox1);

   //----------------------
   frame1 = gtk_frame_new ((gchar *)NULL);
   gtk_widget_set_name (frame1, "frame1");
   gtk_widget_ref (frame1);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (frame1);
   gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (frame1), 2);
   gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_ETCHED_OUT);

   preview1 = gtk_drawing_area_new ();
   gtk_widget_set_name (preview1, "preview1");
   gtk_widget_ref (preview1);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "preview1", preview1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (preview1);
   gtk_container_add (GTK_CONTAINER (frame1), preview1);
   gtk_widget_set_usize (preview1, 260, 300);

   //----------------------
   hbuttonbox1 = gtk_hbutton_box_new ();
   gtk_widget_set_name (hbuttonbox1, "hbuttonbox1");
   gtk_widget_ref (hbuttonbox1);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "hbuttonbox1", hbuttonbox1,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hbuttonbox1);
   gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, TRUE, TRUE, 0);
   gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);
   gtk_button_box_set_child_size (GTK_BUTTON_BOX (hbuttonbox1), 0, 0);
   gtk_button_box_set_child_ipadding (GTK_BUTTON_BOX (hbuttonbox1), 0, 0);

   InitBtn = gtk_button_new_with_label ("Init");
   gtk_widget_set_name (InitBtn, "InitBtn");
   gtk_widget_ref (InitBtn);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "InitBtn", InitBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (InitBtn);
   gtk_container_add (GTK_CONTAINER (hbuttonbox1), InitBtn);
   GTK_WIDGET_SET_FLAGS (InitBtn, GTK_CAN_DEFAULT);

   CaptureBtn = gtk_button_new_with_label ("Capture");
   gtk_widget_set_name (CaptureBtn, "CaptureBtn");
   gtk_widget_ref (CaptureBtn);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "CaptureBtn", CaptureBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (CaptureBtn);
   gtk_container_add (GTK_CONTAINER (hbuttonbox1), CaptureBtn);
   GTK_WIDGET_SET_FLAGS (CaptureBtn, GTK_CAN_DEFAULT);

   StartBtn = gtk_button_new_with_label ("Continous");
   gtk_widget_set_name (StartBtn, "StartBtn");
   gtk_widget_ref (StartBtn);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "StartBtn", StartBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (StartBtn);
   gtk_container_add (GTK_CONTAINER (hbuttonbox1), StartBtn);
   GTK_WIDGET_SET_FLAGS (StartBtn, GTK_CAN_DEFAULT);

   // jkang
   TestBtn = gtk_button_new_with_label ("SmartCap");//Test");
   gtk_widget_set_name (TestBtn, "TestBtn");
   gtk_widget_ref (TestBtn);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "TestBtn", TestBtn,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (TestBtn);
   gtk_container_add (GTK_CONTAINER (hbuttonbox1), TestBtn);
   GTK_WIDGET_SET_FLAGS (TestBtn, GTK_CAN_DEFAULT);
   // jkang
 
   memo = gtk_statusbar_new ();
   gtk_widget_set_name (memo, "memo");
   gtk_widget_ref (memo);
   gtk_object_set_data_full (GTK_OBJECT (CapDlg), "memo", memo,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (memo);
   gtk_box_pack_start (GTK_BOX (vbox1), memo, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (memo), 1);

/* Signals used to handle backing pixmap */
   gtk_signal_connect (GTK_OBJECT (preview1), "expose_event",
                       (GtkSignalFunc) expose_event, NULL);

   gtk_signal_connect (GTK_OBJECT(preview1),"configure_event",
                       (GtkSignalFunc) configure_event, NULL);

   gtk_signal_connect (GTK_OBJECT (CapDlg), "delete_event",
                       GTK_SIGNAL_FUNC (on_CapDlg_delete_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (InitBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_InitBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (CaptureBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_CaptureBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (StartBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_StartBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (TestBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_TestBtn_clicked),
                       NULL);


   return CapDlg;
}

int main (int argc, char *argv[])
{
   GtkWidget *CapDlg;

   gtk_set_locale ();
   gtk_init (&argc, &argv);

   CapDlg = create_CapDlg ();
   gtk_widget_show (CapDlg);

   gtk_main ();
   return 0;
}
