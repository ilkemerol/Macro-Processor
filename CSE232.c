#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//Link List tanýmlama;
typedef struct node{
	void *dataPtr;
	struct node *link;
}NODE;
typedef struct{
	int countlist;
	NODE *head;
	NODE *rear;
	int (*compare)(void* argu1, void* argu2);
}LIST;
int list(void *first,void *second){
	int result;
	result=strcmp((char*)first,(char*)second);
	return result;
}
LIST* createList(int (*compare) (void* argu1, void* argu2)){
	LIST* list;
	list = (LIST*)malloc(sizeof(LIST));
	if(list){
		list->head = NULL;
		list->rear = NULL;
		list->countlist = 0;
		list->compare = compare;
	}
	return list;
}
//Link Listde arama ve Node ekleme;
bool _search (LIST*  pList, NODE** pPre,NODE** pLoc,  void*  pArgu){
#define COMPARE \
	( ((* pList->compare) (pArgu, (*pLoc)->dataPtr)) )
#define COMPARE_LAST \
	((* pList->compare) (pArgu, pList->rear->dataPtr))
	int result;
	*pPre  = NULL;
	*pLoc  = pList->head;
	if (pList->countlist == 0){
	    return false;
	}
	result = COMPARE;
	if (result == 0){
	   return true;
	}
	else{
	   return false;
	}
}
static bool _insert (LIST* pList, NODE* pPre, void* dataInPtr){
	NODE* pNew;
	if (!(pNew = (NODE*) malloc(sizeof(NODE))))
	   return false;
	pNew->dataPtr  = dataInPtr;
	pNew->link      = NULL;
	if (pPre == NULL){
	    pNew->link       = pList->head;
	    pList->head      = pNew;
	    if (pList->countlist == 0)
	       pList->rear = pNew;
	   }
	else{
	    pNew->link  = pPre->link;
	    pPre->link  = pNew;
	    if (pNew->link     == NULL)
	         pList->rear   =  pNew;
	   }
	(pList->countlist)++;
	return true;
}
int addNode (LIST* pList,void* dataInPtr){
	bool success;
	NODE* pPre = pList->rear;
	success = _insert (pList, pPre, dataInPtr);
	if(!success){
		return -1;
	}
	return 0;
}
//Link List'i MSDOS'a bastýrma;
void display(LIST *pList){
	NODE *travel;
	travel = (NODE*)malloc(sizeof(NODE));
	travel = pList->head;
	while(travel->link != NULL){
		printf("%s",(char*)travel->dataPtr);
		travel = travel->link;
	}
	if(travel->link == NULL){
		printf("%s",(char*)travel->dataPtr);
	}
}
//Link List'i file yaz.
void writer(LIST *pList){
	NODE *writeFile;
	writeFile = (NODE*)malloc(sizeof(NODE));
	writeFile = pList->head;
	FILE *Expanded;
	Expanded = fopen("Expanded Code.txt","w");
	while(writeFile->link != NULL){		
		fprintf(Expanded,"%s	",(char*)writeFile->dataPtr);
		writeFile = writeFile->link;
	}
	if(writeFile->link == NULL){
		fprintf(Expanded,"%s",(char*)writeFile->dataPtr);
	}
}
//Main;
int main(void){
	printf("_________________________________________________________________\n");
	printf("|---PARAMETRE-TABLE-LOADING------------------------------>-20---|\n");
	printf("|---PARAMETRE-TABLE-LOADING------------------------------>-60---|\n");
	printf("|---PARAMETRE-TABLE-LOADING------------------------------>-70---|\n");
	printf("|---PARAMETRE-TABLE-LOADING------------------------------>100---|\n");
	printf("|-----------------------PARAMETRE-TABLE-------------------------|\n");
	printf("|Directives	Token1	Token2	Token3	Token4	Token5	Token6	|\n");
	//Dosyalarý aç;
	FILE *Macro, *Source;
	Macro = fopen("MacroCode.txt","r");
	Source = fopen("SourceCode.txt","r");
	//Macro file ve Source file için 2 ayrý Link List oluþtur;
	LIST *macroList, *sourceList;
	char *word;
	char *token;
	char newlineControl;
	macroList=createList(&list);
	sourceList=createList(&list);
	while(!feof(Macro)){
		word = (char*)malloc(sizeof(char)*50);
		//Macro filedan kelime alýp 'word' un içine atýyor.
		fscanf(Macro,"%s%c",word,&newlineControl);
		//Token 'Word'deki kelimeyi ayýrýyor.
		token=strtok(word,".,?!'$[]/\(%)={}+^#<>&-_*:;\"");
		while(token != NULL){
			if(newlineControl == '\r'){
				char newline[2] = {'\n','\0'};
				addNode(macroList,token);
				addNode(macroList,newline);
			}
			else{
				addNode(macroList,token);
			}
			//Döngüden çýkmasý için NULL'a eþitliyor.
			token=strtok(NULL,".,?!'$[]/\(%)={}+^#<>&-_*:;\"");
		}
	}
	//Line by line
	while(!feof(Source)){
		word = (char*)malloc(sizeof(char)*50);
		fscanf(Source,"%s%c",word,&newlineControl);
		token=strtok(word,".,?!'$[]/(%){}+^#&-_*:;\"");
		while(token != NULL){
			if(newlineControl == '\r'){
				char newline[2] = {'\n','\0'};
				addNode(sourceList,token);
				addNode(sourceList,newline);
			}
			else{
				addNode(sourceList,token);
			}	
			token=strtok(NULL,".,?!'$[]/(%){}+^#&-_*:;\"");
		}
	}
	//2 ayrý LinkList oluþturuldu buradan sonra LinkListleri gezerek Expaned File oluþtur.
	LIST *defineList,*expandedList;
	defineList=createList(&list);
	expandedList=createList(&list);

	char *Control;
	Control = (char*)malloc(sizeof(char)*50);
	int First = 1;

	NODE *travel;
	travel = (NODE*)malloc(sizeof(NODE));
	travel = sourceList->head->link;
	NODE *travelPre;
	travelPre = (NODE*)malloc(sizeof(NODE));
	travelPre = sourceList->head;
	//DEFINE için variable.
	char *defineName;
	int *defineValue;
	//MCALL için variable.
	char *macroName;
	char *value1;
	char *value2;
	char *secondmacroName;
	char *secondValue1;
	char *secondValue2;
	//Döngüde sourceList'i tara.
	while(travel->link != NULL){
		//Source File'da DEFINE gecen yerleri bulup DEFINE deðerlerini defineList'in içine atar.
		if(strcmp((char*)travel->dataPtr,"DEFINE") != 0 && strcmp((char*)travel->dataPtr,"MCALL") != 0 && strcmp((char*)travel->link->dataPtr,"DEFINE") != 0 && strcmp((char*)travel->dataPtr,"IF") != 0 && strcmp((char*)travel->dataPtr,"ELSE") != 0 && strcmp((char*)travel->dataPtr,"ENDIF") != 0){
			if(First == 1){
				addNode(expandedList,travelPre->dataPtr);
				First = 0;
			}
			addNode(expandedList,travel->dataPtr);
		}
		if(strcmp((char*)travel->dataPtr,"DEFINE") == 0){
			defineName = (char*)malloc(sizeof(char)*50);
			defineValue = (int*)malloc(sizeof(int)*50);
			strcpy(defineName,(char*)travelPre->dataPtr);
			defineValue = (int*)travel->link->dataPtr;
			addNode(defineList,defineName);
			addNode(defineList,defineValue);
			travel = travel->link;
			travelPre = travelPre->link;
			//'\n'i atlatmak icin.
			travel = travel->link;
			travelPre = travelPre->link;
		}
		if(strcmp((char*)travel->dataPtr,"MCALL") == 0){
			//MCALL bulunca yanýndaki bilgileri alýr alt tarafda macroListe gezerken kullanýr.
			macroName = (char*)malloc(sizeof(char)*50);
			value1 = (char*)malloc(sizeof(char)*50);
			value2 = (char*)malloc(sizeof(char)*50);
			strcpy(macroName,(char*)travel->link->dataPtr);
			strcpy(value1,(char*)travel->link->link->dataPtr);
			strcpy(value2,(char*)travel->link->link->link->dataPtr);
			//Terminale tablo printi;
			printf("|mcall		MCALL	%s	%s	%s			|\n",macroName,value1,value2);
			//macroList'i gezmek için deðiþken oluþtur.
			NODE *traverseMacro;
			traverseMacro = (NODE*)malloc(sizeof(NODE));
			traverseMacro = macroList->head;
			while(traverseMacro->link != NULL){
				//macroList'de gelen macroName'e göre arama yapar ve ENDM görene kadarýný expandedList'e yollar.
				if(strcmp((char*)traverseMacro->dataPtr,macroName) == 0 && strcmp((char*)traverseMacro->link->dataPtr,"MACRO") == 0){
					//PARAMETRE TABLOSU//printf("%s,%s,%s\n",macroName,value1,value2);
					char *macroVariableA;
					char *macroVariableB;
					macroVariableA = (char*)malloc(sizeof(char)*50);
					macroVariableB = (char*)malloc(sizeof(char)*50);
					strcpy(macroVariableA,(char*)traverseMacro->link->link->dataPtr);
					strcpy(macroVariableB,(char*)traverseMacro->link->link->link->dataPtr);
					char *macroA;
					char *macroB;
					macroA = (char*)malloc(sizeof(char)*50);
					macroB = (char*)malloc(sizeof(char)*50);
					strcpy(macroA,value1);
					strcpy(macroB,value2);
					//'\n'
					traverseMacro = traverseMacro->link->link->link->link->link;
					while(strcmp((char*)traverseMacro->dataPtr,"ENDM") != 0){
						char *printMacro;
						printMacro = (char*)malloc(sizeof(char)*50);
						strcpy(printMacro,(char*)traverseMacro->dataPtr);
						//Source Code'dan gelen macro variablelarýný deðiþtirme.
						if(strcmp(printMacro,macroVariableA) == 0){
							addNode(expandedList,macroA);
						}
						if(strcmp(printMacro,macroVariableB) == 0){
							addNode(expandedList,macroB);
						}
						if(strcmp(printMacro,"MCALL") == 0){
							secondmacroName = (char*)malloc(sizeof(char)*50);
							secondValue1 = (char*)malloc(sizeof(char)*50);
							secondValue2 = (char*)malloc(sizeof(char)*50);
							strcpy(secondmacroName,(char*)traverseMacro->link->dataPtr);
							strcpy(secondValue1,(char*)traverseMacro->link->link->dataPtr);
							strcpy(secondValue2,(char*)traverseMacro->link->link->link->dataPtr);
							printf("|mcall		MCALL	%s	%s	%s			|\n",secondmacroName,secondValue1,secondValue2);
							NODE *secondtraverseMacro;
							secondtraverseMacro = (NODE*)malloc(sizeof(NODE));
							secondtraverseMacro = macroList->head;
							while(secondtraverseMacro->link != NULL){	
								if(strcmp((char*)secondtraverseMacro->dataPtr,secondmacroName) == 0 && strcmp((char*)secondtraverseMacro->link->dataPtr,"MACRO") == 0){
										char *macroVariableA;
										char *macroVariableB;
										macroVariableA = (char*)malloc(sizeof(char)*50);
										macroVariableB = (char*)malloc(sizeof(char)*50);
										strcpy(macroVariableA,(char*)secondtraverseMacro->link->link->dataPtr);
										strcpy(macroVariableB,(char*)secondtraverseMacro->link->link->link->dataPtr);
										char *macroA;
										char *macroB;
										macroA = (char*)malloc(sizeof(char)*50);
										macroB = (char*)malloc(sizeof(char)*50);
										strcpy(macroA,secondValue1);
										strcpy(macroB,secondValue2);
										secondtraverseMacro = secondtraverseMacro->link->link->link->link->link;
										while(strcmp((char*)secondtraverseMacro->dataPtr,"ENDM") != 0){
											char *printMacro;
											printMacro = (char*)malloc(sizeof(char)*50);
											strcpy(printMacro,(char*)secondtraverseMacro->dataPtr);
											//Source Code'dan gelen macro variablelarýný deðiþtirme.
											if(strcmp(printMacro,macroVariableA) == 0){
												addNode(expandedList,macroA);
											}
											if(strcmp(printMacro,macroVariableB) == 0){
												addNode(expandedList,macroB);
											}
											if(strcmp(printMacro,macroVariableA) != 0 && strcmp(printMacro,macroVariableB) != 0){
												addNode(expandedList,printMacro);
											}
											secondtraverseMacro = secondtraverseMacro ->link;
										}
								
								}
								else{
									secondtraverseMacro = secondtraverseMacro ->link;
								}
							}
							traverseMacro = traverseMacro->link;
							traverseMacro = traverseMacro->link;
							traverseMacro = traverseMacro->link;
							//'\n'
							traverseMacro = traverseMacro->link;
						}
						if(strcmp(printMacro,macroVariableA) != 0 && strcmp(printMacro,macroVariableB) != 0 && strcmp(printMacro,"MCALL") != 0){
							addNode(expandedList,printMacro);

						}
						traverseMacro = traverseMacro->link;
					}
				}
				else{
					traverseMacro = traverseMacro->link;
				}	
			}
			//MCALL'dan sonra gelen 3 parametreyi expanded'a bastýrmamak için 3 kere ileri git.
			travel = travel->link;
			travelPre = travelPre->link;
			travel = travel->link;
			travelPre = travelPre->link;
			travel = travel->link;
			travelPre = travelPre->link;
			//'\n'
			travel = travel->link;
			travelPre = travelPre->link;
		}
		//IF kontorlü
		if(strcmp((char*)travel->dataPtr,"IF") == 0){
			char *loopControl,*loopValue,*loopOP;
			char *OP;
			loopControl = (char*)malloc(sizeof(char)*50);
			strcpy(loopControl,(char*)travel->link->dataPtr);
			loopOP = (char*)malloc(sizeof(char)*50);
			strcpy(loopOP,(char*)travel->link->link->dataPtr);
			loopValue = (char*)malloc(sizeof(char)*50);
			strcpy(loopValue,(char*)travel->link->link->link->dataPtr);
			if(strcmp(loopOP,"=") == 0){
				OP = (char*)malloc(sizeof(char)*50);
				strcpy(OP,"EQ");
			}
			if(strcmp(loopOP,">") == 0){
				OP = (char*)malloc(sizeof(char)*50);
				strcpy(OP,"GT");
			}
			if(strcmp(loopOP,"<") == 0){
				OP = (char*)malloc(sizeof(char)*50);
				strcpy(OP,"LT");
			}
			NODE *traverseDefine;
			traverseDefine = (NODE*)malloc(sizeof(NODE));
			traverseDefine = defineList->head;
			//Terminale IF printi;
			printf("|if-else-endif	IF	%s	%s	%s			|\n",loopControl,loopOP,loopValue);
			//defineList'in içinde IF'den sonra gelen deðeri arýyor varsa onun DEFINE deðeriyle karþýlaþtýrma yaparýyor.
			while(traverseDefine->link != NULL){
				//EQ kontrol.
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"EQ") == 0 && strcmp((char*)traverseDefine->link->dataPtr,loopValue) == 0){
					//IF True döndü, ELSE görene kadar iþlemleri normal yapacak ELSE görünce ENDIF'e kadar ittirecek.
					strcpy(Control,"TRUE");
					//N1 ve 5'i expandedList'e atmamasý için ileri atýyor.
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					//\n'i atlatmak icin.
					travel = travel->link;
					travelPre = travelPre->link;
				}
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"EQ") == 0 && strcmp((char*)traverseDefine->link->dataPtr,loopValue) != 0){
					//IF FALSE döndü, ELSE görene kadar ittirecek,ENDIF görene kadar normal çalýþacak.
					strcpy(Control,"FALSE");
					while(strcmp((char*)travel->dataPtr,"ELSE") != 0){
						travel = travel->link;
						travelPre = travelPre->link;
					}
					travel = travel->link;
					travelPre = travelPre->link;
				}
				//GT kontrol.
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"GT") == 0 && strcmp(loopValue,(char*)traverseDefine->link->dataPtr) < 0){	
					strcpy(Control,"TRUE");
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					//\n'i atlatmak icin.
					travel = travel->link;
					travelPre = travelPre->link;
				}
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"GT") == 0 && strcmp(loopValue,(char*)traverseDefine->link->dataPtr) > 0){
					strcpy(Control,"FALSE");
					while(strcmp((char*)travel->dataPtr,"ELSE") != 0){
						travel = travel->link;
						travelPre = travelPre->link;
					}
					travel = travel->link;
					travelPre = travelPre->link;
				}
				//LT kontrol.
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"LT") == 0 && strcmp((char*)traverseDefine->link->dataPtr,loopValue) < 0){
					strcpy(Control,"TRUE");
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					travel = travel->link;
					travelPre = travelPre->link;
					//\n'i atlatmak icin.
					travel = travel->link;
					travelPre = travelPre->link;
				}
				if(strcmp((char*)traverseDefine->dataPtr,loopControl) == 0 && strcmp(OP,"LT") == 0 && strcmp((char*)traverseDefine->link->dataPtr,loopValue) > 0){
					strcpy(Control,"FALSE");
					while(strcmp((char*)travel->dataPtr,"ELSE") != 0){
						travel = travel->link;
						travelPre = travelPre->link;
					}
					travel = travel->link;
					travelPre = travelPre->link;
				}
				traverseDefine = traverseDefine->link;
			}
		}
		//IF TRUE dönerse ELSE gördüðü yerde ENDIF'e kadar ittirme.
		if(strcmp(Control,"TRUE") == 0){
			if(strcmp((char*)travel->dataPtr,"ELSE") == 0){
				while(strcmp((char*)travel->dataPtr,"ENDIF") != 0){
				travel = travel->link;
				travelPre = travelPre->link;
				}
			}
		}
		//ENDIF gordugu yerde '\n' atlatma.
		if(strcmp((char*)travel->dataPtr,"ENDIF") == 0){
			travel = travel->link;
			travelPre = travelPre->link;
		}
		travel = travel->link;
		travelPre = travelPre->link;
	}
	if(travel->link == NULL){
		addNode(expandedList,travel->dataPtr);
	}

	//Terminale DEFINE printi;
	NODE *traveldefine;
	traveldefine = (NODE*)malloc(sizeof(NODE));
	traveldefine = defineList->head;
	while(traveldefine->link->link != NULL){
		printf("|define		%s	DEFINE	%s				|\n",(char*)traveldefine->dataPtr,(char*)traveldefine->link->dataPtr);
		traveldefine = traveldefine->link->link;
	}
	if(traveldefine->link->link == NULL){
		printf("|define		%s	DEFINE	%s				|\n",(char*)traveldefine->dataPtr,(char*)traveldefine->link->dataPtr);
	}
	//Terminale MACRO-ENDM printi;
	/*NODE *travelmacro;
	travelmacro = (NODE*)malloc(sizeof(NODE));
	travelmacro = macroList->head;
	printf("|macro-endm	%s	%s	%s	%s			|\n",(char*)travelmacro->dataPtr,(char*)travelmacro->link->dataPtr,(char*)travelmacro->link->link->dataPtr,(char*)travelmacro->link->link->link->dataPtr);
	travelmacro = travelmacro->link->link->link->link;
	while(travelmacro->link->link != NULL){
		printf("							%s	|\n",(char*)travelmacro->dataPtr);
		if(strcmp((char*)travelmacro->dataPtr,"ENDM") == 0){
			travelmacro = travelmacro->link;
			printf("|macro-endm	%s	%s	%s	%s			|\n",(char*)travelmacro->dataPtr,(char*)travelmacro->link->dataPtr,(char*)travelmacro->link->link->dataPtr,(char*)travelmacro->link->link->link->dataPtr);
		travelmacro = travelmacro->link->link->link;
		}
		travelmacro = travelmacro->link;
	}*/
	printf("|---------------------------END---------------------------------|\n");
	//expandedList'i oluþturmasý tamamlandý, expandedList'i file yaz.
	writer(expandedList);
	printf("|--------------ExpandedFile.txt has been created!---------------|\n");
	//Kontrol için printler.
	/*printf("Macro List:\n");
	display(macroList);
	printf("\n\n\nSource List:\n");
	display(sourceList);*/
}
//EKSIKLER
//SADECE MAX 2 LI MACROLAR CALISIYOR.
