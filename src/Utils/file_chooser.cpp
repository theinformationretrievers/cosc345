#include <gtk/gtk.h>
#include <iostream>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    NULL,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::cout << filename << std::endl;
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
    return 0;
}