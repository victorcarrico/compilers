#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define HEAP_SIZE 1000
#define INT_MAX 1000
#define NUM_MAX 1000
#define ARITIMEC_OP_MAX 1000

char input[HEAP_SIZE];
int stack_point = 0; //TIREI DA MAIN
int comp_point = 0;
int tail_stack_point = 0;
char int_string[INT_MAX];
int int_string_point = 0;
// int stack_number[NUM_MAX];
// int stack_num_point = 0;
// char stack_aritmetic_op[ARITIMEC_OP_MAX];
// int stack_arit_op_point = 0;

typedef struct node
{
    char type;
    char management;
    int number;
    struct node* right;
    struct node* left;
} Node;

Node* stack[9000000];
Node* comp_stack[9000];
Node* tail_stack[9000000];
Node* reduce_graph(Node* root, int r);

int get_pair(int start){
    /*
        It returns the lenght of the parameter.
    */

    int index, parentheses;

    index = start - 1;
    parentheses = 0;

    do{
        index++;
        if (input[index] == '(' || input[index] == '['){
            parentheses++;
        }else if(input[index] == ')' || input[index] == ']'){
            parentheses--;
        }
    }while(parentheses);

    return index;
}

int get_last_arg(int start, int end) {
    /* It returns the open
        parentheses of last arg */

    int index, parentheses;

    index = end + 1;
    parentheses = 0;
    do
    {
        index--;
        if (input[index] == ')' || input[index] == ']')
            parentheses--;
        else if (input[index] == '(' || input[index] == '[' )
            parentheses++;
    } while (parentheses);

    // if (input[index] == ',') // Discard list comma
    //     index--;

    return index;
}

Node* make_leaf(char type){
    /*
        It returns a node with node with
        right and left pointers null.
    */

    Node* node;

    node = (Node*) malloc(sizeof(Node));
    node->type = type;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node* make_Nleaf(int start, int end){

    /*
        It returns n:
        NULL<-n->(NULL<-type->NULL).
    */

    Node *node;
    int i, number;
    char str_aux[10];

    // Coping number to str_aux
    for (i=start;i <= end; i++){
        str_aux[i-start] = input[i];
        str_aux[i-start+1] = '\0';
    }

    number = atoi(str_aux);

    node = (Node*) malloc(sizeof(Node));

    node->type = 'n';
    node->number = number;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node* make_graph(int start, int end){
    /*
        Recursive function:
        It returns the root node
        of each subgraph
    */
    Node *node;
    int last_arg;
    char aux_int = 'x';

    last_arg = get_last_arg(start, end);

    if (input[start] == '(' && input[end] == ')' && end == get_pair(start)) {
        node = make_graph(start+1, end-1);
    }
    else if (input[start] == '[' && input[end] == ']' && end == get_pair(start)) {
        node = (Node*) malloc(sizeof(Node));
        node->type = '@';
        node->left = make_leaf(':');
        node->right = make_graph(start+1, end-1);
    }
    else if (last_arg == start || (input[start] >= 48 && input[start] <=57)) {
        if (input[start] >= 48 && input[start] <= 57)
            node = make_Nleaf(start,last_arg);
        else
            node = make_leaf(input[start]);
    }
    else {

        node = (Node*) malloc(sizeof(Node));
        node->type = '@';

        // if (input[end] == '+' || input[end] == '-'){
        //     node->left = make_leaf(input[end]);
        //     node->right = make_graph(start, end-1);
        // }
        // else {
            node->left = make_graph(start, last_arg-1);
            node->right = make_graph(last_arg, end);
        // }
    }

    return node;

}

int stack_update (int stack_point)  {
    /*
        It updates the stack after
        a reduction.
    */
    Node* aux;
    aux = stack[stack_point]->left;

    while (aux != NULL) {
        stack_point = stack_point + 1;
        stack[stack_point] = aux;
        aux = aux->left;
    }

    return stack_point;
}

void print_graph(Node* node) {
    if (node->left) {
        printf("(");
        print_graph(node->left);
        printf(")");
    }
    if (node->type != '@'){
        if (node->type == 'n')
            printf("%d", node->number);
        else
            printf("%c", node->type);
    }
    if (node->right) {
        printf("(");
        print_graph(node->right);
        printf(")");
    }
}

void transform_input() {

    /*
        It changes 2 character operators
        to 1 character lower cased operator.
    */

    unsigned int index_b = 0, index_a = 0, index_up = 0;

    while (input[index_b] != '\0')
    {
        while (((input[index_b + 1] != '\'' && input[index_b + 1] != 'd' && input[index_b + 1] != 'l') || (input[index_b] != 'S' && input[index_b] != 'B' && input[index_b] != 'C' && input[index_b] != 'H' && input[index_b] != 'T')) && (input[index_b] != '\0'))
            index_b = index_b + 1;

        memcpy (input + index_up, input + index_a, index_b - index_a);
        index_up = index_up + (index_b - index_a);

        if (input[index_b + 1] == '\'') {
            if (input[index_b] == 'S')  {
                memcpy (input + index_up, "s", 1);
                index_up = index_up + 1;
                index_b = index_b + 2;
                index_a = index_b;
            }
            else if (input[index_b] == 'B') {
                memcpy (input + index_up, "b", 1);
                index_up = index_up + 1;
                index_b = index_b + 2;
                index_a = index_b;
            }
            else if (input[index_b] == 'C') {
                memcpy (input + index_up, "c", 1);
                index_up = index_up + 1;
                index_b = index_b + 2;
                index_a = index_b;
            }
        }else{
            if (input[index_b] == 'H')  {
                memcpy(input + index_up, "h", 1);
                index_up = index_up + 1;
                index_b = index_b + 2;
                index_a = index_b;
            }
            else if (input[index_b] == 'T') {
                memcpy(input + index_up, "t", 1);
                index_up = index_up + 1;
                index_b = index_b + 2;
                index_a = index_b;
            }
        }
    }

    memcpy (input + index_up, "\0", 1);
}


Node* list_reduction (Node* node) {
    Node *aux, *aux2;

    aux = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));

    aux = node;

    if (stack[stack_point-2]->right->type == ':')   {
        aux2->left = stack[stack_point-1];
        aux2->right = stack[stack_point-3]->right;
        aux->left->type = ':';
        aux->right = aux2;
    }

    return aux;

}

