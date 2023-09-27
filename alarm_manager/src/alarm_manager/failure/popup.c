#include "alarm_manager/failure.h"
#include "alarm_manager/system_condition/usage_utils.h"
#include "utils/program_log.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#ifdef POPUP
#include <gtk/gtk.h>
#endif

#define PROC_STAT_CPU_ENTRY_SIZE 13

int load_to_count_f(double val, double lim)
{
    my_log("load_to_count_f: start", 1);

    return val < lim ?
        trunc((val / lim) * 10) :
        10;
}

int load_to_count_lu(unsigned long val, unsigned long lim)
{
    my_log("load_to_count_lu: start", 1);

    return (double)val < (double)lim ?
        trunc((double)val / (double)lim * 10) :
        10;
}

char* load_to_visual(int load)
{
    my_log("load_to_visual: start", 1);

    char* load_visual = (char*)malloc(PROC_STAT_CPU_ENTRY_SIZE);
    if (load_to_visual == NULL)
    {
        my_log("load_to_visual: malloc", 0);
        exit(EXIT_FAILURE);
    }

    load_visual[0] =  '[';
    for (int i = 1; i < load + 1; i++)
        load_visual[i] = '|';
    for (int i = load + 1; i < PROC_STAT_CPU_ENTRY_SIZE - 2; i++)
        load_visual[i] = ' ';
    load_visual[PROC_STAT_CPU_ENTRY_SIZE - 2] =  ']';
    load_visual[PROC_STAT_CPU_ENTRY_SIZE - 1] =  '\0';
    return load_visual;
}

void stylize(GtkLabel* label,
             GtkLabel* grid,
             int load,
             char* font_desc,
             int row, int col,
             int span_col,
             int span_row)
{
    my_log("stylize: start", 1);
    
    PangoAttribute* Red = pango_attr_foreground_new(60000, 5000, 5000);
    PangoAttribute* Orange = pango_attr_foreground_new(55000, 45000, 20000);
    PangoAttribute* White = pango_attr_foreground_new(55000, 55000, 55000);

    PangoAttrList* const Attrs = pango_attr_list_new();
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_override_font (label, pango_font_description_from_string (font_desc));
    gtk_grid_attach (GTK_CONTAINER (grid), label, row, col, span_col, span_row);


    if (load == 10)
        pango_attr_list_insert(Attrs, Red);
    else if (load >= 7 && load < 10)
        pango_attr_list_insert(Attrs, Orange);
    else 
        pango_attr_list_insert(Attrs, White);
    gtk_label_set_attributes((GtkLabel*)label, Attrs);
    pango_attr_list_unref(Attrs);
}

static gboolean timeout_app(gpointer user_data)
{
    my_log("timeout_app: start", 1);

    g_application_quit(user_data);
    g_object_unref(user_data);
    return FALSE;
}

static void activate(GtkApplication* app, gpointer user_data)
{
    my_log("activate: start", 1);

    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* cpu_info, *ram_info, *swp_info, *dir_info, *dir_path;
    GtkWidget* cpu_visual, *ram_visual, *swp_visual, *dir_visual;
    char* load = NULL;

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "Failure");

    grid = gtk_grid_new();
    gtk_container_add (GTK_CONTAINER (window), grid);

    failure* f = (failure*)user_data;
    size_t size = snprintf(NULL, 0, "CPU: %.2f%%/%.2f%%", f->condition->cpu_usage*100, f->limits->cpu_usage*100) + 1;

    char* label = (char*)malloc(size);
    snprintf(label, size, "CPU: %.2f%%/%.2f%%", f->condition->cpu_usage*100, f->limits->cpu_usage*100);
    cpu_info = gtk_label_new(label);
    free(label);

    size = snprintf(NULL, 0, "RAM: %.2fGB/%.2fGB", kb_to_gb(f->condition->ram_usage), kb_to_gb(f->limits->ram_usage)) + 1;

    label = (char*)malloc(size);
    snprintf(label, size, "RAM: %.2fGB/%.2fGB", kb_to_gb(f->condition->ram_usage), kb_to_gb(f->limits->ram_usage));
    ram_info = gtk_label_new(label);
    free(label);

    label = (char*)malloc(size);
    snprintf(label, size, "SWP: %.2fGB/%.2fGB", kb_to_gb(f->condition->swap_usage), kb_to_gb(f->limits->swap_usage));
    swp_info = gtk_label_new(label);
    free(label);

    label = (char*)malloc(size);
    snprintf(label, size, "DIR: %.2fGB/%.2fGB", kb_to_gb(f->condition->dir_size), kb_to_gb(f->limits->dir_size));
    dir_info = gtk_label_new(label);
    free(label);

    size = snprintf(NULL, 0, "DIR PATH: %s", f->limits->dir_path) + 1;

    label = (char*)malloc(size);
    snprintf(label, size, "DIR PATH: %s", f->limits->dir_path);
    dir_path = gtk_label_new(label);
    free(label);

    load = load_to_visual(load_to_count_f(f->condition->cpu_usage, f->limits->cpu_usage));
    cpu_visual = gtk_label_new(load);
    free(load);

    load = load_to_visual(load_to_count_lu(f->condition->ram_usage, f->limits->ram_usage));
    ram_visual = gtk_label_new(load);
    free(load);

    load = load_to_visual(load_to_count_lu(f->condition->swap_usage, f->limits->swap_usage));
    swp_visual = gtk_label_new(load);
    free(load);

    load = load_to_visual(load_to_count_lu(f->condition->dir_size, f->limits->dir_size));
    dir_visual = gtk_label_new(load);
    free(load);


    stylize(cpu_info, grid, load_to_count_f(f->condition->cpu_usage, f->limits->cpu_usage), "Monospace Bold 12", 0, 0, 1, 1);
    stylize(ram_info, grid, load_to_count_lu(f->condition->ram_usage, f->limits->ram_usage), "Monospace Bold 12", 0, 1, 1, 1);
    stylize(swp_info, grid, load_to_count_lu(f->condition->swap_usage, f->limits->swap_usage), "Monospace Bold 12", 0, 2, 1, 1);
    stylize(dir_info, grid, load_to_count_lu(f->condition->dir_size, f->limits->dir_size), "Monospace Bold 12", 0, 3, 1, 1);
    
    stylize(cpu_visual, grid, load_to_count_f(f->condition->cpu_usage, f->limits->cpu_usage), "Monospace 12", 1, 0, 1, 1);
    stylize(ram_visual, grid, load_to_count_lu(f->condition->ram_usage, f->limits->ram_usage), "Monospace 12", 1, 1, 1, 1);
    stylize(swp_visual, grid, load_to_count_lu(f->condition->swap_usage, f->limits->swap_usage), "Monospace 12", 1, 2, 1, 1);
    stylize(dir_visual, grid, load_to_count_lu(f->condition->dir_size, f->limits->dir_size), "Monospace 12", 1, 3, 1, 1);

    stylize(dir_path, grid, 0, "Monospace 12", 0, 4, 2, 1);

    g_timeout_add_seconds (10, timeout_app, app);
    gtk_widget_show_all (window);
}

int show_popup(failure* f, int argc, char** argv)
{
    my_log("show_popup: start", 1);

    GtkApplication *app;
    int status;
    app = gtk_application_new(NULL, 0);
    g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)f);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    return status;
}
