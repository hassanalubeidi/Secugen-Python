/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPMatcher interface.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include <gtk/gtklist.h>

#define        _(x)     x
#ifndef NULL
#define NULL    0
#endif

#include "callbacks.h"
#include "interface.h"
#include "./pixmaps/register.xpm"
#include "./pixmaps/match.xpm"
#include "./pixmaps/config.xpm"
#include "./pixmaps/close.xpm"

// Toobar widget
GtkWidget *icon[5];

GtkWidget *cb_SecurityLevel;
GList     *cb_SecurityLevel_items = 0;
GtkWidget *en_SecurityLevel;
GtkWidget *cb_UserList;
GList     *cb_UserList_items = 0;
GtkWidget *en_UserList;
GtkWidget *label2;

// StatusBar widget
GtkWidget *memo;

// Button Widget
GtkWidget *RegisterBtn;
GtkWidget *MatchBtn;

GtkWidget *frame1;
GtkWidget *frame2;

// Drawing Area
GtkWidget *RegView;
GtkWidget *MatchView;

gchar *szSecurityLevels[MAX_SECURITY] = { 
	(char*) "",
        (char*) "Lowest",
        (char*) "Lower",
        (char*) "Low",
        (char*) "Below Normal",
        (char*) "Normal",
        (char*) "Above Normal",
        (char*) "High",
        (char*) "Higher",
        (char*) "Highest",
};


//============      Definition of Dialog      ============//
GtkWidget* create_MatchDlg (void)
{
   GtkWidget *MatchDlg;
   GtkWidget *vbox1;
   GtkWidget *hbox1;
   GtkWidget *toolbar1;
   GtkWidget *hbox2;
   GtkWidget *vbox2;
   GtkWidget *vbox3;
   GtkWidget *vbox4;
#ifdef __USER_LIST__
   GtkWidget *vbox5;
#endif
   GtkWidget *label1;

   // Temporary widget
   GdkPixmap * iconPix[5];
   GdkBitmap * mask[5];
   GtkWidget * iconw[5];
  
   MatchDlg = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_object_set_data (GTK_OBJECT (MatchDlg), "MatchDlg", MatchDlg);
#ifdef __SOLARIS
   gtk_window_set_title (GTK_WINDOW (MatchDlg), 
       ("SecuGen SUN Solaris USB Fingerprint Matcher"));
#else
   gtk_window_set_title (GTK_WINDOW (MatchDlg), 
       ("SecuGen Linux USB Fingerprint Matcher"));
#endif

   gtk_window_set_position(GTK_WINDOW (MatchDlg), GTK_WIN_POS_CENTER);
   gtk_widget_show(MatchDlg);

   vbox1 = gtk_vbox_new (FALSE, 0);
   gtk_widget_ref (vbox1);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "vbox1", vbox1,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox1);
   gtk_container_add (GTK_CONTAINER (MatchDlg), vbox1);

   hbox1 = gtk_hbox_new (FALSE, 0);
   gtk_widget_ref (hbox1);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hbox1);
   gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

#ifdef __GTK12
   toolbar1 = gtk_toolbar_new (); //GTK1.2
#else
   toolbar1 = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH);
#endif
   gtk_widget_ref (toolbar1);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "toolbar1", toolbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (toolbar1);
   gtk_box_pack_start (GTK_BOX (hbox1), toolbar1, FALSE, FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (toolbar1), 5);

  /*----       Create Tool Bar        ----*/
//#define __INIT_BTN__
#ifdef __INIT_BTN__
   // Init Button
   iconPix[0] = gdk_pixmap_create_from_xpm_d ( MatchDlg->window, &mask[0], \
                   &MatchDlg->style->white, register_xpm);
   iconw[0] = gtk_pixmap_new ( iconPix[0], mask[0] );    /* icon widget */
   icon[0] = gtk_toolbar_append_item ( GTK_TOOLBAR (toolbar1), /* our toolbar */
                          "Initialize",        /* button label */
                          "Initialize Device", /* this button's tooltip */
                          "Private",           /* tooltip private info */
                          iconw[0],            /* icon widget */
                          GTK_SIGNAL_FUNC (icon0_event), /* a signal */
                          NULL );
   gtk_toolbar_append_space ( GTK_TOOLBAR ( toolbar1 ) ); /* space after item */
