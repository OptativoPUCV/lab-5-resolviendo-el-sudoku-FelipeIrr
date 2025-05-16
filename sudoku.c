#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int i,j,k;
    int filas[9][10]={0};
    int columnas[9][10]={0};
    int box[9][10]={0};
    for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(n->sudo[i][j]!=0){
             k=n->sudo[i][j];
             if(filas[i][k] || columnas[j][k] || box[(i/3)*3+j/3][k]){
                return 0;
             }
             filas[i][k]=1;
             columnas[j][k]=1;
             box[(i/3)*3+j/3][k]=1;
          }
       }
    }
    return 1;
}


List* get_adj_nodes(Node* n){
    List* list=createList();
    Node* new;
    for(int i=0;i<9;i++){
       for(int j=0;j<9;j++){
          if(n->sudo[i][j]==0){
             for(int k=1;k<=9;k++){
                new=copy(n);
                new->sudo[i][j]=k;
                if(is_valid(new)){
                   pushBack(list, new);
                }else{
                   free(new);
                }
             }
             return list;
          }
       }
    }
    return list;
}


int is_final(Node* n){
    for(int i=0;i<9;i++){
       for(int j=0;j<9;j++){
          if(n->sudo[i][j]==0){
             return 0;
          }
       }
    }
    return 1;
}

Node* DFS(Node* initial, int* cont){
    Stack* stack=createStack();
    push(stack, initial);
    Node* current;
    List* adj;
    while(!is_empty(stack)){
       current=top(stack);
       pop(stack);
       (*cont)++;
       if(is_final(current)){
          return current;
       }
       adj=get_adj_nodes(current);
       for(Node* n=first(adj);n!=NULL;n=next(adj)){
          push(stack, n);
       }
       clean(adj);
    }
    clean(stack);
  return NULL;
}




int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}