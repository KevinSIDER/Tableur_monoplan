#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Module_formule_cellules/cell.h"
#include "../Module_liste/liste.h"
#include "bind.h"

// Fonction de recherche de binding par cellule
s_bind *bindLookupByCell(AppData *app, s_cell *cell) {
    if (!app || !cell) return NULL;
    
    for (int c = 0; c < NB_COLONNES; c++) {
        for (int l = 0; l < NB_LIGNES; l++) {
            if (app->bindings[c][l] && app->bindings[c][l]->cell == cell) {
                return app->bindings[c][l];
            }
        }
    }
    return NULL;
}

// Fonction de recherche de binding par position
s_bind *bindLookupByPos(AppData *app, int colonne, int ligne) {
    if (!app || colonne < 0 || colonne >= NB_COLONNES || 
        ligne < 0 || ligne >= NB_LIGNES) {
        return NULL;
    }
    return app->bindings[colonne][ligne];
}

// Mise à jour de l'affichage d'une cellule
void update_cell_display(s_bind *bind) {
    if (!bind || !bind->case_widget) return;
    
    char buffer[256];
    if (bind->cell && bind->cell->chaine) {
        // Afficher la valeur calculée
        snprintf(buffer, sizeof(buffer), "%.2f", bind->cell->valeur);
    } else {
        buffer[0] = '\0';
    }
    
    gtk_label_set_text(GTK_LABEL(bind->case_widget), buffer);
}

// Callback quand on clique sur une cellule
void on_cell_clicked(GtkWidget *event_box, GdkEventButton *event, gpointer user_data) {
    s_bind *bind = (s_bind *)user_data;
    AppData *app = g_object_get_data(G_OBJECT(event_box), "app_data");
    
    if (!bind || !app) return;
    
    // Mettre à jour la cellule active
    app->cellule_active = bind;
    
    // Afficher la formule dans le champ de saisie
    if (bind->cell && bind->cell->chaine) {
        gtk_entry_set_text(GTK_ENTRY(app->entry_formule), bind->cell->chaine);
    } else {
        gtk_entry_set_text(GTK_ENTRY(app->entry_formule), "");
    }
    
    // Afficher quelle cellule est active
    char label[32];
    snprintf(label, sizeof(label), "Cellule %c%d:", 'A' + bind->colonne, bind->ligne + 1);
    
        if (app->label_cellule && GTK_IS_LABEL(app->label_cellule)) {
            gtk_label_set_text(GTK_LABEL(app->label_cellule), label);
        } else {
            g_warning("label_cellule absent ou invalide");
        }
}

// Callback quand on valide une formule
void on_entry_activate(GtkWidget *entry, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (!app || !app->cellule_active) {
        g_print("Aucune cellule sélectionnée\n");
        return;
    }
    
    const char *formule = gtk_entry_get_text(GTK_ENTRY(entry));
    s_bind *bind = app->cellule_active;
    
    // Créer ou mettre à jour la cellule
    if (!bind->cell) {
        bind->cell = calloc(1, sizeof(s_cell));
        bind->cell->successeurs = list_create();
        app->feuille->cellules[bind->colonne][bind->ligne] = bind->cell;
    }
    
    // Libérer l'ancienne formule si elle existe
    if (bind->cell->chaine) {
        free(bind->cell->chaine);
    }
    
    // Copier la nouvelle formule
    bind->cell->chaine = strdup(formule);
    
    // Analyser et évaluer
    analyser_chaine_cell(bind->cell, app->feuille);
    
    // Propager les changements
    propager_changements(bind->cell);
    
    // Mettre à jour l'affichage de toutes les cellules affectées
    for (int c = 0; c < NB_COLONNES; c++) {
        for (int l = 0; l < NB_LIGNES; l++) {
            if (app->bindings[c][l] && app->bindings[c][l]->cell) {
                update_cell_display(app->bindings[c][l]);
            }
        }
    }
    
    g_print("Cellule %c%d mise à jour: %s = %.2f\n", 
            'A' + bind->colonne, bind->ligne + 1, 
            formule, bind->cell->valeur);
}

// Callback pour fermer l'application
void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    gtk_main_quit();
}

