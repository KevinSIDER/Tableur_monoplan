/* ---------------------------------------------------
 * Entete pour la gestion des formules, de la feuille de
 * calcul et du graphe des cellules.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#ifndef CELL_H
#define CELL_H

#include "Stack/stack.h"
#include "../Module_liste/liste.h"

#define NB_LIGNES 50
#define NB_COLONNES 26
#define TAILLEMAXNOM 50

typedef struct s_cell {
    char *chaine;
    double valeur;
    node_t *liste_jetons;
    // Module_cellules (graphe)
    node_t *successeurs;      // Liste des cellules qui dépendent de celle-ci (les arcs sortants)
    int nombre_predecesseurs;
    int dans_sous_graphe;     // Booléen (0 ou 1) : indique si la cellule fait partie du sous-graphe à recalculer
    int has_error;            // Booléen (0 ou 1) : indique si la cellule a généré une erreur lors du dernier calcul
} s_cell;

struct s_bind; /* Déclaration pour le module IHM */

typedef struct s_feuille_calcul {
    char nom[TAILLEMAXNOM];
    int lignes;
    int colonnes;
    s_cell ***cellules;
    struct s_bind ***binds;
} s_feuille_calcul;

typedef struct s_token {
    enum {VALUE, REF, OPERATOR} type ;
    union {
        double cst;
        void (*operator)(my_stack_t *);
        s_cell *ref;
    } value;
} s_token;

s_feuille_calcul* creer_feuille(const char *nom);
void analyser_chaine_cell(s_cell *cell, s_feuille_calcul *feuille);
void retirer_predecesseurs_et_jetons(s_cell *cell);
double evaluate_cell(s_cell *cell);
int is_operator(char c);
void op_add(my_stack_t *s);
void op_sub(my_stack_t *s);
void op_mult(my_stack_t *s);
void op_div(my_stack_t *s);
void propager_changements(s_cell *cell_depart);

#endif