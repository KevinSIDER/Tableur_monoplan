/* ---------------------------------------------------
 * Gestion des formules saisies, création de sous-graphe
 * afin de recalculer les cellules selon leur dépendances.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cell.h"
#include "../Module_liste/liste.h"
#include "Stack/stack.h"

void op_add(my_stack_t *s) {
    double val2, val1;
    if (STACK_POP2(s, val2, double) && STACK_POP2(s, val1, double))
        STACK_PUSH(s, val1 + val2, double);
}

void op_sub(my_stack_t *s) {
    double val2, val1;
    if (STACK_POP2(s, val2, double) && STACK_POP2(s, val1, double)) {
        double res = val1 - val2;
        STACK_PUSH(s, res, double);    
    }
}

void op_mult(my_stack_t *s) {
    double val2, val1;
    if (STACK_POP2(s, val2, double) && STACK_POP2(s, val1, double))
        STACK_PUSH(s, val1 * val2, double);
}

void op_div(my_stack_t *s) {
    double val2, val1;
    if (STACK_POP2(s, val2, double) && STACK_POP2(s, val1, double)) {
        if (val2 != 0.0)
            STACK_PUSH(s, val1 / val2, double);
        else {
            printf("Erreur : division par zéro\n");
            STACK_PUSH(s, 0.0, double);
        }
    }
}

s_feuille_calcul* creer_feuille(const char *nom) {
    s_feuille_calcul *feuille = malloc(sizeof(s_feuille_calcul));
    if(feuille == NULL) return NULL;
    
    strncpy(feuille->nom, nom, TAILLEMAXNOM - 1);
    feuille->nom[TAILLEMAXNOM - 1] = '\0';
    feuille->lignes = NB_LIGNES;
    feuille->colonnes = NB_COLONNES;
    
    feuille->cellules = malloc(NB_COLONNES * sizeof(s_cell **));
    if (feuille->cellules == NULL) return NULL;

    for (int c = 0; c < NB_COLONNES; c++) {
        feuille->cellules[c] = malloc(NB_LIGNES * sizeof(s_cell *));
        if (feuille->cellules[c] == NULL) return NULL;
        
        for (int l = 0; l < NB_LIGNES; l++)
            feuille->cellules[c][l] = NULL;
    }
    return feuille;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void analyser_chaine_cell(s_cell *cell, s_feuille_calcul *feuille) {
    if (cell == NULL || cell->chaine == NULL) return;

    retirer_predecesseurs_et_jetons(cell);

    // Initialisation des champs graphe si nouvelle cellule
    if (cell->successeurs == NULL) cell->successeurs = list_create();
    cell->nombre_predecesseurs = 0;
    cell->dans_sous_graphe = 0;

    if (cell->chaine[0] != '=') {
        printf("Erreur : La formule '%s' est invalide (doit commencer par '=')\n", cell->chaine);
        cell->liste_jetons = NULL;
        return;
    }

    cell->liste_jetons = list_create();
   
    char *expr = cell->chaine + 1; /* skip '=' */
    while (*expr && isspace((unsigned char)*expr)) expr++;
    char *chaine_copy = strdup(expr);
    s_token *temp_tokens[100]; 
    int count = 0;

    char *token = strtok(chaine_copy, " ");
    while (token && count < 100) {  
        s_token *tok = malloc(sizeof(s_token)); 

        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
            tok->type = VALUE;
            tok->value.cst = atof(token);
        } 
        else if (is_operator(token[0]) && token[1] == '\0') {
            tok->type = OPERATOR;
            switch (token[0]) {
                case '+': tok->value.operator = op_add; break;
                case '-': tok->value.operator = op_sub; break;
                case '*': tok->value.operator = op_mult; break;
                case '/': tok->value.operator = op_div; break;
            }
        } 
        else if (isalpha(token[0])) {
            char col_char = toupper(token[0]);     
            int col = col_char - 'A';          
            int lig = atoi(token + 1) - 1;         

            tok->type = REF;
            if (feuille && col >= 0 && col < feuille->colonnes &&
                lig >= 0 && lig < feuille->lignes) {
                    if (feuille->cellules[col][lig] == NULL) {
                        feuille->cellules[col][lig] = calloc(1, sizeof(s_cell));
                        // On initialise juste la liste (nécessaire pour le graphe)
                        feuille->cellules[col][lig]->successeurs = list_create();
                    }
                
                s_cell *ref_cell = feuille->cellules[col][lig];
                tok->value.ref = ref_cell;

                // --- CONSTRUCTION DU GRAPHE ---
                // "Les références... constituent les prédécesseurs"
                // Donc 'cell' est un successeur de 'ref_cell'
                int found = 0;
                node_t *n = ref_cell->successeurs;
                while (n) {
                    if (list_get_data(n) == (void *)cell) {
                        found = 1;
                        break;
                    }
                    n = list_next(n);
                }
                if (!found) {
                    ref_cell->successeurs = list_insert(ref_cell->successeurs, cell);
                }
            } else {
                tok->value.ref = NULL;
                printf("Référence invalide : %s\n", token);
            }
        }
        temp_tokens[count++] = tok;
        token = strtok(NULL, " ");
    }
    for (int i = count - 1; i >= 0; i--) {
        cell->liste_jetons = list_insert(cell->liste_jetons, temp_tokens[i]);
    }
    free(chaine_copy);
}

