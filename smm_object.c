//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100
#define MAX_FOOD        100
#define MAX_FESTIVAL    100

static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험실로이동",
       "음식찬스",
       "축제시간"
};

char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}



typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;

//1. 구조체 형식 정의
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

static smmObject_t smm_node[MAX_NODE];
static int smmObj_noNode = 0;

static smmObject_t smm_food[MAX_FOOD];
static int smmObj_noFood = 0;

static smmObject_t smm_Festival[MAX_FESTIVAL];
static int smmObj_noFestival = 0;

//3. 관련 함수 변경 
//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

//Food generation
void* smmObj_genFood(char* name, int energy)
{
      //메모리 allocate for food object
      smmObject_t* foodPtr ;
      
      foodPtr = (smmObject_t*)malloc(sizeof(smmObject_t));
      
      //이름, 에너지 넣기 
      strcpy(foodPtr->name, name); 
      foodPtr->energy = energy;
      
      return foodPtr;
}
//Festival generation
void* smmObj_genFestival(char* card)
{
      //메모리 allocatie for festival object 
      smmObject_t* festivalPtr;
      
      festivalPtr = (smmObject_t*)malloc(sizeof(smmObject_t));
      //card 넣기 
      strcpy(festivalPtr->card, card);
}

//3. 관련 함수 변경 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

//3. 관련 함수 변경 
int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}

//음 식  
int smmObj_getFoodName(int food_nr)
{
    return smm_food[food_nr].name;
}

int smmObj_getFoodEnergy(int food_nr)
{
    return smm_food[food_nr].energy;
}

//페스티벌  
int smmObj_getFestivalCard (int festival_nr)
{
    return smm_festival[festival_nr].card
}

