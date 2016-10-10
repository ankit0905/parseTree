
/* LOGIC IN COMPUTER SCIENCE - ASSIGNMENT 1 

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

//The root of the Parse tree is initialized to NULL 
vertex* root=NULL;

/*
phi_prefix : input propositional logic formula in prefix form
phi_infix : input propositional logic formula in infix form
stack : data structure to store operators
final_infix : fully parenthesized propositional     logic formula in infix form obtained from parse Tree
*/
char phi_prefix[MAX], phi_infix[MAX], stack[MAX], final_infix[MAX];

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

//TASK 1 : Function to convert propositional formula from infix notation to prefix notation
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

//TASK 2: Function to convert prefix expression into a rooted binary parse tree
void prefixToParseTree(char phi_prefix[])
{
    int i,len=strlen(phi_prefix);
    vertex* curr=root;
    for(i=0; i<len; i++){
        vertex* tmp=(vertex*)malloc(sizeof(vertex));
        tmp->data=phi_prefix[i];
        tmp->left=NULL;
        tmp->right=NULL;
        //printf("%c\n",tmp->data);
        if(root==NULL){
            tmp->parent=NULL;
            root=tmp;
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
                if(curr==root || curr->right==NULL)
                    break;
                //printf("%c\n",tmp->data);
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

/*TASK 3: Function to compute the propositional logic formula in infix notation using inorder traversal of the Parse tree */
void parseTreeToInfix(vertex* node)
{
    if(node->left!=NULL){
        final_infix[start++]='(';
        parseTreeToInfix(node->left);
    }
    if(node->data=='~'){
        final_infix[start++]='(';
    }
    final_infix[start++]=node->data;
    if(node->right!=NULL){
        parseTreeToInfix(node->right);
        final_infix[start++]=')';
    }
}

/*TASK 4: Function to Compute the height of the parse tree  by recursively computing height of each subtree*/
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

/*TASK 5: Function to evaluate the truth value of the propositional logic formula by traversing in a bottom up manner*/
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

int main()
{
    printf("Enter a propositional logic formula in infix notation (well-formed and fully parenthesized):\n");
    scanf("%100s", phi_infix);
    infixToPrefix(phi_infix);
    strrev(phi_prefix);
    printf("Prefix form is : %s\n\n",phi_prefix);
    prefixToParseTree(phi_prefix);
    parseTreeToInfix(root);
    printf("Converted Infix form is : %s\n\n", final_infix);
    int height=computeHeight(root);
    printf("Height of the parse tree is %d\n\n",height);
    inputTruthValue();
    int truthValue=computeTruthValue(root);
    printf("Truth value is %d\n\n",truthValue);
    freeMemory(root);
    return 0;
}