/*
Tekijä: Kuisma Närhi 0453757
pvm.15.12.2016
lähteet: Kasurinen & Nikula: C-ohjelmointiopas-.pdf
http://www.geeksforgeeks.org/avl-tree-set-1-insertion/

Pohdinnat: poistofunktio pitäisi ehkä tehdä molemmille puolille erikseen, jos solmu on vasemmalla annettaisiin poistettavan solmun oikean lapsen pointteri puun tasapainotettavaksi ja toisinpäin.
Haun kompleksisuus on O(h), sillä liikkuessa puurakenteessa huonoin skenaario on, että haku liikkuisi puun korkeuden verran askelia.

*/


#include<stdio.h>
#include<stdlib.h>

struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};


int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}
/*apufunktio kahden luvun vertailuun */
int max(int a, int b) {
    if (a>b){
        return a;
    } else return b;
}

struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

/*Kierrot ylläpitävät noden sisäistä korkeusmuuttujaa height*/
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;
/*Tier 2-node talteen jotta ensimmäisen siirron jälkeen se voidaan "pelastaa bittiavaruudesta" ja kiinnittää uudelle paikalleen*/
    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    return x;
}


struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y;
}
/*tasapainon apufunktio */
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
/*insert-funktio etenee kunnes oikea paikka on löytynyt*/
struct Node* insert(struct Node* node, int key) {
    if (node == NULL)
        return(newNode(key));
    if (key < node->key) {
        node->left  = insert(node->left, key);
    }
    else if (key > node->key) {
        node->right = insert(node->right, key);
    } else
        printf("Number is already in the tree\n");
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
/*tasapainotarkistus*/
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void preOrder(struct Node *root) {
    if(root != NULL) /*Tulostaa aina solmun ja sen lapset, käy ensin vasemmalle jonka jälkeen oikealle. Ei siis varsinaisesti piirrä ascii-puuta vaan tulostaa luettavassa muodossa puun sisällön*/
    {
        if (root->left != NULL && root->right != NULL)
            printf("Main node:%d\nLeft child:%d\nRight child:%d\n\n", root->key, root->left->key, root->right->key);
        else if (root->left != NULL && root->right == NULL)
            printf("Main node:%d\nLeft child:%d\n\n", root->key, root->left->key);
        else if (root->left == NULL && root->right != NULL)
            printf("ain node:%d\nRight child:%d\n\n", root->key, root->right->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

struct Node* search(struct Node* node, int count) {
    int target, i;
    printf("Search for number: ");
    scanf("%d", &target);
/* count = jäsenten määrä, tulee luetun tiedoston countterista ja mahdollisista lisäyksistä*/
    for(i=0; i <= count; i++){
        if(target < node->key)
        {
            if (node->left != NULL) {
                node = node->left;
            } else {
                printf("404\n");
                break;
            }
        }
        else if(target > node->key)
        {
            if (node->right != NULL) {
                node = node->right;
            } else {
                printf("404\n");
                break;
            }
        }
        else if(target == node->key)
        {
            printf("Number %d found\n", target);
            break;
        }
    }
    return node;
}
int main()
{
    int option = 1;
    int num;
    int count;
    struct Node *root = NULL;
/* tiedostonluku */
    FILE *fp;
    fp = fopen("numbers.txt", "r");

    while (!feof(fp)){
        fscanf(fp, "%d", &num);
        root = insert(root, num);
        printf("%d added to the tree.\n", num);
        count++;
    }
    fclose(fp);

    while (option != 0) {
        printf("1) Add number to tree\n");
        printf("2) Search for number\n");
        printf("3) Print\n");
        printf("0) Quit\n");
        scanf("%d", &option);
        if (option == 1) {
            printf("Add number: ");
            scanf("%d", &num);
            root = insert(root, num);
        }
        else if (option == 2) {
            search(root, count);
        }
        else if (option == 3) {
            preOrder(root);
        }
        else if (option == 0) {
            printf("Program is kill");
            return 0;
        }
    }
    return 0;
}
