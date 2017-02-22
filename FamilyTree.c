/*
This program tells you the realtion between members in the family.
Enter family name and two member names between which you will like you to know the realtion.
Program reads the FamiliesSource.txt file and builds the family tree and uses it to derive the relation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 30
struct node
{
	char *parent;
	char *name;
	struct node *child;
	struct node *sibling;
}*root;

struct stack
{
	int top;
	struct node *item[SIZE];
}s;

FILE *fp;

void Create_Families(); /*creates the family, by searching founders in txt file*/

void Add_Members(); /*add members to family*/
struct node* Find_Parent (struct node*, char*); /*returns position of parent node, it is used in Add_Members()*/

void Display();
void Display_Pre(struct node*); /*called by Display(), to print tree pre-orderly*/

void Search ();
void Search_Names(struct node*,char*,char*); /*called by Search()*/

main()
{
	fp = fopen ("FamiliesSource.txt","r");
	if (fp==NULL) { printf ("cannot open"); return 0;}

	root = (struct node *) malloc (sizeof(struct node));
	root->child = root->sibling =  NULL;
	root->parent = root->name = NULL ;
	
	printf("Creating families\n\n");
	Create_Families();
	Add_Members();
	Display();

	printf("Search the people and their relation\n\n");
	Search();

	free (root);
	fclose (fp);
	return 0;
}

void Create_Families ()  
{
	struct node *temp,*q;	/*q is traverse though tree for adding nodes*/
	char *buff;
	buff = (char *) malloc (30); 
	int ret_val, compare;

	ret_val = fscanf (fp,"%s",buff);
	while ( ret_val != EOF )
	{
		compare = strcmp("founder:",buff);

		if (compare==0)
		{
			ret_val = fscanf (fp,"%s",buff);

			/*creating new node*/
			temp = (struct node *) malloc (sizeof(struct node));
			temp->child=temp->sibling=NULL;
			temp->name = (char *) malloc(strlen(buff)+1);
			temp->parent = (char *) malloc(strlen(buff)+1);
			strcpy (temp->parent,"root");
			strcpy (temp->name,buff);

			if (root->child==NULL)	root->child = q = temp;

			else 
			{
				q->sibling = temp ;
				q = q->sibling;
			}
		}

		ret_val = fscanf (fp,"%s",buff);

		if ( strcmp("ENDfounders",buff)==0 ) break;
	}
	
}

void Add_Members ()
{
	struct node *q,*temp,*p;
	int ret_val;
	char *temp_founder,*temp_parent,*temp_child,*buff;

	temp_founder= (char *)malloc(20);
	temp_parent = (char *)malloc(20);
	temp_child = (char *)malloc(20);	
	buff = (char *)malloc(20);
	
	ret_val = fscanf (fp,"%s",buff);
	while(ret_val!=EOF)
	{
		/*temporarily storing the information of founder,parent,child*/
		strcpy (temp_founder,buff);
		ret_val = fscanf (fp,"%s",buff);
		strcpy (temp_parent,buff);
		ret_val = fscanf (fp,"%s",buff);
		strcpy (temp_child,buff);

		/*creating a node of tree */
		temp = (struct node *) malloc (sizeof(struct node));
		temp->child = temp->sibling = NULL;
		temp->name = (char *) malloc(strlen(temp_child)+1);
		temp->parent = (char *) malloc(strlen(temp_parent)+1);
		strcpy (temp->parent, temp_parent);
		strcpy (temp->name, temp_child);

		/*if child is of 2nd generation in a family*/
		if ( strcmp("root",temp_founder)==0 )  
		{
			q=root->child;
			while ( strcmp(q->name,temp_parent)!=0 )  q = q->sibling ;
			
			if (q->child==NULL) /*if child is 1st child of parent*/
			{
				q->child = temp;
				q = q->child;	
			}
			else
			{
				q = q->child;
				while(q->sibling!=NULL) q = q->sibling;

				q->sibling = temp ;
				q=q->sibling;
			}			
		}

		else	/*i.e if child is of 3rd or higher generation in a family*/
		{
			q = root->child;

			/*traversing till family is found*/
			while ( strcmp(q->name,temp_founder)!=0 )  q = q->sibling ;	

			/*pre-orderly traversing the family to find the parent*/
			p = Find_Parent(q, temp_parent);

			/*if child is 1st child of parent*/
			if (p->child==NULL) 
			{
				p->child = temp ;
				p = p->child;
			}

			else
			{
				p = p->child;
				while (p->sibling!=NULL) p = p->sibling;

				p->sibling = temp ;
				p=p->sibling;
			}
		}
		
		ret_val = fscanf (fp,"%s",buff);
	}
}

