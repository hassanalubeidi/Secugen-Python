/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher configure.h source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include <gtk/gtk.h>
#include "sgfplib.h"

GtkWidget* create_ConfigDlg (void);
void EnableAllControls(BOOL bEnable);
void on_PreviewBtn_clicked (GtkButton *button, gpointer user_data);
void on_CancelBtn_clicked (GtkButton *button, gpointer user_data);
void on_OKBtn_clicked (GtkButton *button, gpointer user_data);
void on_ConfigDlg_Load(void);