double evaluate_cell(s_cell *cell) {
    if (cell == NULL || cell->liste_jetons == NULL) {
        return cell->valeur;
    }

    my_stack_t *stack = STACK_CREATE(100, double);
    int error = 0; 
    node_t *current = cell->liste_jetons;

    while (current) {
        s_token *tok = (s_token *)list_get_data(current);
        if (tok) {
            switch (tok->type) {
                case VALUE:
                    STACK_PUSH(stack, tok->value.cst, double);
                    break;
                case REF:
                    if (tok->value.ref) {
                        if (tok->value.ref->chaine == NULL || tok->value.ref->liste_jetons == NULL) {
                            printf("Erreur : référence invalide vers une cellule sans formule.\n");
                            STACK_PUSH(stack, 0.0, double);
                            error = 1;
                        } else {
                            STACK_PUSH(stack, tok->value.ref->valeur, double);
                        }
                    } else
                        STACK_PUSH(stack, 0.0, double);
                    break;
                case OPERATOR:
                    if (tok->value.operator)
                        tok->value.operator(stack);
                    break;
            }
        }
        current = list_next(current);
    }

    double res = 0.0;
    if (!STACK_EMPTY(stack))
        STACK_POP2(stack, res, double);

    STACK_REMOVE(stack);

    cell->valeur = res;
    cell->has_error = error;
    return res;
}

void retirer_predecesseurs_et_jetons(s_cell *cell) {
    if (!cell) return;
    if (!cell->liste_jetons) return;
    node_t *n = cell->liste_jetons;
    while (n) {
        s_token *tok = (s_token *)list_get_data(n);
        if (tok) {
            if (tok->type == REF && tok->value.ref) {
                s_cell *ref = tok->value.ref;
                if (ref && ref->successeurs) {
                    ref->successeurs = list_remove(ref->successeurs, cell);
                }
            }
            free(tok);
        }
        n = list_next(n);
    }
    list_destroy(cell->liste_jetons);
    cell->liste_jetons = NULL;
}

// Fonction utilitaire pour repérer récursivement le sous-graphe impacté (Passe 1)
void reperer_sous_graphe(s_cell *c, node_t **liste_noeuds) {
    // Si la cellule est vide ou déjà marquée comme étant dans le sous-graphe, on arrête
    if (c == NULL || c->dans_sous_graphe) return;

    c->dans_sous_graphe = 1;      // Marquer comme appartenant à X
    c->nombre_predecesseurs = 0;  // Reset du compteur local pour l'algo
    
    // Ajout à la liste linéaire pour pouvoir itérer dessus plus tard
    *liste_noeuds = list_insert(*liste_noeuds, c);

    // Propagation aux successeurs
    node_t *current = c->successeurs;
    while (current) {
        s_cell *succ = (s_cell *)list_get_data(current);
        reperer_sous_graphe(succ, liste_noeuds);
        current = list_next(current);
    }
}

// Mise en oeuvre de l'algorithme complet de la Figure 3
void propager_changements(s_cell *cellule_depart) {
    if (cellule_depart == NULL) return;

    // === 1. Initialisation et calcul des degrés ===
    // "soit X le sous-graphe issu de S_init" 
    node_t *noeuds_sous_graphe = list_create();
    
    // Identifier tous les noeuds du sous-graphe (X)
    reperer_sous_graphe(cellule_depart, &noeuds_sous_graphe);

    // "d-[Si] <- degré-(Si)" : Calculer le nombre de prédécesseurs DANS le sous-graphe
    node_t *noeud_courant = noeuds_sous_graphe;
    while (noeud_courant) {
        s_cell *u = (s_cell *)list_get_data(noeud_courant);
        
        node_t *succ_courant = u->successeurs;
        while (succ_courant) {
            s_cell *v = (s_cell *)list_get_data(succ_courant);
            // On incrémente le compteur de v uniquement si v fait partie du sous-graphe
            if (v->dans_sous_graphe) { 
                v->nombre_predecesseurs++;
            }
            succ_courant = list_next(succ_courant);
        }
        noeud_courant = list_next(noeud_courant);
    }

    // "liste <- {Sinit}"
    // Cette liste sert de file d'attente pour les cellules prêtes à être calculées
    node_t *file_attente = list_create();
    file_attente = list_insert(file_attente, cellule_depart);

    // === 2. Itération ===
    while (file_attente != NULL) {
        // "retirer un sommet S de liste"
        s_cell *S = (s_cell *)list_get_data(file_attente);
        node_t *next_node = list_next(file_attente);
        free(file_attente); 
        file_attente = next_node;
        evaluate_cell(S);

        node_t *succ_courant = S->successeurs;
        while (succ_courant) {
            s_cell *Sj = (s_cell *)list_get_data(succ_courant);
            
            // On ne traite que les cellules appartenant au sous-graphe
            if (Sj->dans_sous_graphe) {
                Sj->nombre_predecesseurs--; // "d-[Sj] <- d-[Sj] - 1" 
                
                // "si d-[Sj] = 0 alors liste <- liste U {Sj}" 
                if (Sj->nombre_predecesseurs == 0) {
                     file_attente = list_insert(file_attente, Sj);
                }
            }
            succ_courant = list_next(succ_courant);
        }
    }

    // Nettoyage : remettre le marqueur 'dans_sous_graphe' à 0 pour la prochaine utilisation
    noeud_courant = noeuds_sous_graphe;
    while(noeud_courant) {
        s_cell *c = (s_cell *)list_get_data(noeud_courant);
        c->dans_sous_graphe = 0;
        noeud_courant = list_next(noeud_courant);
    }
}