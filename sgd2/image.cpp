/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : SGD2 image.cpp source code module
 * Copyright(c): 2004 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/
#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <sys/timeb.h>

long int getclock();
void msg(char *str);
void DrawImage( GtkWidget *widget, char *buffer, GdkPixmap **pixmap, gint m_ImageWidth, gint m_ImageHeight);
gchar **CreateFingerprintBitmap(gint width, gint height, gpointer img);
void    FreeBarBitmap(char **bitmap);
void DisplayMsg( GtkWidget *widget, char *text);

void msg(char *str)
{
   static long int n=0;
   FILE* fp;
   if(n)
      fp = fopen("./log.txt","a+t");
   else
      fp = fopen("./log.txt","wt");
   if( fp != NULL)
   {
     fprintf(fp,"%ld : %s\n",n++,str);
     fclose(fp);
   }
   return ;
}

long int getclock()
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
                        GdkPixmap **pixmap, \
                        gint m_ImageWidth, \
                        gint m_ImageHeight)
{
   GdkBitmap      *mask = (GdkBitmap*)NULL;
   GtkStyle        *style = new GtkStyle();
   GtkAllocation *rc = &widget->allocation;
   gint d_x = 0, d_y = 0;
   if(rc->width > m_ImageWidth)
      d_x = (gint)( rc->width - m_ImageWidth ) / 2;

   if(rc->height > m_ImageHeight)
      d_y = (gint)( rc->height - m_ImageHeight ) / 2;

   gchar **pixmap_d = CreateFingerprintBitmap(m_ImageWidth, 
                                              m_ImageHeight, buffer);
   *pixmap = gdk_pixmap_create_from_xpm_d(widget->window, &mask, \
                                         &style->bg[GTK_STATE_NORMAL], 
                                         (gchar**)pixmap_d);

   FreeBarBitmap(pixmap_d);

   gdk_draw_pixmap( widget->window, 
                    widget->style->fg_gc[GTK_WIDGET_STATE (widget)], 
                    *pixmap, 0, 0, d_x,d_y,m_ImageWidth, m_ImageHeight);

}

gchar ** CreateFingerprintBitmap( gint width, gint height, gpointer imgbuffer)
{
   char* img = (char*)imgbuffer;
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
         buffer[i]=((img[ j*width + i ] >> 4) & 15) + 'A';

      buffer[i] = '\0';
      sBitmap[count++] = g_strdup(buffer);
   }
   sBitmap[count] = NULL;
   return(sBitmap);
}

void  FreeBarBitmap(char **bitmap)
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