#endif  // __INIT_BTN__


   // Register Button
   iconPix[1] = gdk_pixmap_create_from_xpm_d ( MatchDlg->window, &mask[1],  
                   &MatchDlg->style->white, register_xpm);
   iconw[1] = gtk_pixmap_new ( iconPix[1], mask[1] );         /* icon widget */
   icon[1] = gtk_toolbar_append_item ( GTK_TOOLBAR (toolbar1),/* our toolbar */
                          "Register",              /* button label */
                          "Register fingerprints", /* this button's tooltip */
                          "Private",               /* tooltip private info */
                          iconw[1],                /* icon widget */
                          GTK_SIGNAL_FUNC (icon1_event), /* a signal */
                          NULL );
   gtk_toolbar_append_space ( GTK_TOOLBAR ( toolbar1 ) ); /* space after item */

   // Matching Button
   iconPix[2] = gdk_pixmap_create_from_xpm_d ( MatchDlg->window, &mask[2], 
                   &MatchDlg->style->white, match_xpm);
   iconw[2] = gtk_pixmap_new ( iconPix[2], mask[2] );         /* icon widget */
   icon[2] = gtk_toolbar_append_item ( GTK_TOOLBAR (toolbar1),/* our toolbar */
                          "Match",                 /* button label */
                          "Match fingerprints",    /* this button's tooltip */
                          "Private",               /* tooltip private info */
                          iconw[2],                /* icon widget */
                          GTK_SIGNAL_FUNC (icon2_event), /* a signal */
                          NULL );
   gtk_toolbar_append_space ( GTK_TOOLBAR ( toolbar1 ) ); /* space after item */

   // Config Button
   iconPix[3] = gdk_pixmap_create_from_xpm_d ( MatchDlg->window, &mask[3], \
                   &MatchDlg->style->white, config_xpm);
   iconw[3] = gtk_pixmap_new ( iconPix[3], mask[3] );        /* icon widget */
   icon[3] = gtk_toolbar_append_item ( GTK_TOOLBAR (toolbar1),/* our toolbar */
                          "Config...",                       /* button label */
                          "Hardware configuration", /* this button's tooltip */
                          "Private",                /* tooltip private info */
                          iconw[3],                 /* icon widget */
                          GTK_SIGNAL_FUNC (icon3_event), /* a signal */
                          NULL );
   gtk_toolbar_append_space ( GTK_TOOLBAR ( toolbar1 ) ); /* space after item */

   // Close Button
   iconPix[4] = gdk_pixmap_create_from_xpm_d ( MatchDlg->window, &mask[4], \
                   &MatchDlg->style->white, close_xpm);
   iconw[4] = gtk_pixmap_new ( iconPix[4], mask[4] );     /* icon widget */
   icon[4] = gtk_toolbar_append_item ( GTK_TOOLBAR (toolbar1),/* our toolbar */
                          "Close",              /* button label */
                          "Close program",      /* this button's tooltip */
                          "Private",            /* tooltip private info */
                          iconw[4],             /* icon widget */
                          GTK_SIGNAL_FUNC (icon4_event), /* a signal */
                          NULL );
   gtk_toolbar_append_space ( GTK_TOOLBAR ( toolbar1 ) ); /* space after item */


   //  Security level
   vbox4 = gtk_vbox_new (FALSE, 0);
   gtk_widget_set_name (vbox4, "vbox4");
   gtk_widget_ref (vbox4);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "vbox4", vbox4,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox4);
   gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar1), vbox4, (const char*)NULL, (const char*)NULL);

   label1 = gtk_label_new ("Security Level");
   gtk_widget_set_name (label1, "label1");
   gtk_widget_ref (label1);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "label1", label1,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (label1);
   gtk_box_pack_start (GTK_BOX (vbox4), label1, FALSE, FALSE, 4);

   cb_SecurityLevel = gtk_combo_new ();
   gtk_widget_ref (cb_SecurityLevel);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "cb_SecurityLevel", 
                             cb_SecurityLevel,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (cb_SecurityLevel);
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[1]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[2]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[3]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[4]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[5]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[6]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[7]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[8]));
   cb_SecurityLevel_items = g_list_append (cb_SecurityLevel_items, 
                                          (void *)(szSecurityLevels[9]));
   gtk_widget_set_usize (cb_SecurityLevel, 105, -2);
   gtk_combo_set_value_in_list (GTK_COMBO (cb_SecurityLevel), TRUE, TRUE);
   gtk_combo_set_popdown_strings (GTK_COMBO (cb_SecurityLevel), 
                                          cb_SecurityLevel_items);
   g_list_free (cb_SecurityLevel_items);

   en_SecurityLevel = GTK_COMBO (cb_SecurityLevel)->entry;
   gtk_widget_ref (en_SecurityLevel);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "en_SecurityLevel", 
                             en_SecurityLevel,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (en_SecurityLevel);
   gtk_entry_set_editable (GTK_ENTRY (en_SecurityLevel), FALSE);
   gtk_entry_set_text (GTK_ENTRY (en_SecurityLevel), szSecurityLevels[5]);

   gtk_signal_connect (GTK_OBJECT (en_SecurityLevel), "changed",
                       GTK_SIGNAL_FUNC (on_en_SecurityLevel_changed),
                       NULL);
   gtk_box_pack_start (GTK_BOX (vbox4), cb_SecurityLevel, FALSE, FALSE, 0);

