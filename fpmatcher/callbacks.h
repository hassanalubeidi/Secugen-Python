/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher callbacks.h source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/
#include <gtk/gtk.h>
#include "sgfplib.h"

#define  MAX_WIDTH      400
#define  MAX_HEIGHT     400

// Status of Capturing
#define NONE            -1
#define FIRST_FINGER     0
#define SECOND_FINGER    1
#define MATCH_FINGER     2

// Status of Panels
#define PANEL_NORMAL     0
#define PANEL_CLICK      1
#define PANEL_CAPTURING  2
#define PANEL_CAPTURED   3
#define PANEL_READY      4

#define MAX_SECURITY     10

//=============  Functinos of device access  =============
bool InitDevice(void);
bool CaptureImage(int );
bool Register(void);
void FingerprintCaptured(int );
int GetIndexFromCombo(GtkWidget *, gchar **, int );
bool Matching(void);
void FormLoad(void);

//=============  Definition of Common library  =============
// External functions
void DrawImage( GtkWidget *preview1, char *buffer, GdkPixmap **pixmap, gint m_ImageWidth, gint m_ImageHeight);
gchar **CreateFingerprintBitmap(gint width, gint height, gpointer img);
void DisplayMsg( GtkWidget *widget, char *text);
void SetWidgetColor(GtkWidget *widget, int fg = 0, int text = 0, int bg = 0, int base = 0);

// Internal Only
void    FreeBarBitmap(char **bitmap);

//=============  Definition of Signal  =============
void icon0_event (GtkWidget *widget, gpointer data);
void icon1_event (GtkWidget *widget, gpointer data);
void icon2_event (GtkWidget *widget, gpointer data);
void icon3_event (GtkWidget *widget, gpointer data);
void icon4_event (GtkWidget *widget, gpointer data);

gboolean
on_MatchDlg_delete_event (GtkWidget       *widget, \
                                        GdkEvent        *event, \
                                        gpointer         user_data);
void
on_RegisterBtn_clicked (GtkButton       *button, \
                                   gpointer         user_data);

void
on_MatchBtn_clicked (GtkButton       *button, \
                                   gpointer         user_data);
void
on_en_SecurityLevel_changed (GtkEditable     *editable, \
                                        gpointer         user_data);

gboolean
on_RegView_configure_event (GtkWidget       *widget, \
                                        GdkEventConfigure *event, \
                                        gpointer         user_data);
gboolean
on_MatchView_configure_event (GtkWidget       *widget, \
                                        GdkEventConfigure *event, \
                                        gpointer         user_data);
gboolean
on_RegView_expose_event (GtkWidget       *widget, \
                                        GdkEventExpose  *event, \
                                        gpointer         user_data);
gboolean
on_MatchView_expose_event (GtkWidget       *widget, \
                                        GdkEventExpose  *event, \
                                        gpointer         user_data);

