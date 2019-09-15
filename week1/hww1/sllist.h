typedef struct usr{
	char name[20];
	char pass[20];
	int status;
} acc;

typedef acc ListElementType;

void displayType(ListElementType user){

    printf("%-20s%d\n",user.name,user.status);
};

struct LISTNODE {
    ListElementType inf;
    struct LISTNODE *next;
};

typedef struct LISTNODE LISTNODE;

typedef struct list
{
    LISTNODE *root;
    LISTNODE *cur;
} LIST;

/*----------------------CAC THAO TAC TREN DSLK---------------------------------*/

LIST* newList(); //khoi tao dslk moi
void insertAtHead(LIST*, ListElementType); //them 1 phan tu vao dau ds
void insertAfterCurrent(LIST*, ListElementType); // them 1 phan tu vao sau current
LISTNODE* searchList(LIST*,char*);
ListElementType registerAcc(LIST*);
void deleteNode(LIST*, char*);
void deleteHead(LIST*);
void deleteAtPosition(LIST *, int);
/*-----------------------------------------------------------------------------*/

LISTNODE* searchList(LIST* list, char X[]){
    LISTNODE * p;
    
    for (p = list->root; p!= NULL; p = p->next) {
        if(strcasecmp(p->inf.name,X) == 0 ) {
            return p;
            break;
        }
    }
    return NULL;
}

void deleteHead(LIST *list){
    if(list->root!=NULL) {
        LISTNODE *tmp = list->root->next;
        free(list->root);
        if(list->cur == list->root) list->cur = tmp;
        list->root = tmp;
    }
}

void deleteAtPosition(LIST *list, int p){
    LISTNODE *tmp = list->root;
    LISTNODE *prev = NULL;
    if(p < 0) {
        //printf("Vi tri khong hop le!\n");
    }
    else if (p == 0) {
        deleteHead(list);
        //printf("Xoa thanh cong!\n");
    }
    else {
        int i;
        for(i = 0; i < p; i++) {
            if(tmp->next == NULL) break;
            prev = tmp;
            tmp = tmp->next;
        }
        if(i == p){
            prev->next = tmp->next;
            free(tmp);
            list->cur = list->root;
            //printf("Xoa thanh cong\n"); 
        }
        //else printf("Het danh sach! Xoa khong thanh cong\n");   
    }
}

void deleteNode(LIST* list, char X[]){
    int i=0;
    LISTNODE * p;
    
    for (p = list->root; p!= NULL; p = p->next,i++) {
        if(strcasecmp(p->inf.name,X) == 0 ) {
            deleteAtPosition(list,i);
            break;
        }
    }
}

ListElementType registerAcc(LIST* list){
    ListElementType temp;
    LISTNODE* p;

    printf("Username: ");
    scanf("%s",temp.name);
    p=searchList(list,temp.name);
    if(p!=NULL) printf("Account existed!\n");
    else{
        printf("Password: ");
        scanf("%s",temp.pass);
        temp.status = 1;
        printf("Successful registration\n");
    }
    return temp;
}


LIST* newList(){
    LIST *list = malloc(sizeof(LIST));
    list->root = NULL;
    list->cur = list->root;
    return list;
}

LISTNODE* makeNewNode(ListElementType inf){
    LISTNODE* new = (LISTNODE*) malloc(sizeof(LISTNODE));
    new->inf=inf;
    new->next =NULL;
    return new;
}

void insertAtHead(LIST *list, ListElementType e){
    LISTNODE* new = makeNewNode(e);
    new->next = list->root;
    list->root = new;
    list->cur = list->root;
}

void insertAfterCurrent(LIST *list, ListElementType e){
    LISTNODE *new = makeNewNode(e);
    if ( list->root == NULL ) {
        list->root = new;
        list->cur = list->root;
    } 
    else {
        new->next = list->cur->next;
        list->cur->next = new;
        list->cur = new;
    }
}