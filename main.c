#include <stdio.h>
#include <stdlib.h>
typedef struct nodo_ {

    char color;
    int key;
    struct nodo_ *left;  //puntatore al figlio sinistro
    struct nodo_ *right; //puntatore al nodo destro
    struct nodo_ *p;     //puntatore al nodo padre

} nodo;

nodo *NIL;  //riferimento a NIL di mezzo mondo

//la radice va usata cos? (*radice)->key o cose varie
void LeftRotate(nodo **, nodo*);
void RightRotate(nodo **, nodo*);
void Inorder_Tree_Walk(nodo*);
void RB_insert(nodo**, nodo*); //gli passo il nodo radice ed il nodo da inserire (al momento del passaggio ha solo la chiave)
void RB_insert_fixup(nodo**, nodo*);
nodo * Minimum(nodo *);
void RB_transplant(nodo**, nodo*, nodo*);
void RB_delete(nodo **, nodo*);
void RB_delete_fixup(nodo **, nodo*);
nodo * Tree_search(nodo *, int);
void Cancella_Albero2(nodo *);



int main()
{
    nodo *dainserire;
    NIL=(nodo*) malloc (sizeof(nodo));
    NIL->color = 'B';
    NIL->left = NULL;
    NIL->right = NULL;
    nodo *radice = NIL;

    int i=0;

    while(i<10){

        if(dainserire=(nodo*) malloc (sizeof(nodo))){
            //scanf("%d", &dainserire->key);
            dainserire->key = i;
            RB_insert(&radice, dainserire); //servono i puntatori doppi per modifcare la radice globalmente e non localmente
        }
        i++;
    }

    i=0;


    /*
    while(i<10){

        nodo *cancellare = Tree_search(radice,i);
        RB_delete(&radice,cancellare); //importante & prima di radice
        i++;
    }
    */
    Inorder_Tree_Walk(radice);

    Cancella_Albero2(radice);

    Inorder_Tree_Walk(radice);

    //printf("%d-----%d", radice->key, NIL->key);

    return 0;
}

void LeftRotate(nodo **radice, nodo* target) {

    nodo *y;

    y = target->right;
    target->right = y->left;

    if(y->left != NIL){
        y->left->p=target;
    }


    y->p = target->p;

    if(target->p == NIL)
        *radice = y;         //ma ? visto globalmente il cambio????

    else if (target == target->p->left)
        target->p->left = y;

    else
        target->p->right = y;

    y->left = target;
    target->p = y;

}

void RightRotate(nodo **radice, nodo* target) { //target = y delle slide

    nodo *x;

    x = target->left;
    target->left = x->right;

    if(x->right != NIL)
        x->right->p=target;

    x->p = target->p;

    if(target->p == NIL)
        *radice = x;

    else if (target == target->p->left)
        target->p->left = x;

    else
        target->p->right = x;

    x->right = target;
    target->p = x;
}


