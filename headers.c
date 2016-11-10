/*
The basic structure used for the representation of a vertex of parse Tree.
*/

typedef struct vertex
{
    char data;
    struct vertex* left;
    struct vertex* right;
    struct vertex* parent;
}vertex;

void strrev(char str[]);

/*
 * Takes in a pointer to a character array (namely, string) as input.
 * Reverses the string in linear time. 
 */

int isLowerPrecedence(char a, char b);

/*
 * Takes in 2 character type parameters among {'(', ')', '~', '+', '*', '>'}.
 * Returns 1 if first parameter has a lower precedence than the second one.
 */
 
 int isOperator(char ele);
 /*
  * Takes in a character as input.
  * Returns 1 if input is among the operators : '~', '+', '*' and '>'.
  * Otherwise returns 0.
  */
  
 int isOperand(char ele);
 /*
  * Takes in a character as input.
  * Returns 1 if input is not '(' or ')' and also not an operator.
  * Otherwise returns 0.
  */
 
 void infixToPrefix(char phi_infix[]);
 /*
  * Takes in a string (pointer to character array) as a parameter - formula in infix form
  * String is assumed to be a well formed and parathesized propositional logic formula in infix notation.
  * Converts the infix notation to prefix notation. 
  */
  
 void prefixToParsetree(char phi_prefix[]);
 /*
  * Takes in propositional logic formula in prefix form.
  * Constructs a parse Tree using structure of vertex as previously defined.
  */
 
 void parseTreeToInfix(vertex* node);
 /* 
  * Takes in pointer to the root of the parse Tree.
  * Recursively computes fully parenthesized infix notation using inorder traversal of binary tree.
  */
 
 int computeHeight(vertex* node);
 /*
  * Takes in pointer to the root of the parse Tree.
  * Recursively computes the height of subtrees and then returns it.
  */
  
 void inputTruthValue();
 /*
  * Function to take input the truth values of propositional atoms.
  * Stores the truth value in an array which can then be accessed.
  */

 int computeTruthValue(vertex* node);
 /*
  * Takes in pointer to the root of the parse Tree.
  * Computes truth value using the following: 
    Truth value of node = (Truth value of left child node) Operator (Truth value of right child node) 
  * Compututation is done recursilvely in bottom up fashion.
  */

 void freeMemory(vertex *node); 
 /*
  * Takes in the pointer to parse Tree.
  * Frees the memory allocated using malloc for the parse Tree recursively.
  */
    
void impl_free(vertex* node);
/*Function to compute a formula which is without implication and is equivalent to phi*/

void neg_at_left(vertex *ptr);
/*Function which adds a node to the left child of the given node and contains '~' as its data element
 */

void neg_at_right(vertex *ptr);
/*Function which adds a node to the right child of the given node and contains '~' as its data element
 */

void nnf(vertex* node);
/* Function to compute a formula which only negates atoms - called as Negation Normal Form    
    *pre-condition: phi is implication free
    *post-condition: computes NNF(phi)
 */

vertex* DISTR(vertex* ch1, vertex* ch2, vertex* node);
/*Function that computes the Distribution of Formulas
    *pre-condition: subformulas under ch1 and ch2 are CNF.
    *post-condition: computes DISTR() recursively
 */

void cnf_pre(vertex* node);
/* Function that calls impl_free() followed by nnf() which are the pre-conditions before calling cnf() method.
 */

int validity(char* phi_cnf);
/* Function to check if the given formula is valid or not.
 * It takes one argument: the propositional logic formula in CNF form and returns '0' (not valid) OR '1' (valid)
 */

int calculate_size(vertex* node);
/*Function to recursively compute the size of the parse Tree. It takes in one argument - the pointer the root node.
 */

void compare_tree_sizes();
/* Function to compare the sizes of the original propositional logic formula and the one in the CNF (Conjunctive Normal     Form)
 */