Node* head_reduction(Node* node)    {

    Node *aux;

    aux = (Node*) malloc(sizeof(Node));
    aux = node;

    while (node->left != NULL){
       node = node->left;
       if (node->right->type == ','){
            aux = node->left;
       }
    }

    return aux;
}

Node* tail_reduction(Node* node)    {

    Node *aux, *aux2, *aux3;
    int comma = 0;

    aux = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));
    aux3 = (Node*) malloc(sizeof(Node));

    //aux = node;

	if (node->left->type == 't'){
        aux = tail_reduction(node->right);
		node->right = aux;
		node = aux;
    }

    if (node->left->type == ':'){
        aux2 = node->right;
        aux3 = aux2;
        while (aux3->left != NULL){
            if (aux3->left->right->type == ','){
                aux2 = aux3;
            }
            aux3 = aux3->left;
        }
        aux2->left = NULL;
    }

   // else
     //   printf("Tail de nao lista.\n");

    printf("Saida do tail_reduction\n");
    print_graph(node);
    return node;
}

Node* I_reduction (Node* node)
{
    return node->right;
}

Node* Cl_reduction (Node* node)
{
    Node *a, *b, *c, *d;
    Node *aux1, *aux2, *aux3;

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));
    aux3 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'a', 'b', 'c' e 'd', respectivamente
    a = node->left->left->left->right;
    b = node->left->left->right;
    c = node->left->right;
    d = node->right;


    aux1->type = '@';
    aux1->left = b;
    aux1->right = d;

    aux2->type = '@';
    aux2->left = a;
    aux2->right = aux1;

    aux3->type = '@';
    aux3->left = aux2;
    aux3->right = c;

    return aux3;
}

Node* C_reduction (Node* node)
{
    Node *f, *x, *y;
    Node *aux1, *aux2;

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'f', 'x' e 'y', respectivamente
    f = node->left->left->right;
    x = node->left->right;
    y = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução C (ou toda a redução para 'aux2')
    aux1->type = '@';
    aux1->left = f;
    aux1->right = y;

    aux2->type = '@';
    aux2->left = aux1;
    aux2->right = x;

    return aux2;
}

