
typedef struct nodeT{
  int coef;
  int powr;
  struct nodeT *next;
} node;

typedef struct {
   int num_terms;
   node *terms;
} poly;

void poly_print(poly *P) ;
void poly_free(poly **P) ;
poly *poly_duplicate(poly *P ) ;
poly *poly_add(poly *P1, poly *P2) ;
poly *poly_create(int num,...) ;
poly *poly_scalar_mult(poly *P, int x) ;
