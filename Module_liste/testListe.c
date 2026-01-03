/* ---------------------------------------------------
 * Tests unitaires du module liste.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#include <stdio.h>
#include "liste.h"

int main() {
    printf("--- TESTS UNITAIRES POUR LE MODULE LISTE --- \n");

    printf("- Test list_create() \n");

    node_t *list = list_create();

    if(list == NULL){
        printf("-- Création d'une liste vide\n");
    } else {
        printf("-- La liste vide n'a pas été créée \n");
    }

    printf("- Test list_insert() \n");

    int a = 10, b = 20, c = 30;
    list = list_insert(list, &a);
    list = list_insert(list, &b);
    list = list_insert(list, &c);

    printf("- Test list_get_data() et list_next() \n");
    printf("-- Contenu de la liste :\n");

    int compteur = 0;
    for(node_t *actuel = list; actuel != NULL; actuel = list_next(actuel)) {
        compteur++;
        printf("--- Donnée du nœud numéro %d : %d\n", compteur, *(int *)list_get_data(actuel));
    }

    printf("- Test list_set_data() \n");

    list_set_data(list, &a);
    printf("-- Nouvelle valeur du premier élément de la liste : %d\n", *(int*)list_get_data(list));
    printf("-- Nouveau Contenu de la liste :\n");
    compteur = 0;
    for(node_t *actuel = list; actuel != NULL; actuel = list_next(actuel)) {
        compteur++;
        printf("--- Donnée du nœud numéro %d : %d\n", compteur, *(int *)list_get_data(actuel));
    }

    printf("- Test list_append() \n");
    int d = 40, e = 50;
    printf("-- Ajout de 40 et 50 \n");
    list = list_append(list, &d);
    list = list_append(list, &e);

    printf("-- Contenu de la liste après append :\n");
    compteur = 0;
    for(node_t *actuel = list; actuel != NULL; actuel = list_next(actuel)) {
        compteur++;
        printf("--- Donnée du nœud numéro %d : %d\n", compteur, *(int *)list_get_data(actuel));
    }

    printf("- Test list_remove() \n");
    printf("-- SUpprimer le noeud contenant 20 \n");
    list = list_remove(list, &b);

    printf("-- Contenu de la liste après remove :\n");
    compteur = 0;
    for(node_t *actuel = list; actuel != NULL; actuel = list_next(actuel)) {
        compteur++;
        printf("--- Donnée du nœud numéro %d : %d\n", compteur, *(int *)list_get_data(actuel));
    }

    printf("- Test list_headRemove() \n");
    list = list_headRemove(list);

    printf("-- Contenu de la liste après headRemove :\n");
    compteur = 0;
    for(node_t *actuel = list; actuel != NULL; actuel = list_next(actuel)) {
        compteur++;
        printf("--- Donnée du nœud numéro %d : %d\n", compteur, *(int *)list_get_data(actuel));
    }

    printf("- Test list_destroy() \n");
    list_destroy(list);
    list = NULL;
    if (list == NULL) {
        printf("-- Liste vide confirmée\n");
    }
}