void RB_insert(nodo **radice, nodo *nuovo){

    nodo *x;
    nodo *y = NIL;

    x = *radice;

    while (x != NIL) {

        y=x;

        if (nuovo->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    nuovo->p = y;

    if(y == NIL)           //entra una sola volta
        *radice=nuovo;

    else if (nuovo->key < y->key)
        y->left = nuovo;

    else
        y->right = nuovo;

    nuovo->left = NIL;
    nuovo->right = NIL;
    nuovo->color = 'R';

    RB_insert_fixup(radice,nuovo); //radice ? gi? un doppio puntatore in questo caso

}

void RB_insert_fixup(nodo **radice, nodo *nuovo){

    nodo *y = NIL;

    while (*radice != nuovo && nuovo->p->color == 'R') {

        if (nuovo->p == nuovo->p->p->left){

            y = nuovo->p->p->right;
            if (y && y->color == 'R'){

                nuovo->p->color = 'B';
                y->color = 'B';
                nuovo->p->p->color = 'R';
                nuovo = nuovo->p->p;
            }

            else {

                if (nuovo == nuovo->p->right){

                    nuovo = nuovo->p;
                    LeftRotate(radice,nuovo);
                }

                nuovo->p->color = 'B';
                nuovo->p->p->color = 'R';
                RightRotate(radice,nuovo->p->p);
            }

        }
        else {

            y = nuovo->p->p->left;
            if (y && y->color == 'R'){

                nuovo->p->color = 'B';
                y->color = 'B';
                nuovo->p->p->color = 'R';
                nuovo = nuovo->p->p;
            }

            else {

                if (nuovo == nuovo->p->left){

                    nuovo = nuovo->p;
                    RightRotate(radice,nuovo);
                }

                nuovo->p->color = 'B';
                nuovo->p->p->color = 'R';
                LeftRotate(radice,nuovo->p->p);
            }
        }
    }
    (*radice)->color = 'B';
}

void Inorder_Tree_Walk(nodo *radice){

    if(radice != NIL){
        Inorder_Tree_Walk(radice->left);
        printf("\n%d-", radice->key);
        Inorder_Tree_Walk(radice->right);
    }
}

nodo * Minimum(nodo *x){

    while(x->left != NIL)
        x=x->left;
    return x;
}

void RB_transplant(nodo **radice, nodo *u, nodo *v) {

    if(u->p == NIL)
        *radice=v;

    else if(u == u->p->left)
        u->p->left=v;
    else
        u->p->right=v;
    v->p=u->p;

}

void RB_delete(nodo **radice, nodo *z){ //il nodo z ? quello da cancellare

    nodo *x, *y;
    char y_original_color;

    y=z;
    y_original_color = y->color;

    if(z->left == NIL){
        x=z->right;
        RB_transplant(radice,z,z->right);
    }
    else if(z->right == NIL){
        x=z->left;
        RB_transplant(radice,z,z->left);
    }
    else{

        y = Minimum(z->right);
        y_original_color = y->color;
        x=y->right;

        if(y->p == z)
            x->p = y;
        else {
            RB_transplant(radice,y,y->right);
            y->right = z->right;
            y->right->p = y;
        }

        RB_transplant(radice,z,y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if(y_original_color == 'B')
        RB_delete_fixup(radice,x);
}

void RB_delete_fixup(nodo **radice, nodo *x){

    nodo *w; //nodo fratello di x
    while(x != *radice && x->color == 'B'){

        if(x==x->p->left){

            w=x->p->right;
            if(w->color == 'R'){

                w->color = 'B';
                x->p->color = 'R';
                LeftRotate(radice,x->p);
                w=x->p->right;
            }
            if(w->left->color == 'B' && w->right->color == 'B'){

                w->color = 'R';
                x=x->p;
            }
            else{

                if (w->right->color == 'B'){

                    w->left->color = 'B';
                    w->color = 'R';
                    RightRotate(radice,w);
                    w=x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 'B';
                w->right->color = 'B';
                LeftRotate(radice,x->p);
                x=*radice;
            }
        }
        else {

            w=x->p->left;
            if(w->color == 'R'){

                w->color = 'B';
                x->p->color = 'R';
                RightRotate(radice,x->p);
                w=x->p->left;
            }
            if(w->left->color == 'B' && w->right->color == 'B'){

                w->color = 'R';
                x=x->p;
            }
            else{

                if (w->left->color == 'B'){

                    w->right->color = 'B';
                    w->color = 'R';
                    LeftRotate(radice,w);
                    w=x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 'B';
                //w->right->color = 'B';
                w->left->color = 'B';
                RightRotate(radice,x->p);
                x=*radice;
            }
        }
    }
    x->color = 'B';
}

nodo * Tree_search(nodo *radice, int chiave){

    if(radice == NIL || chiave == radice->key)
        return radice;

    if(chiave < radice->key)
        return Tree_search(radice->left,chiave);
    else
        return Tree_search(radice->right,chiave);
}

void Cancella_Albero2(nodo *radice){

    if(radice != NIL){
        Cancella_Albero2(radice->left);
        Cancella_Albero2(radice->right);
        free(radice);
    }
}