// Création de la grille de cellules
void create_cell_grid(GtkWidget *grid, AppData *app) {
    // Créer les en-têtes de colonnes (A, B, C, ...)
    for (int c = 0; c < NB_COLONNES; c++) {
        char col_name[3];
        snprintf(col_name, sizeof(col_name), "%c", 'A' + c);
        
        GtkWidget *header = gtk_label_new(col_name);
        gtk_widget_set_size_request(header, 80, 30);
        gtk_grid_attach(GTK_GRID(grid), header, c + 1, 0, 1, 1);
    }
    
    // Créer les en-têtes de lignes (1, 2, 3, ...)
    for (int l = 0; l < NB_LIGNES; l++) {
        char row_name[5];
        snprintf(row_name, sizeof(row_name), "%d", l + 1);
        
        GtkWidget *header = gtk_label_new(row_name);
        gtk_widget_set_size_request(header, 30, 30);
        gtk_grid_attach(GTK_GRID(grid), header, 0, l + 1, 1, 1);
    }
    
    // Créer toutes les cellules
    for (int c = 0; c < NB_COLONNES; c++) {
        for (int l = 0; l < NB_LIGNES; l++) {
            // Créer le binding
            s_bind *bind = calloc(1, sizeof(s_bind));
            bind->ligne = l;
            bind->colonne = c;
            bind->cell = NULL; // Sera créé à la première saisie
            
            // Créer le label pour afficher la valeur
            bind->case_widget = gtk_label_new("");
            gtk_widget_set_size_request(bind->case_widget, 80, 30);
            gtk_label_set_xalign(GTK_LABEL(bind->case_widget), 1.0); // Alignement à droite
            
            // Créer un EventBox pour capturer les clics
            GtkWidget *event_box = gtk_event_box_new();
            gtk_container_add(GTK_CONTAINER(event_box), bind->case_widget);
            
            // Ajouter une bordure visible
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_container_add(GTK_CONTAINER(frame), event_box);
            
            // Connecter le signal de clic
            g_object_set_data(G_OBJECT(event_box), "app_data", app);
            g_signal_connect(event_box, "button-press-event", 
                           G_CALLBACK(on_cell_clicked), bind);
            
            // Ajouter à la grille
            gtk_grid_attach(GTK_GRID(grid), frame, c + 1, l + 1, 1, 1);
            
            // Enregistrer le binding
            app->bindings[c][l] = bind;
        }
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Créer la feuille de calcul
    s_feuille_calcul *feuille = creer_feuille("MonoPlan");
    if (!feuille) {
        g_print("Erreur: impossible de créer la feuille\n");
        return 1;
    }
    
    // Créer la structure AppData
    AppData *app = calloc(1, sizeof(AppData));
    app->feuille = feuille;
    app->cellule_active = NULL;
    
    // Allouer le tableau de bindings
    app->bindings = calloc(NB_COLONNES, sizeof(s_bind **));
    for (int c = 0; c < NB_COLONNES; c++) {
        app->bindings[c] = calloc(NB_LIGNES, sizeof(s_bind *));
    }
    
    // Charger l'interface Glade
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "Module_IHM/monoplan.glade", &error)) {
        g_print("Erreur lors du chargement de monoplan.glade: %s\n", error->message);
        g_error_free(error);
        return 1;
    }
    
    // Récupérer les widgets
    GtkWidget *fenetre = GTK_WIDGET(gtk_builder_get_object(builder, "fenetre"));
    GtkWidget *grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid_cellules"));
    app->entry_formule = GTK_WIDGET(gtk_builder_get_object(builder, "entry_formule"));
    app->label_cellule = GTK_WIDGET(gtk_builder_get_object(builder, "label_cellule"));
    if (app->label_cellule) {
        g_print("label_cellule initialisé: %p (%s)\n", app->label_cellule, G_OBJECT_TYPE_NAME(app->label_cellule));
    } else {
        g_warning("label_cellule absent après gtk_builder_get_object");
    }
    
    // Créer la grille de cellules
    create_cell_grid(grid, app);
    
    // Connecter manuellement les signaux nécessaires
    GtkWidget *menu_quitter = GTK_WIDGET(gtk_builder_get_object(builder, "menu_quitter"));
    if (menu_quitter) {
        g_signal_connect(menu_quitter, "activate", G_CALLBACK(on_window_destroy), NULL);
    }
    g_signal_connect(fenetre, "destroy", G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect(app->entry_formule, "activate", G_CALLBACK(on_entry_activate), app);
    
    // Afficher la fenêtre
    gtk_widget_show_all(fenetre);
    
    g_object_unref(builder);
    
    // Lancer la boucle principale GTK
    gtk_main();
    
    // Nettoyage
    for (int c = 0; c < NB_COLONNES; c++) {
        for (int l = 0; l < NB_LIGNES; l++) {
            if (app->bindings[c][l]) {
                free(app->bindings[c][l]);
            }
        }
        free(app->bindings[c]);
    }
    free(app->bindings);
    free(app);
    
    return 0;
}