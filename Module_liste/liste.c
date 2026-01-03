/* ---------------------------------------------------
 * Création et gestion d'une liste chainée.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

node_t * list_create(void){
        // creation d'une nouvelle liste vide
        return NULL;
}

void * list_get_data(const node_t * node){
        // lire la donnee d'un noeud
        if(node == NULL){
                printf("Le noeud sélectionner n'existe pas.\n");
        }
        return node->data;
}

void list_set_data(node_t * node, void * data){
        // ecrire la donnée d'un noeud
        if(node == NULL || data == NULL){
                printf("Le noeud ou la valeur sélectionnée n'existe pas.\n");
                return;
        }
        node->data = data;
}

node_t * list_next(node_t * node){
        // obtenir le noeud suivant
        if(node == NULL){
                printf("Le noeud sélectionner n'existe pas.\n");
                return NULL;
        }
        return node->next;

}

node_t * list_insert(node_t * head, void * data){
        // creation et insertion d'un noeud en tete de liste
        // retourne la tete de liste
        if(data == NULL){
                printf("La valeur sélectionnée n'existe pas.\n");
                return head;
        }
        // Allocation mémoire du nouveau noeud
        node_t * new_node = (node_t *)malloc(sizeof(node_t));
        if (new_node == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            return head;
        }
        // Déplacement de ce noeud en tête
        new_node->data = data;
        new_node->next = head;
        return new_node;
}

node_t * list_append(node_t * head, void * data){
        // creation et ajout d'un noeud en queue de liste
        // retourne la tete de liste
        node_t * new_node = (node_t *)malloc(sizeof(node_t));
        if (new_node == NULL) {
                printf("Erreur d'allocation mémoire.\n");
                return head;
        }
        new_node->data = data;
        new_node->next = NULL;

        if (head == NULL) {
                return new_node; // si la liste est vide
        }

        node_t * current_node = head; // noeud pour parcourir la liste
        while (current_node->next != NULL) {
                current_node = current_node->next; // aller jusqu'au dernier noeud
        }
        current_node->next = new_node; // ajouter en queue de liste
        return head;
}

node_t * list_remove(node_t * head, void * data){
        // suppression de la premiere instance d'une
        // donnee dans la liste, retourne la tete de liste
        if (head == NULL || data == NULL) {
                printf("Le noeud ou la valeur sélectionnée n'existe pas.\n");
                return head;
        }

        if (head->data == data) { // Si la tête contient déja la valeur
                node_t * new_head = list_next(head);
                free(head); 
                return new_head;
        }

        node_t * current_node = head;
        while (list_next(current_node) != NULL) {
                if (current_node->next->data == data) {
                        node_t * node_to_remove = current_node->next;
                        // le noeud suivant la cible recule sur cette cible
                        current_node->next = node_to_remove->next; 
                        free(node_to_remove);
                        return head;
                }
                current_node = list_next(current_node);
        }
        return head;
}

node_t * list_headRemove(node_t * head){
        // suppression de la tete de liste
        // retourne la nouvelle tete de liste
        if (head == NULL) {
                return head;
        }
        node_t * next = list_next(head); // prochain noeud
        free(head);
        return next;
}

void list_destroy(node_t * head){
        // destruction d'une liste
        // (La liberation des donnees n'est pas prise en charge)*/
        while (head != NULL){
                head = list_headRemove(head); // on enlève la tête 
        }
}