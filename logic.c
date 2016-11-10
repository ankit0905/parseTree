/* LOGIC IN COMPUTER SCIENCE - ASSIGNMENT 

Assumptions:
->Input to the program is in infix notation and Well formed
->Input Formula is well formed and fully parenthesized
->Propositional atoms are single character english alphabets
->Maximum length of input propositional logic formula is 500

Connectives:
1. ~ for negation
2. + for OR
3. * for AND
4. > for implication

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX 501

/*Structure used to represent the parse tree. Each node has a character data element and pointers to left & right child and to its parent node */
typedef struct vertex
{
    char data;
    struct vertex* left;
    struct vertex* right;
    struct vertex* parent;
}vertex;

//The root of the Parse tree for original formula is initialized to NULL 
vertex* root=NULL;

//The root of the Parse tree for CNF formula initialized to NULL
vertex* cnf_root=NULL;

/*  STRINGS USED TO REPRESENT FORMULAS
==============================================================
    phi_prefix : input propositional logic formula in prefix form
    phi_infix : input propositional logic formula in infix form
    stack : data structure to store operators
    final_infix : fully parenthesized propositional logic formula in infix form obtained from parse Tree
    phi_cnf : propositional logic formula in CNF form
*/
char phi_prefix[MAX], phi_infix[MAX], stack[MAX], final_infix[MAX], phi_cnf[MAX];

//a variable that tracks the index of final_infix
int start=0, truth_val[150];

//Function to reverse the expression
void strrev(char str[])
{
    int len=strlen(str);
    int lo=0,hi=len-1;
    while(lo<hi){
        char temp=str[lo];
        str[lo]=str[hi];
        str[hi]=temp;
        lo++;
        hi--;
    }
}

//Function to evaluate if the first operator has a lower precedence that the second
int isLowerPrecedence(char a, char b)
{
    char operators[]={'(',')','~','*','+','>'};
    int i,idxa,idxb;
    if(a=='(' && b==')'){
        return 0;
    }
    for(i=0; i<6; i++){
        if(a==operators[i])
            idxa=i;
        if(b==operators[i])
            idxb=i;
    }
    if(idxa>idxb)
        return 1;
    else
        return 0;
}

//TASK 1.1 : Function to convert propositional formula from infix notation to prefix notation
void infixToPrefix(char phi_infix[])
{
    int i,top=-1,curr=0,len=strlen(phi_infix);
    strrev(phi_infix);
    for(i=0; i<len; i++){
        char ch=phi_infix[i];
        if(ch=='('){
            ch=phi_infix[i]=')';
        }
        else if(ch==')'){
            ch=phi_infix[i]='(';
        }
        if(ch=='~' || ch=='+' || ch=='*' || ch=='>'){
            while(top!=-1 && stack[top]!='(' && !isLowerPrecedence(stack[top],ch)){
                phi_prefix[curr++]=stack[top];
                top--;
            }
            top++;
            stack[top]=ch;
        }
        else if(ch=='('){
            top++;
            stack[top]=ch;
        }
        else if(ch==')'){
            while(stack[top]!='('){
                phi_prefix[curr++]=stack[top];
                top--;
            }
            top--;
        }
        else{
            phi_prefix[curr++]=ch;
        }
    }
    strrev(phi_infix);
    while(top!=-1){
        phi_prefix[curr++]=stack[top];
        top--;
    }
}

/*Function of check if an element is an operator*/
int isOperator(char ele)
{
    if(ele=='~' || ele=='>' || ele=='+' || ele=='*')
        return 1;
    else 
        return 0;
}

/*Function of check if an element is an operand*/
int isOperand(char ele)
{
    if(ele!='(' && ele!=')' && isOperator(ele)==0)
        return 1;
    else
        return 0;
}

//TASK 1.2: Function to convert prefix expression into a rooted binary parse tree
void prefixToParseTree(vertex** root, char phi_prefix[])
{
    int i,len=strlen(phi_prefix);
    vertex* curr=*root;
    for(i=0; i<len; i++){
        vertex* tmp=(vertex*)malloc(sizeof(vertex));
        tmp->data=phi_prefix[i];
        tmp->left=NULL;
        tmp->right=NULL;
        if(!*root){
            tmp->parent=NULL;
            *root=tmp;
            curr=tmp;
        }
        else if(isOperand(phi_prefix[i])==1){
            if(curr->left==NULL && phi_prefix[i-1]!='~'){
                tmp->parent=curr;
                curr->left=tmp;
                curr=curr->left;
            }
            else{
                tmp->parent=curr;
                curr->right=tmp;
                curr=curr->right;
            }
            while(1){
                curr=curr->parent;
                if(curr==*root || curr->right==NULL)
                    break;
            }
        }
        else{
            if(curr->left==NULL && phi_prefix[i-1]!='~'){
                tmp->parent=curr;
                curr->left=tmp;
                curr=curr->left;
            }
            else{
                tmp->parent=curr;
                curr->right=tmp;
                curr=curr->right;
            }
        }
    }
}

