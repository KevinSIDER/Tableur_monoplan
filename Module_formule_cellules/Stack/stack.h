/* ---------------------------------------------------
 * Entete et macro pour la gestion de pile
 * les elements doivent etre affectables
 * ---------------------------------------------------
 * S. Rubini
 * ---------------------------------------------------
 */
#ifndef _STACK_H_
#define _STACK_H_

typedef struct  {
        int nbMaxElement;
        int free;
        void * value; // valeurs enregistrees dans la pile
		      // types a l'utilisation
	int pos_iteration;
} my_stack_t;

/* ---------------------------------------------------
 * macro definition
 * capacite de la pile
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne la capacite de la pile
 * ----------------------------------------------------
 */
#define STACK_CAPACITY(stack)  ( (stack)->nbMaxElement ) 

/* ---------------------------------------------------
 * macro definition
 * teste si le stack est vide 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne 1 si la pile est vide, 0 sinon
 * ----------------------------------------------------
 */
#define STACK_EMPTY(stack)  ( (stack)->free ? 0 : 1 ) 

/* ---------------------------------------------------
 * macro definition
 * teste si la pile n'est pas vide
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne 1 si la pile n'est pas vide, 0 sinon
 * ----------------------------------------------------
 */
#define STACK_VALUE_AVAILABLE(stack) ( (stack)->free ? 1 : 0 ) 

/* ---------------------------------------------------
 * macro definition
 * nombre de places libres
 * ---------------------------------------------------
 * stack 	: stack (my_stack_t *)
 * ---------------------------------------------------
 * retourne le nombre de places libres
 * ----------------------------------------------------
 */
#define STACK_MEM_AVAILABLE(stack) ( (stack)->nbMaxElement - (stack)->free )

/* ---------------------------------------------------
 * macro definition
 * nombre de places occupees
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne le nombre de places utilisees
 * ----------------------------------------------------
 */
#define STACK_MEM_USED(stack) ( (stack)->free )

 
/* ---------------------------------------------------
 * macro definition
 * depile une valeur 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * type		: type de l'element a depiler
 * ---------------------------------------------------
 * retourne la valeur depil�e ou 0 en absence de valeur
 * Il faut que la valeur 0 puisse etre convertie
 * dans le type choisi pour les elements 
 * ----------------------------------------------------
 */
#define STACK_POP(stack, type)  \
    ( (stack)->free ? \
   	  ((type *)((stack)->value))[--((stack)->free)] : \
	  (type)0  )

/* ---------------------------------------------------
 * macro definition
 * depile une valeur 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * var		: objet affectable ou sera enregistree la
 *                   valeur depilee
 * type		: type de l'element a depiler
 * ---------------------------------------------------
 * retourne 0 si la pile est vide, 1 sinon
 * si la pile est vide, la valeur de var n'est pas changee
 * ----------------------------------------------------
 */
#define STACK_POP2(stack, var, type)  \
    (  ( (stack)->free ? \
   	    (var)=((type *)((stack)->value))[--((stack)->free)], 1 : \
	    0 ) \
	   )
	
/* ---------------------------------------------------
 * macro definition
 * initialisation d'un parcours iteratif du contenu de la pile 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * ----------------------------------------------------
 */
#define STACK_INIT_ITERATOR(stack)  \
	(stack)->pos_iteration=0  

/* ---------------------------------------------------
 * macro definition
 * parcours iteratif du contenu de la pile 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * var		: elemnt courrant
 * type		: type des elements 
 * ---------------------------------------------------
 * retourne une valeur enregistr�e dans la pile dans var, 
 *	et 0 en fin de parcours 1 sinon 
 * si la pile est vide, la valeur de var n'est pas changee
 * ----------------------------------------------------
 */
#define STACK_ITERATE(stack, var, type)  \
 ( (stack)->pos_iteration < (stack)->free ? \
   	(var)=((type*)(stack)->value)[(stack)->pos_iteration++], 1  : \
	0 )  

/* ---------------------------------------------------
 * macro definition
 * empile une valeur 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * valeur	: valeur � mettre dans la pile
 * type		: type de l'element 
 * ---------------------------------------------------
 * retourne 0 si la pile est pleine, 1 sinon 
 * ----------------------------------------------------
 */
#define STACK_PUSH( stack , val, type )  \
 (  ( (stack)->free < (stack)->nbMaxElement) ? \
   	( ((type*)((stack)->value))[(stack)->free++]=(type)(val), 1 ) : \
	(type)0  )

/* macro pour faire joli ...*/
#define STACK_CREATE(nbMaxElement, type) stack_create(nbMaxElement, sizeof(type))
#define STACK_REMOVE(stack)              stack_remove(stack)

// declarations des fonctions publics
my_stack_t * stack_create(int nbMaxElement, int elementSize);
void stack_remove(my_stack_t * stack);




#endif
