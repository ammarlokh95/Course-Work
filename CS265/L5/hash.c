/*
 * hash.c - quick example of a hash table (w/a really stupid hash function)
 *
 * Kurt Schmidt
 * Feb '06
 *
 * EDITOR:
 *	tabstop=2
 *	cols=80
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TABLE_SIZE  7
#define NUM_INPUTS  7

int hash( char *s )
	/* Note, this is a horrible hash function.  It's here for 
		instructional purposes */
{
	return strlen( s ) % TABLE_SIZE ;
}

typedef struct entry
{
	char *key;
	int	val;
	struct entry *next;
} entry;

entry* table[ TABLE_SIZE ] = { NULL };

int find( char *key, int *p_ans)
{
	int h = hash( key );
	entry *t=NULL;
	t=table[h];
	while (t != NULL)
	{
		if (strcmp(t->key, key)==0)
		{
			*p_ans= t->val;
			return 1;
		}
		t=t->next;
	}
	return 0;
}

void insert( char *s, int v )
	/* this insert is NOT checking for duplicates.  :/ */
{
	int h = hash( s );
	entry *t = (entry*) malloc( sizeof( entry ));

	t->key = s;
	t->val = v;
	t->next = table[h];
	table[h] = t;
}

void clean_table()
{
	entry *p, *q;
	int i;

	for( i=0; i<TABLE_SIZE; ++i )
	{
		for( p=table[i]; p!=NULL; p=q )
		{
			q = p->next;
			free( p );
		}
	}	// for each entry
}	// clean_table


int main()
{
	char* keyList[] = { "Jaga", "Jesse", "Cos", "Kate", "Nash", "Vera",
		"Bob" };

	int valList[] = { 24, 78, 86, 28, 11, 99, 38 };

	int i;

	for( i=0; i<NUM_INPUTS; ++i )
		insert( keyList[i], valList[i] );

	/* what does the table look like here? */
char *name = "Bob";
int data;
if ( find( name, &data))
	   printf( "Found %s.  (S)he's %i\n\n", name, data );
else
	   printf( "\nCouldn't find %s\n\n", name );
	
clean_table();

	return( 0 );
}
