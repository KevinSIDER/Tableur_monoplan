/* ---------------------------------------------------
 * Gestion simple d'une pile
 * ---------------------------------------------------
 * S. Rubini
 * ---------------------------------------------------
 */

#include <stdlib.h>
#include "stack.h"


/* ----------------------------------------------------
 * creation d'une stack LIFO
 * ----------------------------------------------------
 * nbMaxElement	: nombre d'elements maximum 
 * elementSize : taille en octet d'un element
 * ----------------------------------------------------
 * retourne un pointeur de stack, ou NULL en cas d'erreur
 * ----------------------------------------------------
 */
my_stack_t * stack_create(int nbMaxElement, int elementSize) {
	my_stack_t * stack;

	if ( (stack=(my_stack_t *)malloc(sizeof(my_stack_t)))==NULL) {
		return NULL;
	}

	stack->nbMaxElement=nbMaxElement;
	stack->free=0;
	if ( (stack->value=(void *)malloc(elementSize*nbMaxElement))==NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}	

  
	
/* ----------------------------------------------------
 * Destruction d'une pile LIFO
 * ----------------------------------------------------
 * stack		: pile à detruire
 * ----------------------------------------------------
 * retourne un pointeur de pile, ou NULL en cas d'erreur
 * ----------------------------------------------------
 */
void stack_remove(my_stack_t * stack) {
	free(stack->value);
	free(stack);
	return;
}
		
