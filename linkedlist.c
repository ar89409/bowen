#include <stdio.h>
#include <stdib.h>

struct node{
	int data;
	struct node *next;
};
typedef struct node Node;

int main(int argc, char *argv[])
{
 	int i,val,num;
	Node *first,*current,*previous;
	printf("Number of nodes: ");
	scant("%d" ,&num);

for (i=0;i<num;i++){
	current=(Note *) malloc(sizeof(Node));
	if(i==0){
			first=current;
}
	else{
	        previous->next=current;
        }
	current->next=NULL;
	previous=current;
}

	current=first;
	while(current!=NULL){
		printf("address=%p, ",current->data);
		printf("data=%d ",current->next);
		printf("next=%p\n",current->next);
		current=current->next;
	}
	system("PAUSE");
	return 0:
}
