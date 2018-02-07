/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher main.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include <gtk/gtk.h>
#include "interface.h"

int main (int argc, char *argv[])
{
   GtkWidget *MatchDlg;

   gtk_set_locale ();
   gtk_init (&argc, &argv);

   MatchDlg = create_MatchDlg ();
   gtk_widget_show (MatchDlg);

   gtk_main ();
   return 0;
}