#ifdef __USER_LIST__

   vbox5 = gtk_vbox_new (FALSE, 0);
   gtk_widget_set_name (vbox5, "vbox5");
   gtk_widget_ref (vbox5);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "vbox5", vbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox5);
   gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar1), vbox5, NULL, NULL);

   label2 = gtk_label_new ("User Name");
   gtk_widget_set_name (label2, "label2");
   gtk_widget_ref (label2);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "label2", label2,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (label2);
   gtk_box_pack_start (GTK_BOX (vbox5), label2, FALSE, FALSE, 4);

   //  User List
   cb_UserList = gtk_combo_new ();
   gtk_widget_ref (cb_UserList);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "cb_UserList", cb_UserList,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_set_usize (cb_UserList, 120, -2);
   gtk_widget_show (cb_UserList);

   en_UserList = GTK_COMBO (cb_UserList)->entry;
   gtk_widget_ref (en_UserList);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "en_UserList", en_UserList,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (en_UserList);
   gtk_box_pack_start (GTK_BOX (vbox5), cb_UserList, FALSE, FALSE, 0);
#endif          //__USER_LIST__
   /*----       Create  Tool Bar       ----*/


   hbox2 = gtk_hbox_new (FALSE, 0);
   gtk_widget_ref (hbox2);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (hbox2);
   gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);

   vbox2 = gtk_vbox_new (FALSE, 0);
   gtk_widget_ref (vbox2);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox2);
   gtk_box_pack_start (GTK_BOX (hbox2), vbox2, TRUE, TRUE, 0);

   RegisterBtn = gtk_button_new_with_label (_("Register"));
   gtk_widget_ref (RegisterBtn);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "RegisterBtn", RegisterBtn,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (RegisterBtn);
   gtk_box_pack_start (GTK_BOX (vbox2), RegisterBtn, FALSE, FALSE, 0);

   frame1 = gtk_frame_new ((const gchar*)NULL);
   gtk_widget_ref (frame1);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "frame1", frame1,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (frame1);
   gtk_box_pack_start (GTK_BOX (vbox2), frame1, TRUE, TRUE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (frame1), 2);
   gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_ETCHED_OUT);

   RegView = gtk_drawing_area_new ();
   gtk_widget_ref (RegView);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "RegView", RegView,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (RegView);
   gtk_container_add (GTK_CONTAINER (frame1), RegView);
   gtk_widget_set_usize (RegView, 260, 300);

   vbox3 = gtk_vbox_new (FALSE, 0);
   gtk_widget_ref (vbox3);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "vbox3", vbox3,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (vbox3);
   gtk_box_pack_start (GTK_BOX (hbox2), vbox3, TRUE, TRUE, 0);

   MatchBtn = gtk_button_new_with_label (_("Matching"));
   gtk_widget_ref (MatchBtn);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "MatchBtn", MatchBtn,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (MatchBtn);
   gtk_box_pack_start (GTK_BOX (vbox3), MatchBtn, FALSE, FALSE, 0);

   frame2 = gtk_frame_new ((const gchar*)NULL);
   gtk_widget_ref (frame2);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "frame2", frame2,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (frame2);
   gtk_box_pack_start (GTK_BOX (vbox3), frame2, TRUE, TRUE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (frame2), 2);
   gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_ETCHED_OUT);

   MatchView = gtk_drawing_area_new ();
   gtk_widget_ref (MatchView);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "MatchView", MatchView,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (MatchView);
   gtk_container_add (GTK_CONTAINER (frame2), MatchView);
   gtk_widget_set_usize (MatchView, 260, 300);

   memo = gtk_statusbar_new ();
   gtk_widget_ref (memo);
   gtk_object_set_data_full (GTK_OBJECT (MatchDlg), "memo", memo,
                             (GtkDestroyNotify) gtk_widget_unref);
   gtk_widget_show (memo);
   gtk_box_pack_start (GTK_BOX (vbox1), memo, FALSE, FALSE, 0);

   //================     Definition of Signal     ====================
   gtk_signal_connect (GTK_OBJECT (MatchDlg), "delete_event",
                       GTK_SIGNAL_FUNC (on_MatchDlg_delete_event),
                      NULL);
   gtk_signal_connect (GTK_OBJECT (RegisterBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_RegisterBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (MatchBtn), "clicked",
                       GTK_SIGNAL_FUNC (on_MatchBtn_clicked),
                       NULL);
   gtk_signal_connect (GTK_OBJECT(RegView),"configure_event",
                       GTK_SIGNAL_FUNC (on_RegView_configure_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (RegView), "expose_event",
                       GTK_SIGNAL_FUNC (on_RegView_expose_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT(MatchView),"configure_event",
                       GTK_SIGNAL_FUNC (on_MatchView_configure_event),
                       NULL);
   gtk_signal_connect (GTK_OBJECT (MatchView), "expose_event",
                       GTK_SIGNAL_FUNC (on_MatchView_expose_event),
                       NULL);

   FormLoad();

   return MatchDlg;
}