Node* Bl_reduction (Node* node)
{
    Node *a, *b, *c, *d;
    Node *aux1, *aux2, *aux3;

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));
    aux3 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'a', 'b', 'c' e 'd', respectivamente
    a = node->left->left->left->right;
    b = node->left->left->right;
    c = node->left->right;
    d = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução B' (ou toda a redução para 'aux3')
    aux1->type = '@';
    aux1->left = a;
    aux1->right = b;

    aux2->type = '@';
    aux2->left = c;
    aux2->right = d;

    aux3->type = '@';
    aux3->left = aux1;
    aux3->right = aux2;

    return aux3;
}

Node* B_reduction (Node* node)      //Redução B
{
    Node *f, *g, *x;                //Ponteiros que armazenarão os endereços das expressões f, g e x
    Node *aux1, *aux2;              //Ponteiros que armazenarão os endereços das expressões (gx) e (f(gx)), respectivamente

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'f', 'g' e 'x', respectivamente
    f = node->left->left->right;
    g = node->left->right;
    x = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução B (ou toda a redução para 'aux2')
    aux1->type = '@';
    aux1->left = g;
    aux1->right = x;

    aux2->type = '@';
    aux2->left = f;
    aux2->right = aux1;

    return aux2;                    //'aux2' tem o endereço inicial de toda a redução B
}

Node* Sl_reduction (Node* node)     //Redução S'
{
    Node *a, *b, *c, *d;                //Ponteiros que armazenarão os endereços das expressões a, b, c e d
    Node *aux1, *aux2, *aux3, *aux4;    //Ponteiros que armazenarão os endereços das expressões (bd), (a(bd)), (cd) e (a(bd)(cd)), respectivamente

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));
    aux3 = (Node*) malloc(sizeof(Node));
    aux4 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'a', 'b', 'c' e 'd', respectivamente
    a = node->left->left->left->right;
    b = node->left->left->right;
    c = node->left->right;
    d = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução S' (ou toda a redução para 'aux4')
    aux1->type = '@';
    aux1->left = b;
    aux1->right = d;

    aux2->type = '@';
    aux2->left = a;
    aux2->right = aux1;

    aux3->type = '@';
    aux3->left = c;
    aux3->right = d;

    aux4->type = '@';
    aux4->left = aux2;
    aux4->right = aux3;

    return aux4;                    //'aux4' tem o endereço inicial de toda a redução S'
}

Node* S_reduction (Node* node)      //Redução S
{
    Node *a, *b, *c;                //Ponteiros que armazenarão os endereços das expressões a, b e c
    Node *aux1, *aux2, *aux3;       //Ponteiros que armazenarão os endereços das expressões (ac), (bc) e ((ac)(bc)), respectivamente

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));
    aux2 = (Node*) malloc(sizeof(Node));
    aux3 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'a', 'b' e 'c', respectivamente
    a = node->left->left->right;
    b = node->left->right;
    c = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução S (ou toda a redução para 'aux3')
    aux1->type = '@';
    aux1->left = a;
    aux1->right = c;

    aux2->type = '@';
    aux2->left = b;
    aux2->right = c;

    aux3->type = '@';
    aux3->left = aux1;
    aux3->right = aux2;

    return aux3;                    //'aux3' tem o endereço inicial de toda a redução S
}

Node* Y_reduction (Node* node)      //Redução S
{
    Node *F;                //Ponteiros que armazenarão os endereços das expressões a, b e c
    Node *aux1;       //Ponteiros que armazenarão os endereços das expressões (ac), (bc) e ((ac)(bc)), respectivamente

    //Alocando espaço para cada nó auxiliar
    aux1 = (Node*) malloc(sizeof(Node));

    //Retornam os endereços dos caminhos desde a raíz até as expressões 'a', 'b' e 'c', respectivamente
    F = node->right;

    //O type de cada nó é '@', porém cada nó guarda dois subgrafos que seriam uma parte da redução S (ou toda a redução para 'aux3')
    aux1->type = '@';
    aux1->left = F;
    aux1->right = node;

    return aux1;                    //'aux3' tem o endereço inicial de toda a redução S
}