struct node* Find_Parent (struct node *r, char *target)
{
	s.item[++s.top]=r;
	while (s.top!=-1)
	{
		r=s.item[s.top--];
		
		if (r!=NULL)
		{
			if ( strcmp(r->name, target)==0 )	return r;
			s.item[++s.top] = r->sibling;
			s.item[++s.top] = r->child;
		}
	}
} 


void Display()
{
	struct node *q=root->child;
		Display_Pre (q);		
}
void Display_Pre (struct node *r)
{
	if (r!=NULL)
	{
		if ( strcmp(r->parent,"root")==0 )	printf("Showing %s family\n\n", r->name);
		else printf("Parent: %s || Child: %s\n", r->parent, r->name);
		Display_Pre (r->child);
		Display_Pre (r->sibling);
		printf ("\n");
	}	
}

struct node *name1,*name2, *parent1,*parent2; /*these are used in following search functions*/
char *family_name1, *family_name2;

void Search ()
{
	int choice=1;
	char *temp_family,*temp1,*temp2;
	struct node *q;

	/*these following variables takes input from user*/
	temp_family = (char *)malloc(20);
	temp1 = (char *)malloc(20);
	temp2 = (char *)malloc(20);

	while (choice==1)
	{
		q=root->child;

		printf("\nEnter name of the family: ");	scanf ("%s",temp_family);
		printf("Enter a name to Search: "); scanf ("%s",temp1);
		printf("Enter another name to Search: ");  scanf ("%s",temp2);

		/*traversing till family is found*/
		while ( q!=NULL )  
		{
			if ( strcmp(temp_family,q->name)==0 )	break;
			q = q->sibling ;	
		}

		if (q==NULL) 
		{
			printf("\nFamily not found\n");
		}
		else
		{
			Search_Names (q->child, temp1, temp2);

			if ( name1==NULL || name2==NULL )
			{
				printf("\nNames not found in the family\n\n");
			}

			else 
			{
				if ( strcmp(name1->parent, name2->parent)==0 )
				{
					printf("\n%s & %s are siblings\n\n", name1->name, name2->name);
				}
				else if ( strcmp(name1->name, name2->parent)==0 )
				{
					printf("\n%s is parent of %s\n\n", name1->name,name2->name);
				}
				else if ( parent1!=NULL && parent2!=NULL && strcmp(parent1->parent, parent2->parent)==0  )
				{				
						printf("\n%s & %s are cousins\n\n",  name1->name,name2->name);				
				}
				else
				{
					printf("\n%s is ancestor of %s\n\n", name1->name,name2->name);
				}
			}
		}

		printf("Do you want to Search another names, if YES press 1, else 0 : ");	scanf("%d",&choice);
	}
}

/*following function searches (levelwise) the names in the family tree, 
uses the queue, 
and stores the information in nodes name1 & name2 (if names are found in family tree) 
name found first is stored in name1, 
name found second is stored in name 2

parent1,parent2 stores the information of parents name1,name2 respectively*/

void Search_Names ( struct node *r, char *s1, char *s2 )
{
	struct node *queue[SIZE];
	int front,rear,FLAG=1;

	front=rear=-1;
	name1=NULL;
	name2=NULL;
	parent1=parent2=NULL;

	queue[++rear]=r;
	++front;
		
	while (front<=rear)
	{
		r = queue[front];
		
		if (r!=NULL)
		{
			
			if ( strcmp(r->name,s1)==0  || strcmp(r->name,s2)==0 )
			{
				if (FLAG==1)
				{
					name1 = r;
					FLAG=0;
				}
				else
				{
					name2 = r;
					break;
				}
			}
	
			if (r->sibling!= NULL) queue[++rear] = r->sibling;
			if (r->child != NULL) queue[++rear] = r->child;	
		}
		++front;	
	}	

}
