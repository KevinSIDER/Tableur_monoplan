#ifndef BIND_H
#define BIND_H

#include <gtk/gtk.h>
#include "../Module_formule_cellules/cell.h"

typedef struct bind {
    int ligne, colonne;      // position dans la feuille
    GtkWidget *case_widget;  // widget d'affichage (label)
    GtkWidget *entry;        // widget d'édition
    s_cell *cell;            // modèle interne de la cellule
} s_bind;

typedef struct {
    s_feuille_calcul *feuille;
    s_bind ***bindings;      // tableau 2D de bindings [colonne][ligne]
    GtkWidget *entry_formule; // champ de saisie de formule
    GtkWidget *label_cellule;  // label affichant la cellule active
    s_bind *cellule_active;   // cellule actuellement sélectionnée
} AppData;

// Fonctions de recherche de bindings
s_bind *bindLookupByCell(AppData *app, s_cell *cell);
s_bind *bindLookupByPos(AppData *app, int colonne, int ligne);

// Fonction pour mettre à jour l'affichage d'une cellule
void update_cell_display(s_bind *bind);

#endif