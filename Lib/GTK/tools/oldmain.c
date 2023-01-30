#include <gtk/gtk.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <pthread.h>

void RefreshDirectory(LPTSTR);
void RefreshTree(LPTSTR);
void WatchDirectory(LPTSTR);

GObject *global_window;
GtkWidget  *global_tmp_window;
GtkApplication *app;
pthread_mutex_t lock;

void mymain(int argc, TCHAR *argv[])
{
    // if(argc != 2)
    // {
    //     _tprintf(TEXT("Usage: %s <dir>\n"), argv[0]);
    //     return;
    // }

    char command[1024];
    while (1)
    {
        printf("\nsh# ");
        gets(command);

        if(strcmp(command, "ss") == 0)
        {
            g_signal_emit_by_name  (app, "activate");
        }
    }
    

    // WatchDirectory("C:\\Users\\shino\\Desktop\\CodeNote\\Lib\\GTK\\tools");
}

void *check_file_change(void * arg) 
{
    mymain(0, NULL);
}

void WatchDirectory(LPTSTR lpDir)
{
   DWORD dwWaitStatus; 
   HANDLE dwChangeHandles[2]; 
   TCHAR lpDrive[4];
   TCHAR lpFile[_MAX_FNAME];
   TCHAR lpExt[_MAX_EXT];

   _tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

   lpDrive[2] = (TCHAR)'\\';
   lpDrive[3] = (TCHAR)'\0';
 
// Watch the directory for file creation and deletion. 
 
   dwChangeHandles[0] = FindFirstChangeNotification( 
      lpDir,                         // directory to watch 
      TRUE,                         // do not watch subtree 
      FILE_NOTIFY_CHANGE_SIZE); // watch file name changes 
 
   if (dwChangeHandles[0] == INVALID_HANDLE_VALUE) 
   {
     printf("\n ERROR: FindFirstChangeNotification function failed.\n");
     ExitProcess(GetLastError()); 
   }
 
// Watch the subtree for directory creation and deletion. 
 
   dwChangeHandles[1] = FindFirstChangeNotification( 
      lpDrive,                       // directory to watch 
      TRUE,                          // watch the subtree 
      FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes 
 
   if (dwChangeHandles[1] == INVALID_HANDLE_VALUE) 
   {
     printf("\n ERROR: FindFirstChangeNotification function failed.\n");
     ExitProcess(GetLastError()); 
   }
 

// Make a final validation check on our handles.

   if ((dwChangeHandles[0] == NULL) || (dwChangeHandles[1] == NULL))
   {
     printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
     ExitProcess(GetLastError()); 
   }

// Change notification is set. Now wait on both notification 
// handles and refresh accordingly. 
 
   while (TRUE) 
   { 
   // Wait for notification.
 
      printf("\nWaiting for notification...\n");

      dwWaitStatus = WaitForMultipleObjects(1, dwChangeHandles, 
         FALSE, INFINITE); 
 
      switch (dwWaitStatus) 
      { 
         case WAIT_OBJECT_0: 
 
         // A file was created, renamed, or deleted in the directory.
         // Refresh this directory and restart the notification.
            
             RefreshDirectory(lpDir); 

             if ( FindNextChangeNotification(dwChangeHandles[0]) == FALSE )
             {
               printf("\n ERROR: FindNextChangeNotification function failed.\n");
               ExitProcess(GetLastError()); 
             }

             printf("Send !!!\n");
            //  g_signal_emit_by_name  (global_window, "close_request");
            g_signal_emit_by_name  (app, "activate");
            Sleep(1000);

             break; 
 
        //  case WAIT_OBJECT_0 + 1: 
 
        //  // A directory was created, renamed, or deleted.
        //  // Refresh the tree and restart the notification.
 
        //      RefreshTree(lpDrive); 
        //      if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE )
        //      {
        //        printf("\n ERROR: FindNextChangeNotification function failed.\n");
        //        ExitProcess(GetLastError()); 
        //      }
        //      break; 
 
         case WAIT_TIMEOUT:

         // A timeout occurred, this would happen if some value other 
         // than INFINITE is used in the Wait call and no changes occur.
         // In a single-threaded environment you might not want an
         // INFINITE wait.
 
            printf("\nNo changes in the timeout period.\n");
            break;

         default: 
            printf("\n ERROR: Unhandled dwWaitStatus.\n");
            ExitProcess(GetLastError());
            break;
      }
   }
}

void RefreshDirectory(LPTSTR lpDir)
{
   // This is where you might place code to refresh your
   // directory listing, but not the subtree because it
   // would not be necessary.

   _tprintf(TEXT("Directory (%s) changed.\n"), lpDir);
}

void RefreshTree(LPTSTR lpDrive)
{
   // This is where you might place code to refresh your
   // directory listing, including the subtree.

   _tprintf(TEXT("Directory tree (%s) changed.\n"), lpDrive);
}


static char css_file[256] = "builder.css";
static char ui_file[256] = "builder.ui";

static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data);

static void quit_cb(GtkWindow *window)
{
    printf("take\n");
    pthread_mutex_lock(&lock);
    if(global_window)
    {
        printf(">>>>>>>>>>>>>>>>>> EXIT !!!!!!!!!!!!!!!!!!!!!!!!!\n");
        // gtk_window_close(GTK_WINDOW(global_window));
        gtk_application_remove_window(app, GTK_WINDOW(global_window));
        gtk_window_present(GTK_WINDOW(global_tmp_window));
        g_signal_emit_by_name  (app, "activate");
        global_window = NULL;
    }

    g_signal_emit_by_name  (app, "activate");
    pthread_mutex_unlock(&lock);
    printf("give\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    printf(">>>>>>>>>>>>>>>>>> activate !!!!!!!!!!!!!!!!!!!!!!!!!\n");

    printf("take\n");
    pthread_mutex_lock(&lock);

    if(global_tmp_window == NULL)
    {
        global_tmp_window = gtk_application_window_new(app);
    }

    // gtk_window_present(GTK_WINDOW(global_tmp_window));

    if(global_window != NULL)
    {
        printf("Remove!\n");
        gtk_application_remove_window(app, GTK_WINDOW(global_window));
        global_window = NULL;
    }

    else 
    {
        GtkBuilder *builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, ui_file, NULL);

        /* 从界面构造器中获取窗口, 并设置到当前应用 */
        global_window = gtk_builder_get_object(builder, "window");
        // gtk_window_set_application(GTK_WINDOW(global_window), app);
        
        /* 加载 CSS 样式到整个窗口 */
        GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (global_window));
        GtkCssProvider *provider = gtk_css_provider_new ();
        gtk_css_provider_load_from_path(provider, css_file);
        gtk_style_context_add_provider_for_display (display, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        g_object_unref(builder);

        if(global_window)
            gtk_window_present(GTK_WINDOW(global_window));
    }

    pthread_mutex_unlock(&lock);
    printf("give\n");
}

void app_load(void)
{
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), 0, NULL);

    g_object_unref(app);
}

int main(int argc, char *argv[])
{
    // if(argc != 3) 
    // {
    //     printf("Usage: %s <CSS File> <UI File>\n", argv[0]);
    //     return -1;
    // }

    // strncpy(css_file, argv[1], sizeof(css_file));
    // strncpy(ui_file, argv[2], sizeof(ui_file));

    printf("UI %s\n", ui_file);
    printf("CSS %s\n", css_file);

    pthread_t th;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&th, NULL, check_file_change, NULL);  

    while (1)
    {
        app_load();
        Sleep(1);
    }
    
    
    return 0;
}