Node* K_reduction (Node* node)      //Redução K
{
    return node->left->right;       //Retorna o endereço do caminho desde a raíz até a expressão 'a'
}

int gt_operator(int n1, int n2) {
    int ret;
    if (n1 > n2)
        ret = 1;
    else
        ret = 0;

    return ret;
}

int lt_operator(int n1, int n2) {
    int ret;
    if (n1 < n2)
        ret = 1;
    else
        ret = 0;

    return ret;
}

int eq_operator(int n1, int n2) {
    int ret;
    if (n1 == n2)
        ret = 1;
    else
        ret = 0;

    return ret;
}

Node* comp_reduction  (Node* node, char op)  {
    Node *aux, *aux2;
    int num1, num2;
    int boolean;
    aux = node;

    if (aux->right->type == 'n')
        num1 = aux->right->number;
    else{
        aux2 = aux->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        num1 = aux2->number;
    }

    if (aux->left->right->type == 'n')
        num2 = aux->left->right->number;
    else{
        aux2 = aux->left->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        num2 = aux2->number;
    }

    switch (op){
        case '>':
            boolean = gt_operator(num2, num1);
            break;
        case '<':
            boolean = lt_operator(num2, num1);
            break;
        case '=':
            boolean = eq_operator(num2, num1);
            break;
        default:
            printf("Operador deve ser >, < ou =\n");
            break;
    }

    if (boolean)
        aux = stack[stack_point - 3]->right;
    else
        aux = stack[stack_point - 4]->right;

    return aux;
}

Node* sub_reduction (Node* node)
{
    Node *aux, *sub, *sub_aux, *aux2;
    int n1, n2, res;
    char num1, num2, res_char, debug;

    sub = (Node*) malloc(sizeof(Node));
    sub_aux = (Node*) malloc(sizeof(Node));
    aux = node;

    if (aux->right->type == 'n')
        n1 = aux->right->number;
    else{
        aux2 = aux->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        n1 = aux2->number;
    }

    if (aux->left->right->type == 'n')
        n2 = aux->left->right->number;
    else{
        aux2 = aux->left->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        n2 = aux2->number;
    }

    res = n2 - n1;

    sub_aux->type = 'n';
    sub_aux->number = res;
    sub_aux->right = NULL;
    sub_aux->left = NULL;

    return sub_aux;
}

Node* add_reduction (Node* node)
{
    Node *aux, *sum, *sum_aux, *aux2;
    int n1, n2, res;
    char num1, num2, res_char;

    sum = (Node*) malloc(sizeof(Node));
    sum_aux = (Node*) malloc(sizeof(Node));
    aux = node;

    if (aux->right->type == 'n')
        n1 = aux->right->number;
    else{
        aux2 = aux->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        } while(aux2->type != 'n');
        n1 = aux2->number;
    }

    if (aux->left->right->type == 'n')
        n2 = aux->left->right->number;
    else{
        aux2 = aux->left->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        } while(aux2->type != 'n');
        n2 = aux2->number;
    }

    res = n1 + n2;

    sum_aux->type = 'n';
    sum_aux->number = res;
    sum_aux->right = NULL;
    sum_aux->left = NULL;

    return sum_aux;

}

Node* mul_reduction (Node* node)
{
    Node *aux, *mul, *mul_aux, *aux2;
    int n1, n2, res;
    char num1, num2;

    mul = (Node*) malloc(sizeof(Node));
    mul_aux = (Node*) malloc(sizeof(Node));
    aux = node;

    if (aux->right->type == 'n')
        n1 = aux->right->number;
    else{
        aux2 = aux->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        } while(aux2->type != 'n');
        n1 = aux2->number;
    }

    if (aux->left->right->type == 'n')
        n2 = aux->left->right->number;
    else{
        aux2 = aux->left->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        } while(aux2->type != 'n');
        n2 = aux2->number;
    }

    res = n1 * n2;

    mul_aux->type = 'n';
    mul_aux->number = res;
    mul_aux->right = NULL;
    mul_aux->left = NULL;

    return mul_aux;

}

