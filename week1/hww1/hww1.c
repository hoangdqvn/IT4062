#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sllist.h"


void menuM(){
	printf("USER MANAGEMENT PROGRAMM\n");
	printf("---------------------------------------------\n");
	printf("\t1. Register\n");
	printf("\t2. Sign in\n");
	printf("\t3. Search\n");
	printf("\t4. Sign out\n");
	printf("Your choice (1-4, other to quit): ");
}

int logIn(LIST *list, LIST *listLogin, int *k){
	char tempUser[20];
	char tempPass[20];
	LISTNODE* tempnode;

	do{
		printf("Username: ");
		scanf("%s",tempUser);
		tempnode = searchList(list,tempUser);
		if(tempnode==NULL) printf("Cannot find account\n");
		else if(tempnode->inf.status == 0) printf("Account is blocked\n");
	}while(tempnode==NULL||tempnode->inf.status == 0);
	do{
		printf("Password: ");
		scanf("%s",tempPass);
		if(strcmp(tempPass,tempnode->inf.pass)!=0){
			printf("Password is incorrected\n");
			*k+=1;
		}
		else *k=0;
	}while(strcasecmp(tempPass,tempnode->inf.pass)!=0 && *k<3);
	if(*k>=3) {
		tempnode->inf.status = 0;
		printf("Password is incorrected. Account is blocked\n");
		return 0;
	}
	else {
		insertAfterCurrent(listLogin,tempnode->inf);
		printf("Hello %s\n",tempnode->inf.name );
	}
	return 1;
}

void logOut(LIST *list,LIST *listLogin){
	char tempUser[20];
	LISTNODE* tempnode;

	printf("Username: ");
	scanf("%s",tempUser);
	tempnode = searchList(list,tempUser);
	if(tempnode==NULL) printf("Cannot find account\n");
	else{
		tempnode = searchList(listLogin,tempUser);
		if(tempnode==NULL) printf("Account is not sign in\n");
		else{
			printf("Goodbye %s\n", tempUser);
			deleteNode(listLogin,tempUser);
		}
	}
}

void searchUser(LIST *list){
	char tempUser[20];
	LISTNODE* tempnode;

	printf("Username: ");
	scanf("%s",tempUser);
	tempnode = searchList(list,tempUser);
	if(tempnode==NULL) printf("Cannot find account\n");
	else if(tempnode->inf.status == 0) printf("Account is blocked\n");
	else printf("Account is active\n");
}

int readFile(LIST *list, char *file){
	FILE *fptr;
	if((fptr=fopen(file,"r"))==NULL){
		printf("File %s is not found!\n",file);
		return 0;
	}
	else{
		ListElementType temp;
		while(1){
			fscanf(fptr,"%s",temp.name);
			fscanf(fptr,"%s",temp.pass);
			fscanf(fptr,"%d",&temp.status);
			if(feof(fptr)) break;
			insertAfterCurrent(list, temp);
		}
	}
	fclose(fptr);
	return 1;
}

void saveFile(LIST *list, char *file){
	FILE *fptr = fopen(file,"wb");
	if(list->root==NULL) return;
	LISTNODE * p;
    for (p = list->root; p!= NULL; p = p->next){
        fprintf(fptr,"%s %s %d\n",p->inf.name,p->inf.pass,p->inf.status);
    }
    
	fclose(fptr);
}

int main(){
	int n;

	LIST *list = newList();
	LIST *listLogin = newList();

	int k=0;
	int loginStt = 0;

	if(readFile(list,"account.txt")){

	do{
		menuM();
		scanf("%d",&n);
		switch(n){
			case 1:
			insertAfterCurrent(list,registerAcc(list));
			break;
			case 2:
			{
				loginStt=logIn(list,listLogin,&k);
			}
			break;
			case 3:
			{
				if (listLogin->root == NULL) loginStt = 0;
				if(loginStt==0) printf("Account is not sign in!\n");
				else{
					searchUser(list);
				}
			}
			break;
			case 4:
			{
				if (listLogin->root == NULL) loginStt = 0;
				if(loginStt==0) printf("Account is not sign in!\n");
				else{
					logOut(list,listLogin);
				}
			}
			break;
			default: break;
		}
	}while(n==1||n==2||n==3||n==4);
	
	saveFile(list,"account.txt");

	}
	return 0;
}