/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher configure.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/
#include "configure.h"
#include "callbacks.h"
#include <math.h>

GtkWidget *ConfigDlg;
GdkPixmap *pixAdj = (GdkPixmap*)NULL;
BYTE      m_AdjImg[MAX_WIDTH*MAX_HEIGHT] = {0};
BOOL      bFDP02 = FALSE;

extern LPSGFPM  g_Fpm;
extern DWORD  m_ComSpeed, m_Comport, m_ImageWidth, m_ImageHeight;
extern DWORD  m_nBrightness, m_nContrast, m_nGain;
extern BOOL g_EnableSmartCapture;

GtkWidget *PreView;
GtkWidget *hsBrightness;
GtkWidget *hsContrast;
GtkWidget *hsGain;
GtkAdjustment *adjBr, *adjCont, *adjGain;
GtkWidget *labelBr;
GtkWidget *labelCont;
GtkWidget *labelGain;
GtkWidget *PreviewBtn;
GtkWidget *OKBtn;
GtkWidget *CancelBtn;
GtkWidget *progressbar1;
GtkAdjustment *adjPgrbar;
GtkWidget *AdjustBtn;
GtkWidget *TestBtn; // jkang


void UpdateSecuGenDevice()
{
   if (g_Fpm)
   {
      g_Fpm->SetLedOn(true);
      g_Fpm->SetBrightness((guint) adjBr->value);
      g_Fpm->SetLedOn(false);
   }
}

void EnableAllControls(BOOL bEnable)
{
   gtk_widget_set_sensitive(labelBr, bEnable);
   gtk_widget_set_sensitive(labelCont, bEnable);
   gtk_widget_set_sensitive(labelGain, bEnable);

   gtk_widget_set_sensitive(PreviewBtn, bEnable);
   gtk_widget_set_sensitive(OKBtn, bEnable);
   gtk_widget_set_sensitive(CancelBtn, bEnable);
}

void on_PreviewBtn_clicked (GtkButton *button,
                            gpointer  user_data)
{
   if (!g_Fpm)
   {
      if(!InitDevice())
         return ;
   }
   UpdateSecuGenDevice();
   if (g_Fpm->GetImage(m_AdjImg) == SGFDX_ERROR_NONE) 
   {
      DrawImage( PreView, (char*)m_AdjImg, &pixAdj, 
                 m_ImageWidth, m_ImageHeight);
   }
   return ;
}


void on_CancelBtn_clicked (GtkButton *button,
                           gpointer  user_data)
{
   if (g_Fpm)
   {
      g_Fpm->SetBrightness(m_nBrightness);
      g_Fpm->SetLedOn(false);
   }
   gtk_grab_remove(ConfigDlg);
   gtk_widget_destroy (ConfigDlg);
}

void on_OKBtn_clicked (GtkButton *button,
                       gpointer  user_data)
{
   UpdateSecuGenDevice();
   m_nBrightness = (DWORD) adjBr->value;
   m_nContrast = (DWORD) adjCont->value;
   m_nGain = 1 << (DWORD) adjGain->value;
   gtk_grab_remove(ConfigDlg);
   gtk_widget_destroy (ConfigDlg);
}

void on_ConfigDlg_Load(void)
{
   SGDeviceInfoParam device_info;
   g_Fpm->GetDeviceInfo(&device_info);
   if( device_info.ImageHeight == 300 )
   {
      bFDP02 = TRUE;
      gtk_widget_set_sensitive(hsContrast, TRUE);
      gtk_widget_set_sensitive(labelCont, TRUE);
      gtk_widget_set_sensitive(hsGain, TRUE);
      gtk_widget_set_sensitive(labelGain, TRUE);
   }
   pixAdj = (GdkPixmap*)NULL;
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
#ifdef __GTK12
      gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child),"SmartCap On");//GTK 1.2
#else
      gtk_label_set_text(GTK_LABEL(GTK_BUTTON(button)->child),"SmartCap On");
#endif

//    DisplayMsg(memo, (char*)"SmartCapure Enabled");
   else
#ifdef __GTK12
      gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child),"SmartCap Off");//GTK 1.2
#else
      gtk_label_set_text(GTK_LABEL(GTK_BUTTON(button)->child),"SmartCap Off");
#endif
//    DisplayMsg(memo, (char*)"SmartCapure Disabled");

}


/* Create a new backing pixmap of the appropriate size */
gboolean on_PreView_configure_event  (GtkWidget       *widget,
                                      GdkEventConfigure *event,
                                      gpointer         user_data)
{
   if (pixAdj)
   {
      gdk_pixmap_unref(pixAdj);

      pixAdj = gdk_pixmap_new(widget->window,
                              widget->allocation.width,
                              widget->allocation.height,
                              -1);
   }
   return TRUE;
}

/* Redraw the screen from the backing pixmap */
gboolean
on_PreView_expose_event                (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  if (pixAdj){
  gdk_draw_pixmap(widget->window,
                  widget->style->bg_gc[GTK_STATE_NORMAL],
                  pixAdj,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height);
  }
  return FALSE;
}