Node* div_reduction (Node* node)
{

    Node *aux, *div, *div_aux, *aux2;
    int n1, n2,res;
    char num1, num2;

    div = (Node*) malloc(sizeof(Node));
    div_aux = (Node*) malloc(sizeof(Node));
    aux = node;

    if (aux->right->type == 'n')
        n1 = aux->right->number;
    else{
        aux2 = aux->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        n1 = aux2->number;
    }

    if (aux->left->right->type == 'n')
        n2 = aux->left->right->number;
    else{
        aux2 = aux->left->right;
        do{
            aux2 = reduce_graph(aux2, 1);
        }while(aux2->type != 'n');
        n2 = aux2->number;
    }

    //printf("N1 = %d\nN2 = %d\n", n1, n2);
    res = n2 / n1;

    div_aux->type = 'n';
    div_aux->number = res;
    div_aux->right = NULL;
    div_aux->left = NULL;

    return div_aux;

}


Node* reduce_graph(Node* root, int r)  {

    Node* aux, *rec;
    int check = 1;
    char num1, num2, debug;
    int n1, n2, res;

    rec = (Node*) malloc(sizeof(Node));
    rec->type = 'r';
    if (r == 1){
        stack[stack_point] = rec;
        stack_point++;
    }

    aux = root;

    while (aux != NULL) {
        stack[stack_point] = aux;
        aux = aux->left;
        stack_point = stack_point + 1;
    }
    stack_point = stack_point - 1;

    while (check != 0)  {
        switch (stack[stack_point]->type)   {
            case 'K':
                if (stack_point - 2 >= 0)   {
                    aux = K_reduction(stack[stack_point - 2]);


                    /* It points the rest of graph
                        to the K reducted sub-graph */
                    if ((stack_point - 3) >= 0 && stack[stack_point - 3]->type != 'r'){
                        stack[stack_point - 3]->left = aux;
                    }
                    else
                        root = aux;

                    if (stack_point < 3)
                        stack_point = 3;
                    if (stack[stack_point - 3]->type == 'r')
                        stack_point = stack_point - 3;
                    else
                        stack_point = stack_point - 2;


                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;

            // printf("KOPERATORRRRR\n");
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'S':
                if (stack_point - 3 >= 0)
                {
                    // scanf("%c", &debug);
                    aux = S_reduction(stack[stack_point - 3]);

                    if ((stack_point - 4) >= 0 && stack[stack_point - 4]->type != 'r')
                        stack[stack_point - 4]->left = aux;
                    else
                        root = aux;

                    if (stack_point < 4)
                        stack_point = 4;
                    if (stack[stack_point - 4]->type == 'r')
                        stack_point = stack_point - 4;
                    else
                        stack_point = stack_point - 3;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else{
                    check = 0;
                }
            // print_graph(root);
            // printf("\n\n");
            break;
            case 's':
                if (stack_point - 4 >= 0)
                {
                    aux = Sl_reduction(stack[stack_point - 4]);

                    if ((stack_point - 5) >= 0)
                        stack[stack_point - 5]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 4;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'B':
                if (stack_point - 3 >= 0)
                {
                    aux = B_reduction(stack[stack_point - 3]);

                    if ((stack_point - 4) >= 0)
                        stack[stack_point - 4]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 3;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'b':
                if (stack_point - 4 >= 0)
                {
                    aux = Bl_reduction(stack[stack_point - 4]);

                    if ((stack_point - 5) >= 0)
                        stack[stack_point - 5]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 4;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'C':
                if (stack_point - 3 >= 0)
                {
                    aux = C_reduction(stack[stack_point - 3]);

                    if ((stack_point - 4) >= 0)
                        stack[stack_point - 4]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 3;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'c':
                if (stack_point - 4 >= 0)
                {
                    aux = Cl_reduction(stack[stack_point - 4]);

                    if ((stack_point - 5) >= 0)
                        stack[stack_point - 5]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 4;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'I':
                if (stack_point - 1 >= 0)   {
                    aux = I_reduction(stack[stack_point - 1]);

                    if ((stack_point - 2) >= 0 && stack[stack_point - 2]->type != 'r'){
                        stack[stack_point - 2]->left = aux;
                        // stack[stack_point - 2]->type = '@';
                    }
                    else
                        root = aux;


                    if (stack_point < 2)
                        stack_point = 2;
                    if (stack[stack_point - 2]->type == 'r')
                        stack_point = stack_point - 2;
                    else
                        stack_point = stack_point - 1;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);

                }
                else{
                    check = 0;
                }
            // print_graph(root);
            // printf("\n\n");
            break;
            case ':':
                if (stack_point - 2 >= 0)   {
                    aux = list_reduction(stack[stack_point]);

                    if (stack[stack_point - 3] >= 0)
                        stack[stack_point - 3]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 2;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'h':
                if (stack_point - 1 >= 0){
                    if (stack[stack_point - 1]->right->left->type == ':' || stack[stack_point - 1]->right->left->type == 't'){
                        if (stack[stack_point - 1]->right->left->type == ':'){
                            aux = head_reduction(stack[stack_point - 1]->right->right);
                        }
                        else if (stack[stack_point - 1]->right->left->type == 't'){
                            aux = tail_reduction(stack[stack_point - 1]->right->right);
                            aux = head_reduction(aux->right);
                        }
                    }else  {
                        printf("Head de nao lista.\n");
                    }

                    if (stack_point - 2 >= 0)
                        stack[stack_point - 2]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 1;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                    check = 0;  // TIRAR ISSO AQUI      ||      SEM ISSO NAO FUNCIONA!
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case 't':
                if (stack_point - 1 >= 0){
                    if (stack[stack_point - 1]->right->left->type == ':' || stack[stack_point - 1]->right->left->type == 't')
                        aux = tail_reduction(stack[stack_point - 1]->right);
                    else
                        printf("Head de nao lista.\n");


                    if ((stack_point - 2) >= 0)
                        stack[stack_point - 2]->left = aux;
                    else
                        root = aux;

                    stack_point = stack_point - 1;
                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;
            case '=':
                // printf("\ncomparacao\n");
                // print_graph(stack[stack_point-4]);
                // scanf("%c", &debug);
                if (stack_point - 2 >= 0){


                    aux = comp_reduction(stack[stack_point - 2], '=');
                    root = aux;

                    stack_point = 0;
                    print_graph(aux);
                    while (aux->left != NULL){
                        stack[stack_point] = aux;
                        aux = aux->left;
                        stack_point = stack_point + 1;
                    }
                    stack[stack_point] = aux;
                    // printf("Saiu comparacao\n");
                }
                else
                    check = 0;
            // print_graph(root);
            // printf("\n\n");
            break;

            case '>':
                if (stack_point - 2 >= 0){
                    aux = comp_reduction(stack[stack_point - 2], '>');
                    root = aux;

                    stack_point = 0;
                    while (aux->left != NULL){
                        stack[stack_point] = aux;
                        aux = aux->left;
                        stack_point = stack_point + 1;
                    }
                    stack[stack_point] = aux;
                    // printf("Saiu gt\n");
                }
                else
                    check = 0;
            break;

            case '<':
                if (stack_point - 2 >= 0){
                    aux = comp_reduction(stack[stack_point - 2], '<');
                    root = aux;

                    stack_point = 0;
                    while (aux->left != NULL){
                        stack[stack_point] = aux;
                        aux = aux->left;
                        stack_point = stack_point + 1;
                    }
                    stack[stack_point] = aux;
                    // printf("Saiu lt\n");
                }
                else
                    check = 0;
            break;
            case '+':
                if (stack_point - 2 >= 0){

                    aux = add_reduction(stack[stack_point - 2]);

                    if ((stack_point - 3) >= 0 && stack[stack_point - 3]->type != 'r'){
                        stack[stack_point - 3]->left = aux;
                    }
                    else
                        root = aux;

                    if (stack_point < 3)
                        stack_point = 3;
                    if (stack[stack_point - 3]->type == 'r')
                        stack_point = stack_point - 3;
                    else
                        stack_point = stack_point - 2;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // printf("operador +\n");
            // print_graph(root);
            // printf("\n\n");
            break;

            case '-':
                if (stack_point - 2 >= 0){

                    aux = sub_reduction(stack[stack_point - 2]);


                    if ((stack_point - 3) >= 0 && stack[stack_point - 3]->type != 'r'){
                        stack[stack_point - 3]->left = aux;
                    }
                    else
                        root = aux;

                    if (stack_point < 3)
                        stack_point = 3;
                    if (stack[stack_point - 3]->type == 'r')
                        stack_point = stack_point - 3;
                    else
                        stack_point = stack_point - 2;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // printf("\noperador -\n");
            // print_graph(root);
            // printf("\n\n");
            break;
            case '*':
                if (stack_point - 2 >= 0){

                    aux = mul_reduction(stack[stack_point - 2]);


                    if ((stack_point - 3) >= 0 && stack[stack_point - 3]->type != 'r'){
                        stack[stack_point - 3]->left = aux;
                    }
                    else
                        root = aux;

                    if (stack_point < 3)
                        stack_point = 3;
                    if (stack[stack_point - 3]->type == 'r')
                        stack_point = stack_point - 3;
                    else
                        stack_point = stack_point - 2;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // printf("\noperador -\n");
            // print_graph(root);
            // printf("\n\n");
            break;
            case '/':
                if (stack_point - 2 >= 0){

                    aux = div_reduction(stack[stack_point - 2]);


                    if ((stack_point - 3) >= 0 && stack[stack_point - 3]->type != 'r'){
                        stack[stack_point - 3]->left = aux;
                    }
                    else
                        root = aux;

                    if (stack_point < 3)
                        stack_point = 3;
                    if (stack[stack_point - 3]->type == 'r')
                        stack_point = stack_point - 3;
                    else
                        stack_point = stack_point - 2;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else
                    check = 0;
            // printf("\noperador -\n");
            // print_graph(root);
            // printf("\n\n");
            break;
            case 'Y':
                if (stack_point - 1 >= 0)
                {
                    // scanf("%c", &debug);
                    aux = Y_reduction(stack[stack_point - 1]);

                    if ((stack_point - 2) >= 0 && stack[stack_point - 2]->type != 'r')
                        stack[stack_point - 2]->left = aux;
                    else
                        root = aux;

                    if (stack_point < 2)
                        stack_point = 2;
                    if (stack[stack_point - 2]->type == 'r')
                        stack_point = stack_point - 2;
                    else
                        stack_point = stack_point - 1;

                    stack[stack_point] = aux;
                    stack_point = stack_update(stack_point);
                }
                else{
                    check = 0;
                }
            // print_graph(root);
            // printf("\n\n");
            break;

            case 'n':
                // printf("\nn case!!!!\n");
                aux = stack[stack_point];
                root = aux;
                // print_graph(aux);
                // printf("\n");
                check = 0;
                break;

            default:
                // printf("\n");
                // printf("\nEntrou no default\n");
                // printf("STACK[STACK_POINT]");
                // print_graph(stack[stack_point]);
                // printf("%c\n", stack[stack_point]->type);
                // printf("\n\n");

                if (r == 0)
                    root = stack[stack_point];
                check = 0;
                break;
        }
    }

    return root;

}

int main() {
    Node *root, *aux;
    int size, check = 1, number, temp, num1, num2;
    clock_t c2, c1;
    float _time;

    scanf("%s", input);
    transform_input();
    printf("(%s)\n", input);

    size = strlen(input);
    root = make_graph(0, size - 1);

    printf("Original Graph:\n");
    print_graph(root);
    printf("\n");

    printf("Start..\n");

    c1 = clock();
    root = reduce_graph(root, 0);
    c2 = clock();

    _time = (c2 - c1)*1000/CLOCKS_PER_SEC;

    printf("\n\nEnd!\n");
    printf("Reduced Graph:\n");
    print_graph(root);
    printf("\nTempo de redução: %dhs %dmin %dseg %dmiliseg\n\n", (((int) _time)/1000)/3600, ((((int) _time)/1000)%3600)/60, ((((int) _time)/1000)%3600)%60, ((int) _time) - (((int) _time)/1000)*1000);
    printf("\n");

    return 0;
}