/*TASK 1.3: Function to compute the propositional logic formula in infix notation using inorder traversal of the Parse tree */
void parseTreeToInfix(vertex* node)
{
    if(node->left!=NULL){
        final_infix[start++]='(';
        parseTreeToInfix(node->left);
    }
    if(node->data=='~'){
        final_infix[start++]='(';
    }
    //printf("%c",node->data);
    final_infix[start++]=node->data;
    if(node->right!=NULL){
        parseTreeToInfix(node->right);
        final_infix[start++]=')';
    }
}

/*TASK 1.4: Function to Compute the height of the parse tree  by recursively computing height of each subtree*/
int computeHeight(vertex* node) 
{
   if (node==NULL) 
       return 0;
   else
   {
       int leftHeight = computeHeight(node->left);
       int rightHeight = computeHeight(node->right);

       if (leftHeight > rightHeight) 
           return(leftHeight+1);
       else return(rightHeight+1);
   }
} 

/* Function that takes user input for the truth values of all the propositional atoms. */
void inputTruthValue()
{
    int i,temp,len=strlen(phi_prefix);
    for(i=0; i<150; i++)
        truth_val[i]=-1;
    for(i=0; i<len; i++){
        if(isOperand(phi_prefix[i])==1 && truth_val[phi_prefix[i]]==-1){
            printf("Enter truth value of %c : ",phi_prefix[i]);
            scanf("%d", &temp);
            truth_val[phi_prefix[i]]=temp;
        }
    }
}