GtkWidget* create_ConfigDlg (void)
{

   GtkWidget *hbox1;
   GtkWidget *frame1;
   GtkWidget *vbox1;
   GtkWidget *frame2;
   GtkWidget *table1;
   GtkWidget *table2;

   ConfigDlg = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_name (ConfigDlg, "ConfigDlg");
   gtk_object_set_data (GTK_OBJECT (ConfigDlg), "ConfigDlg", ConfigDlg);
   gtk_window_set_title (GTK_WINDOW (ConfigDlg), "Device Configuration");
   gtk_window_set_modal(GTK_WINDOW (ConfigDlg), TRUE);

   hbox1 = gtk_hbox_new (FALSE, 0);
   gtk_widget_set_name (hbox1, "hbox1");
   gtk_widget_ref (hbox1);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hbox1);
   gtk_container_add (GTK_CONTAINER (ConfigDlg), hbox1);

   frame1 = gtk_frame_new ((const gchar*)NULL);
   gtk_widget_set_name (frame1, "frame1");
   gtk_widget_ref (frame1);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (frame1);
   gtk_box_pack_start (GTK_BOX (hbox1), frame1, TRUE, TRUE, 0);

   PreView = gtk_drawing_area_new ();
   gtk_widget_set_name (PreView, "PreView");
   gtk_widget_ref (PreView);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "PreView", PreView,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (PreView);
   gtk_container_add (GTK_CONTAINER (frame1), PreView);
   gtk_widget_set_usize (PreView, 260, 300);

   vbox1 = gtk_vbox_new (FALSE, 0);
   gtk_widget_set_name (vbox1, "vbox1");
   gtk_widget_ref (vbox1);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox1);
   gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

   frame2 = gtk_frame_new ((const gchar*)NULL);
   gtk_widget_set_name (frame2, "frame2");
   gtk_widget_ref (frame2);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (frame2);
   gtk_box_pack_start (GTK_BOX (vbox1), frame2, TRUE, TRUE, 0);

   table1 = gtk_table_new (3, 2, FALSE);
   gtk_widget_set_name (table1, "table1");
   gtk_widget_ref (table1);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (table1);
   gtk_container_add (GTK_CONTAINER (frame2), table1);
   gtk_table_set_col_spacings (GTK_TABLE (table1), 10);

   adjBr =  GTK_ADJUSTMENT(gtk_adjustment_new (m_nBrightness, 
                           0, 100, 10, 10, 0));
   hsBrightness = gtk_hscale_new (adjBr);
   gtk_widget_set_name (hsBrightness, "hsBrightness");
   gtk_widget_ref (hsBrightness);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "hsBrightness", 
                            hsBrightness,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hsBrightness);
   gtk_table_attach (GTK_TABLE (table1), hsBrightness, 1, 2, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
   gtk_scale_set_value_pos (GTK_SCALE (hsBrightness), GTK_POS_BOTTOM);
   gtk_scale_set_digits (GTK_SCALE (hsBrightness), 0);
   gtk_range_set_update_policy (GTK_RANGE (hsBrightness),
                                  GTK_UPDATE_DELAYED);

   adjCont =  GTK_ADJUSTMENT(gtk_adjustment_new (m_nContrast, 
                             0, 100, 10, 10, 0));
   hsContrast = gtk_hscale_new (adjCont);
   gtk_widget_set_name (hsContrast, "hsContrast");
   gtk_widget_ref (hsContrast);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "hsContrast", hsContrast,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hsContrast);
   gtk_table_attach (GTK_TABLE (table1), hsContrast, 1, 2, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
   gtk_scale_set_value_pos (GTK_SCALE (hsContrast), GTK_POS_BOTTOM);
   gtk_scale_set_digits (GTK_SCALE (hsContrast), 0);
   gtk_range_set_update_policy (GTK_RANGE (hsContrast),
                                  GTK_UPDATE_DELAYED);
//   int i = 0;
//   for( i=0;i<4;i++)
//   {
//      if((int)m_nGain == 1 << i)
//         break;
//   }
   if (m_nGain < 1)
     m_nGain = 1;
   if (m_nGain > 2)
     m_nGain = 2;
   adjGain =  GTK_ADJUSTMENT(gtk_adjustment_new (m_nGain , 1, 2, 1, 1, 0));
   hsGain = gtk_hscale_new (adjGain);
   gtk_widget_set_name (hsGain, "hsGain");
   gtk_widget_ref (hsGain);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "hsGain", hsGain,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hsGain);
   gtk_table_attach (GTK_TABLE (table1), hsGain, 1, 2, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
   gtk_scale_set_value_pos (GTK_SCALE (hsGain), GTK_POS_BOTTOM);
   gtk_scale_set_digits (GTK_SCALE (hsGain), 0);
   gtk_range_set_update_policy (GTK_RANGE (hsGain),
                                   GTK_UPDATE_DELAYED);

   labelBr = gtk_label_new ("Brightness");
   gtk_widget_set_name (labelBr, "labelBr");
   gtk_widget_ref (labelBr);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "labelBr", labelBr,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (labelBr);
   gtk_table_attach (GTK_TABLE (table1), labelBr, 0, 1, 0, 1,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);
   gtk_widget_set_usize (labelBr, 100, -2);

   labelCont = gtk_label_new ("Contrast");
   gtk_widget_set_name (labelCont, "labelCont");
   gtk_widget_ref (labelCont);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "labelCont", labelCont,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (labelCont);
   gtk_table_attach (GTK_TABLE (table1), labelCont, 0, 1, 1, 2,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);

   labelGain = gtk_label_new ("Gain");
   gtk_widget_set_name (labelGain, "labelGain");
   gtk_widget_ref (labelGain);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "labelGain", labelGain,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (labelGain);
   gtk_table_attach (GTK_TABLE (table1), labelGain, 0, 1, 2, 3,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);

   /* Create a Adjusment object to hold the range of the
    * progress bar */
   adjPgrbar = (GtkAdjustment *) gtk_adjustment_new (0, 0, 100, 0, 0, 0);

   progressbar1 = gtk_progress_bar_new_with_adjustment (adjPgrbar);
   gtk_widget_set_name (progressbar1, "progressbar1");
   gtk_widget_ref (progressbar1);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "progressbar1", 
                             progressbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (progressbar1);
   gtk_box_pack_start (GTK_BOX (vbox1), progressbar1, FALSE, FALSE, 0);
   gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR (progressbar1),
                                                 GTK_PROGRESS_DISCRETE);


   table2 = gtk_table_new (2, 2, FALSE);
   gtk_widget_set_name (table2, "table2");
   gtk_widget_ref (table2);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "table2", table2,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (table2);
   gtk_box_pack_start (GTK_BOX (vbox1), table2, TRUE, TRUE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (table2), 10);
   gtk_table_set_row_spacings (GTK_TABLE (table2), 10);
   gtk_table_set_col_spacings (GTK_TABLE (table2), 10);

   PreviewBtn = gtk_button_new_with_label ("Preview");
   gtk_widget_set_name (PreviewBtn, "PreviewBtn");
   gtk_widget_ref (PreviewBtn);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "PreviewBtn", PreviewBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (PreviewBtn);
   gtk_table_attach (GTK_TABLE (table2), PreviewBtn, 0, 1, 0, 1,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);
   gtk_widget_set_usize (PreviewBtn, 100, 25);

   OKBtn = gtk_button_new_with_label ("OK");
   gtk_widget_set_name (OKBtn, "OKBtn");
   gtk_widget_ref (OKBtn);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "OKBtn", OKBtn,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (OKBtn);
   gtk_table_attach (GTK_TABLE (table2), OKBtn, 1, 2, 0, 1,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);
   gtk_widget_set_usize (OKBtn, 100, 25);
   //GTK_WIDGET_SET_FLAGS (OKBtn, GTK_CAN_DEFAULT);

 // jkang

   if (g_EnableSmartCapture)
     TestBtn = gtk_button_new_with_label ("SmartCap On");//Test");
   else
     TestBtn = gtk_button_new_with_label ("SmartCap Off");//Test");
   gtk_widget_set_name (TestBtn, "TestBtn");
   gtk_widget_ref (TestBtn);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "TestBtn", TestBtn,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (TestBtn);
   //gtk_container_add (GTK_CONTAINER (hbuttonbox1), TestBtn);
   GTK_WIDGET_SET_FLAGS (TestBtn, GTK_CAN_DEFAULT);
    gtk_table_attach (GTK_TABLE (table2), TestBtn, 0, 1, 1, 2,
                     (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
   gtk_widget_set_usize (TestBtn, 100, 25);
// jkang



   CancelBtn = gtk_button_new_with_label ("Cancel");
   gtk_widget_set_name (CancelBtn, "CancelBtn");
   gtk_widget_ref (CancelBtn);
   gtk_object_set_data_full (GTK_OBJECT (ConfigDlg), "CancelBtn", CancelBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (CancelBtn);

   gtk_table_attach (GTK_TABLE (table2), CancelBtn, 1, 2, 1, 2,
                     (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
   gtk_widget_set_usize (CancelBtn, 100, 25);
   //GTK_WIDGET_SET_FLAGS (CancelBtn, GTK_CAN_DEFAULT);

   //gtk_widget_grab_default(CancelBtn);

   gtk_grab_add(ConfigDlg);

   gtk_signal_connect (GTK_OBJECT (PreviewBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_PreviewBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (OKBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_OKBtn_clicked),
                      NULL);
   gtk_signal_connect (GTK_OBJECT (CancelBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_CancelBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT(PreView),"configure_event",
                       GTK_SIGNAL_FUNC (on_PreView_configure_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (PreView), "expose_event",
                       GTK_SIGNAL_FUNC (on_PreView_expose_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (TestBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_TestBtn_clicked),
                       NULL);
   on_ConfigDlg_Load();

   return ConfigDlg;
}


