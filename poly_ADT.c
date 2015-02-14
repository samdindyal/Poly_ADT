/*
	Name: poly_ADT.c
	Author: Samuel Dindyal
	Written: September, 2014
	Description: A C program which does multiple operations with a polynomial with a user-defined amount of terms,
	coefficients and degrees, and automatically sorts them in descending order by power.
*/

#include "poly_ADT.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Prototypes for helper methods.
*/
int check_sort(poly*);
void poly_sort(poly*);

/*
	Takes in a "poly" and prints it out in a mathematical form. 
		i.e. ax^n + bx^m + cx^o ...
*/
void poly_print(poly *P) 
{
	node *cursor;
	cursor = (node*)malloc(sizeof(node));
	cursor = P->terms;
	int i;
	for (i = 0; i<P->num_terms; i++)	//loops through terms and prints them out in a formatted manner
	{
		printf("%dx^%d%s",cursor->coef, cursor->powr, i<P->num_terms-1 ?  " + " : "" );
		cursor=cursor->next;
	}
	printf("\n");

}

/*
	Frees a "poly" and its "nodes" from memory.
*/
void poly_free(poly **P) 
{
	node *cursor = (*P)->terms;
	node *temp ;
	int i;
	for (i=0; i<(*P)->num_terms; i++)	//loops through terms then cleans and frees them
	{
		temp = cursor;
		cursor = cursor->next;
		temp = NULL;
		free(temp);
		
	}
	*P = NULL;		//cleans and frees "poly"
	free(*P);

}

/*
	Takes in a "poly" and duplicates it into a new "poly" without harming the original.
*/
poly *poly_duplicate(poly *P ) 
{
	poly *P2 = (poly*)malloc(sizeof(poly));		//allocates memory for the new "poly" and copies the number of terms
	P2->num_terms = P->num_terms; 
	int i;
	node *cursor = (node*)malloc(sizeof(node));
	cursor = P->terms;
	node *cursor2 = (node*)malloc(sizeof(node)); 	//creates and allocates memory for cursor to traverse through nodes
	P2->terms = cursor2;
	for (i=0; i<P->num_terms; i++)
	{
		cursor2->coef = cursor->coef;			//loops through nodes and copies values from original to duplicate 
		cursor2->powr = cursor->powr;

		cursor2->next = (node*)malloc(sizeof(node)); 		//allocates memory for the next node to be filled, then advances
		cursor2 = cursor2->next;
		cursor = cursor->next;			//advances to the next node to be copied
	}
	return P2;
}

/*
	Adds two "poly"s together without harming each of the originals.
*/
poly *poly_add(poly *P1, poly *P2) 
{
	poly* result;
	node *c, *cr;
	if (P1->num_terms > P2->num_terms)
	{
		result = poly_duplicate(P1); 	//duplicate the first "poly" for altering

		c = P2->terms;		//cursor for traversing through "P2"
		cr = result->terms; 	//cursor for traversing through "result"
	}

	else
	{
		result = poly_duplicate(P2); 	//duplicate the first "poly" for altering

		c = P1->terms;		//cursor for traversing through "P1"
		cr = result->terms; 	//cursor for traversing through "result"
	}
	while(c->next != NULL)		//loops through "P2" 
	{
		while(cr->next != NULL)		//loops through "result"
		{
			if (cr->powr == c->powr)	//checks powers of "result" against "P2"
			{							//combines if they are equal, then breaks out of loop
				cr->coef+=c->coef;
				break;
			}
			if (cr->next->next == NULL || cr->next == NULL)		//advance if there is enough space to
			{
				result->num_terms++;
				cr->next = (node*)malloc(sizeof(node));
				cr->next->coef = c->coef;
				cr->next->powr = c->powr;
			}
			cr = cr->next;
		}
		c = c->next;
		cr = result->terms;
	}

	poly_sort(result); 		//sort the result into descending powers
	
	return result;
}

/*
	Creates a "poly" with a user-defined number of terms.
*/
poly *poly_create(int num,...) 
{
	va_list list;		//creates a list
	va_start(list, num);	//starts a list with the size being the user-defined value
	poly *p = (poly*)malloc(sizeof(poly)); 		//assign cursors and allocate memory
	p->num_terms = num;
	node *cursor;
	cursor = (node*)malloc(sizeof(node));
	p->terms = cursor;
	cursor->next = NULL;
	int i;
	for (i=0; i<num; i++)		//loops through list and places coefficients and powers into the corresponding variables
	{
		cursor->coef = va_arg(list, int);
		cursor->powr = va_arg(list, int);


		cursor->next = (node*)malloc(sizeof(node));
		cursor = cursor->next;
	}
	
	va_end(list);		//close list
	poly_sort(p);		//sort terms into descending powers
	return p;
}

/*
	Scales a "poly" by a user-defined value by multiplying the coefficient of each of its terms.
*/
poly *poly_scalar_mult(poly *P, int x) 
{
	poly *P2 = poly_duplicate(P);		//creates duplicate "poly" to protect P.
	node *cursor = (node*)malloc(sizeof(node));
	cursor = P2->terms;
	int i;
	for (i = 0; i<P2->num_terms; i++)		//loop through and multiply all the coefficients by x
	{
		cursor->coef*=x;
		cursor = cursor->next;
	}
	return P2;
}

/*
	Sorts a "poly"'s terms, descending in terms of their powers.
*/
void poly_sort(poly *p)
{
	int vals[p->num_terms][2];		//2D array for storing values for sorting
	node *cursor = p->terms;
	int i;
	int cache0 = 0, cache1 = 0;		//caches for swapping values

	for (i = 0; i<p->num_terms; i++)		//loops through and places powers and coeffiecients into array
	{
		vals[i][0] = cursor->powr;
		vals[i][1] = cursor->coef;
		cursor = cursor->next;
	}

	int flag = 1;
	while (flag == 1)		//loops through and sorts the array in descending powers with bubble sort
	{
		flag = 0;
		for (i=0; i<p->num_terms-1; i++)
			if (vals[i][0]< vals[i+1][0])
			{
				cache0 = vals[i][0];
				cache1 = vals[i][1];

				vals[i][0] = vals[i+1][0];
				vals[i][1] = vals[i+1][1];

				vals[i+1][0] = cache0;
				vals[i+1][1] = cache1; 
				flag = 1;
			}
	}

	cursor = p->terms;
	for (i = 0; i<p->num_terms; i++) 		//loops through terms in "poly" and places them back in a sorted order
	{
		cursor->powr = vals[i][0];
		cursor->coef = vals[i][1];
		cursor = cursor->next;
	}

}

/*
	Checks to see if a "poly" has been sorted in descending order, in terms of powers.
*/
int check_sort(poly *p)
{
	int i;
	node *cursor = p->terms;
	for (i = 0; i<p->num_terms-1; i++)	//loops through to check order
	{
		if (cursor->powr < cursor->next->powr)		//if a power is higher than its predecessor, it is not sorted
			return 0;
		cursor = cursor->next;		//continue traversing
	}
	return 1;		//if all powers have been in descending order, it is sorted

}