/*TASK 1.5: Function to evaluate the truth value of the propositional logic formula by traversing in a bottom up manner*/
int computeTruthValue(vertex* node)
{
    if(isOperand(node->data)){
        return truth_val[node->data];
    }
    else if(node->data=='+'){
        if(computeTruthValue(node->left)==1 || computeTruthValue(node->right)==1){
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(node->data=='*'){
        if(computeTruthValue(node->left)==1 && computeTruthValue(node->right)==1){
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(node->data=='~'){
        if(computeTruthValue(node->right)==1){
            return 0;
        }
        else{
            return 1;
        }
    }
    else if(node->data=='>'){
        if(computeTruthValue(node->left)==1 && computeTruthValue(node->right)==0){
            return 0;
        }
        else{
            return 1;
        }
    }
}

/*Function to free the memory allocated using malloc for the vertex */
void freeMemory(vertex* node)
{
    if(node==NULL)
        return;
    freeMemory(node->left);
    freeMemory(node->right);
    free(node);
}

/*Function to compute a formula which is without implication and is equivalent to phi*/
void impl_free(vertex* node)
{
    vertex *curr=node;
    if(curr==NULL)
        return;
    impl_free(curr->left);
    impl_free(curr->right);
    if(curr->data=='>'){
        vertex* tmp=(vertex*)malloc(sizeof(vertex));
        tmp->data='~';
        tmp->left=NULL;
        tmp->right=curr->left;
        tmp->parent=curr;
        curr->left->parent=tmp;
        curr->left=tmp;
        curr->data='+';
    }
}

/*Function which adds a node to the left child of the given node and contains '~' as its data element*/
void neg_at_left(vertex *ptr){
        vertex* temp;
        temp = (vertex *)malloc(sizeof(vertex));
        temp->data='~';
        temp->left=NULL;
        temp->parent=ptr;
        temp->right=ptr->left;
        ptr->left=temp;
        temp->right->parent=temp;
}

/*Function which adds a node to the right child of the given node and contains '~' as its data element*/

void neg_at_right(vertex *ptr){
        vertex* temp;
        temp = (vertex *)malloc(sizeof(vertex));
        temp->data='~';
        temp->left=NULL;
        temp->parent=ptr;
        temp->right=ptr->right;
        ptr->right=temp;
        temp->right->parent=temp;
}

/* Function to compute a formula which only negates atoms - called as Negation Normal Form
    
pre-condition: phi is implication free
post-condition: computes NNF(phi)
*/
void nnf(vertex* node)
{
    vertex *curr=node;
    if(curr==NULL)
        return;
    if(curr->data=='~'){
        if(curr->right->data=='~'){
            if(curr->parent==NULL){
                curr->right->right->parent=NULL;
                cnf_root=curr->right->right;
                curr=curr->right->right;
                nnf(curr);
            }
            else{
                curr->right->right->parent=curr->parent;
                if(curr->parent->left==curr){
                    curr->parent->left=curr->right->right;
                }
                else{
                    curr->parent->right=curr->right->right;
                }
                curr=curr->right->right;
                nnf(curr);
            }
        }
        else if(curr->right->data=='+' || curr->right->data=='*'){
            if(curr->parent==NULL){
                neg_at_left(curr->right);
                neg_at_right(curr->right);
                curr->right->parent=NULL;
                cnf_root=curr->right;
                curr=curr->right;
                nnf(curr->left);
                nnf(curr->right);
                if(curr->data=='+'){
                    curr->data='*';
                }
                else{
                    curr->data='+';
                }
            }
            else{
                neg_at_left(curr->right);
                neg_at_right(curr->right);
                curr->right->parent=curr->parent;
                if(curr==curr->parent->right){
                    curr->parent->right=curr->right;
                }
                else{
                    curr->parent->left=curr->right;
                }
                curr=curr->right;
                nnf(curr->left);
                nnf(curr->right);
                if(curr->data=='+'){
                    curr->data='*';
                }
                else{
                    curr->data='+';
                }
            }
        }
    }
    else{
        nnf(curr->left);
        nnf(curr->right);
    }
}

/*Function that computes the Distribution of Formulas
    
  pre-condition: subformulas under ch1 and ch2 are CNF.
  post-condition: computes DISTR() recursively
*/
vertex* DISTR(vertex* ch1, vertex* ch2, vertex* node)
{
    if(ch2!=NULL && ch1!=NULL)
    {

        if(ch1->data=='*'){
            node->data='*';
            vertex *temp1=(vertex*)malloc(sizeof(vertex));
            temp1->left=NULL;
            temp1->right=NULL;
            temp1->data='+';
            vertex *temp2=(vertex*)malloc(sizeof(vertex));
            temp2->left=NULL;
            temp2->right=NULL;
            temp2->data='+';
            temp1->left=ch1->left; temp1->right=ch2;
            temp2->left=ch1->right; temp2->right=ch2;
            temp1=DISTR(temp1->left,temp1->right,temp1);
            temp2=DISTR(temp2->left,temp2->right,temp2);
            node->left=temp1;
            node->right=temp2;
            return node;
        }
        else if(ch2->data=='*'){
            node->data='*';
            vertex *temp1=(vertex*)malloc(sizeof(vertex));
            temp1->left=NULL;
            temp1->right=NULL;
            temp1->data='+';
            vertex *temp2=(vertex*)malloc(sizeof(vertex));
            temp2->left=NULL;
            temp2->right=NULL;
            temp2->data='+';
            temp1->left=ch1; temp1->right=ch2->left;
            temp2->left=ch1; temp2->right=ch2->right;
            temp1=DISTR(temp1->left,temp1->right,temp1);
            temp2=DISTR(temp2->left,temp2->right,temp2);
            node->left=temp1;
            node->right=temp2;
            return node;
        }
        else{
            return node;
        }
    }   
    else
    {
        return NULL;
    }
}

/* Task 2.1: Function that computes the Conjunctive normal Form
   
   pre-condition: formula is implication free and in NNF
   post-condition: computes CNF() recursively
*/
vertex* CNF(vertex *node)
{
    if(node!=NULL)
    {
        if(node->data=='*' || node->data=='+'){
            node->left=CNF(node->left);
            node->right=CNF(node->right);
            if(node->data=='*'){
                return node;
            }
            else if(node->data=='+'){
                return DISTR(node->left,node->right,node);
            }
        }
        else { 
            return node; 
        }
    }
    else
    {
        return NULL;
    }
}

/* Function that calls impl_free() followed by nnf() which are the pre-conditions before calling cnf() method.
*/
void cnf_pre(vertex* node)
{
    impl_free(cnf_root);
    start=0;
    parseTreeToInfix(cnf_root);
    printf("Implication Free Output is : %s\n\n", final_infix);
    final_infix[start]='\0';
    start=0;
    nnf(cnf_root);
    parseTreeToInfix(cnf_root);
    final_infix[start]='\0';
    printf("NNF Output is : %s\n\n", final_infix);
    start=0;
    CNF(cnf_root);
    parseTreeToInfix(cnf_root);
    final_infix[start]='\0';
    printf("CNF Output is : %s\n\n", final_infix);
    strcpy(phi_cnf,final_infix);
}


/* TASK 2.2:  Function to check if the given formula is valid or not.
   It takes one argument: the propositional logic formula in CNF form and returns '0' (not valid) OR '1' (valid)
*/
int validity(char* phi_cnf)
{
    int i,len = strlen(phi_cnf);
    int st[50], en[50], num_clauses=0;
    for(i=0; i<50; i++){
        st[i]=en[i]=-1;
    }
    st[0]=0, en[0]=len;
    for(i=0; i<len; i++){
        if(i==0){
            st[num_clauses]=i;
        }
        else{
            if(phi_cnf[i]=='&'){
                en[num_clauses]=i-1;
                num_clauses++;
                if(phi_cnf[i+1]=='(')
                    st[num_clauses]=i+1;
            }
        }
    }
    en[num_clauses]=len-1;
    int clause_idx=0,neg=0,truth_val;
    while(1){
        if(en[clause_idx]==-1 && en[clause_idx]==-1){
            break;
        }
        int atom[200]={0};
        truth_val=0,neg=0;
        for(i=st[clause_idx]; i<=en[clause_idx]; i++){
            if(phi_cnf[i]=='~'){
                neg=1;
            }
            else if(isOperand(phi_cnf[i])){
                if(neg==1 && atom[phi_cnf[i]]==0){
                    atom[phi_cnf[i]]=-1;
                }
                else if(neg==1 && atom[phi_cnf[i]]==1){
                    truth_val=1;
                    break;
                }
                else if(neg==0 && atom[phi_cnf[i]]==0){
                    atom[phi_cnf[i]]=1;
                }
                else if(neg==0 && atom[phi_cnf[i]]==-1){
                    truth_val=1;
                    break;
                }
                neg=0;
            }
        }
        if(truth_val==0){
            return 0;
        }
        clause_idx++;
    }
    return 1;
}

/*Function to recursively compute the size of the parse Tree. It takes in one argument - the pointer the root node.
*/
int calculate_size(vertex* node)
{
    if(node==NULL){
        return 0;
    }
    else{
        return (1 + calculate_size(node->left) + calculate_size(node->right));
    }
}

/*TASK 2.3: Function to compare the sizes of the original propositional logic formula and the one in the CNF (Conjunctive Normal Form)
*/
void compare_tree_sizes()
{
    int sz1 = calculate_size(root);
    int sz2 = calculate_size(cnf_root);
    printf("Size Of Original Formula: %d\n",sz1);
    printf("Size Of Formula in CNF: %d\n",sz2);
    if(sz1>sz2){
        printf("Hence, Size of Original Formula is Greater.\n");
    }
    else if(sz1<sz2){
        printf("Hence, Size of Formula in CNF is Greater.\n\n");
    }
    else{
        printf("Hence, Size of Original Formula and its CNF is equal.\n\n");
    }
}

int main()
{
    printf("Enter a propositional logic formula in infix notation (well-formed and fully parenthesized):\n");
    scanf("%100s", phi_infix);
    infixToPrefix(phi_infix);
    strrev(phi_prefix);
    printf("\nPrefix form is : %s\n\n",phi_prefix);
    prefixToParseTree(&root,phi_prefix);
    prefixToParseTree(&cnf_root,phi_prefix);
    vertex* root2=cnf_root;
    parseTreeToInfix(root);
    printf("Converted Infix form is : %s\n\n", final_infix);
    int height=computeHeight(root);
    printf("Height of the parse tree is %d\n\n",height);
    inputTruthValue();
    int truthValue=computeTruthValue(root);
    printf("Truth value is %d\n\n",truthValue);
    cnf_pre(cnf_root);
    compare_tree_sizes();
    if(validity(final_infix)){
        printf("The Formula is Valid\n\n");
    }
    else{
        printf("The Formula is Invalid\n\n");
    }
    freeMemory(root);
    freeMemory(cnf_root);
    return 0;
}