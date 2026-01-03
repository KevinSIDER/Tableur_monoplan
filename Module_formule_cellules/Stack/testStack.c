/* -------------------------------------------------------------------
   Teste unitaire de la pile
   -------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define SIZE 5
int main(void) {
 int pos;
 double val, val_pile;
 my_stack_t * s = STACK_CREATE(SIZE,double);

 val=0.0;
 while ( STACK_PUSH(s,val,double)!=0 )
 {
  val += 1.0;
 }

 if ( STACK_CAPACITY(s) != SIZE )
 	printf("Capacite de la pile:%d (attendu 2) \n",STACK_CAPACITY(s));
 if ( STACK_MEM_USED(s) != SIZE )
 	printf("Nombre de cases pleines:%d (attendu 3)\n",STACK_MEM_USED(s));
 if ( STACK_MEM_AVAILABLE(s) != 0 )
 	printf("Nombre de cases vides:%d (attendu 0) \n",STACK_MEM_AVAILABLE(s));
 if ( STACK_VALUE_AVAILABLE(s) != 1  )
	printf("valeur disponible : %d (attendu VRAI) \n", STACK_VALUE_AVAILABLE(s));
 if ( STACK_EMPTY(s) != 0  )
	printf("pile vide : %d (attendu FAUX) \n", STACK_EMPTY(s));


 while ( STACK_POP2(s,val_pile,double) )
 {
  if ( --val != val_pile) printf("Valeur depilee : %f (attendue %f)\n",val_pile, val);
 }

 if ( STACK_MEM_USED(s) != 0 )
 	printf("Nombre de cases pleines:%d (attendu 0)\n",STACK_MEM_USED(s));
 if ( STACK_MEM_AVAILABLE(s) != SIZE )
 	printf("Nombre de cases vides:%d (attendu 3) \n",STACK_MEM_AVAILABLE(s));
 if ( STACK_VALUE_AVAILABLE(s) != 0  )
	printf("valeur disponible : %d (attendu FAUX) \n", STACK_VALUE_AVAILABLE(s));
 if ( STACK_EMPTY(s) != 1  )
	printf("pile vide : %d (attendu VRAI) \n", STACK_EMPTY(s));

 val=0.0;
 while ( STACK_PUSH(s,val,double) )
 {
  val += 1.0;
 }

 STACK_INIT_ITERATOR(s);
 

 val=0.0;
 while ( (pos=STACK_ITERATE(s,val_pile, double)) )
 {
  if ( val != val_pile) printf("Valeur lue : %f (attendue %f)\n",val_pile, val);
  val++;
 }

 STACK_REMOVE(s);
 
 printf("Fin du test de la pile\n");
 return 0;
}
