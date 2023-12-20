//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"



//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;


typedef struct player {
        int energy;
        int position;
        char name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
} player_t;

typedef struct food {
        char name[MAX_CHARNAME];
        int energy;
} food_t;

static player_t *cur_player;
static food_t *cur_food;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
 //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif


void printGrades(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
     }
}

void printPlayerStatus(void)
{
     int i;
     
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
}

void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name:", i); //안내 문구 
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //set energy
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
     }
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

//랜덤한 성적을 내보내는 함수
smmObjGrade_e getRandomGrade()
{
  return (smmObjGrade_e)(rand()%MAX_GRADE);
} 

//랜덤의 음식을 출력하기  
int getRandomFoodEnergy()
 {
   int randomIndex = (rand()%food_nr);
   return randomIndex;
 }
 
//랜덤한 카드 뽑기
int getRandomFestivalCard()
{
    int randomCard = (rand90%festival_nr);
    return randomCard;
}
 

//action code when a player stays at a node
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
    void *foodPtr = smmdb_getData(LISTNO_FOODCARD, cur_food[player].energy );
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getNodeName( boardPtr );
    void *gradePtr;
    
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
             if 
            cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr );     
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);           
            //grade generation
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, getRandomGrade());
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
            break;
            
        //case restaurant
        case SMMNODE_TYPE_RESTAURANT;
          cur_player[player].energy += //보충에너지가 어디 명시되어있지? ;
          
          break;
        
        
        //case LABORATORY
        case SMMNODE_TYPE_LABORATORY;
          //실험실에 왔다는거 알리기
          //실험 중인 상태인지 확인하기 
            //실험중인 상태라면 - 주사위를 굴려서 지정된 실험 성공 기준값 이상이 나오면 실험 종료 아니면 실험중 상태로 머무름
            //실험중인 상태가 아니라면 - 나가기 
        
        //case HOME
        case SMMNODE_TYPE_HOME;
          //집이라는거 알리기
          printf("&s gets a food chance! press any key to pick a food card", cur_player[player].name);              
          //지정된 보충 에너지 만큼 현재 에너지에 추가하기  보충에너지 = 기초 에너지
        
        //case GOTOLAB
        case SMMNODE_TYPE_GOTOLAB;
          //실험실로 이동한다고 알리기
          //실험 중 상태로 전환하기
        //case FOODCHANCE
        case SMMNODE_TYPE_FOODCHANCE;
          //음식찬스라는 걸 알리고 키 누를 때 까지 기다리 기  
          printf("&s gets a food chance! press any key to pick a food card", cur_player[player].name); 
          getchar();
          //기존의 에너지에, 랜덤으로 선택된 음식의 에너지 더하기 
          int randomFoodIndex = getRandomFoodEnergy();
          cur_player[player].energy += smmObj_getFoodEnergy(randomFoodIndex);
          
          //선택된 음식 출력
          printf("Selected food: %s, Energy: %d\n", smmObj_getFoodName(randomFoodIndex), smmObj_getFoodEnergy(randomFoodIndex)); 
          
          break;
          
        //case FESTIVAL
        case SMMNODE_TYPE_FESTIVAL;
          //축제에 왔다는 걸 알리고
          printf("&s participates to Snow festival! press any key to pick a festival card", cur_player[player].name);
          getchar();
          //축제 카드 랜덤으로 보여주기
          int randomCardIndex = getRandomFestivalCard();
          printf("Your mission is %s", smmObj_getFestivalCard(getRandomFestivalCard())); 
          //미션 완료했는지 확인하기 
          printf("Press any key when mission is ended."); 
          getchar ();
           
          break;
        
        default:
            break;
    }
}

void goForward(int player, int s력tep)
{
     void *boardPtr;
     //플레이어의 위치에 전진한걸 더하기  
     cur_player[player].position += step;
     //플레이어가 이동한 위치의 노드 정보를 얻어 옴  
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     
     //이동한 노드 정보 출 
     printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr);
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int card;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while ( fscanf( fp, "%s %i", name, &energy) == 2 ) //read a food parameter set
    {
          void *foodObj = smmObj_genFood( name, energy );
          smmdb_addTail(LISTNO_FOOD, foodObj);
          food_nr++;
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    //food_nr 만큼 반복  
    for (i=0; i<food_nr; i++)
    {
        void *foodObj = smmdb_getData(LISTNO_FOOD, i);
        
        printf("&i. &s, charge:&i\n", i, smmObj_getFoodName(foodObj), smmObj_getFoodEnergy(foodObj));  
    }
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf( fp, "&s", card ) == 1 ) //read a festival card string
    {
          void *FestivalObj = smmObj_genFestival( card );
          smmdb_addTail(LISTNO_FOODCARD, foodObj);
          festival_nr++;
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    //festival_nr만큼 반복
    for (i=0; i<festival_nr; i++)
    {
        void *FestivalObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("%i. %s", i, card\n);
    } 
    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    
    
    
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result; 
        
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)        
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
    }
    
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
