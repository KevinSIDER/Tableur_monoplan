/* ---------------------------------------------------
 * Entete pour la gestion de liste chainée.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#ifndef _LIST_H_
#define _LIST_H_

struct node {
        void* data;
        struct node *next;
};
typedef struct node node_t;

node_t * list_create(void);
        // creation d'une nouvelle liste vide

void * list_get_data(const node_t * node);

void list_set_data(node_t * node, void * data);
        // lire ou ecrire la donnee d'un noeud

node_t * list_next(node_t * node);
        // obtenir le noeud suivant

node_t * list_insert(node_t * head, void * data);
        // creation et insertion d'un noeud en tete de liste
        // retourne la tete de liste

node_t * list_append(node_t * head, void * data);
        // creation et ajout d'un noeud en queue de liste
        // retourne la tete de liste

node_t * list_remove(node_t * head, void * data);
        // suppression de la premiere instance d'une
        // donnee dans la liste, retourne la tete de liste

node_t * list_headRemove(node_t * head);
        // suppression de la tete de liste
        // retourne la nouvelle tete de liste
        
void list_destroy(node_t * head);
        // destruction d'une liste
        // (La liberation des donnees n'est pas prise en charge)


#endif