/*//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

Operating System Project 

In This program football player transfer operations will be performed using threads, mutex locks, and semaphores.

Instructor:
Dr. Şeniz Demir

Group Members:
AHMET SELİM DİZER
ÖMER OĞUZ ÇELİKEL
ALPEREN GÜVENİR

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////*/



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

sem_t  negotiate[4]; // Every agent only negotiate 2 clubs, every agent use 1 semaphore
sem_t  Club[5]; // Clubs don't 2 transfer same time. Every clube use 1 seamphore 
pthread_mutex_t mutex; // for the clubs' transfer list first incoming thread makes the first transfer list also. Srand fuction () works well with mutex.

// Returns 5 random numbers from 0 to 23, for 23 players. 
int * TransferList() {
    int *arr = malloc(4);
    int i;
    sleep(1);
    srand(time(NULL));
    for ( i = 0; i < 5; i++) {
        arr[i] = rand()%24;
    }
    pthread_mutex_unlock(&mutex);
    return arr;
}
//Converts duplicate numbers that occur in the transfer list to different numbers.
int *Dp(int a[]){
    for(int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (a[i] == a[j]) {
                if (a[j] == 23) {
                    a[j] = a[i] - 1;
                } else {
                    a[j] = a[i] + 1;
                }
            }
        }
    }
    return a;
}

void *club(void *data); // Clubs threads' goes club function with club name "A", "B" etc. ALso function creates 5 threads for the transfers.
void *Agent1(void *num); // Agents threads' goes Agent function with 2 argument club name and player id. It either performs the transfer or fails.
void *Agent2(void *num);// Agents threads' goes Agent function with 2 argument club name and player id. It either performs the transfer or fails.
void *Agent3(void *num);// Agents threads' goes Agent function with 2 argument club name and player id. It either performs the transfer or fails.
void *Agent4(void *num);// Agents threads' goes Agent function with 2 argument club name and player id. It either performs the transfer or fails.

struct thread_data{ // for the agent thread 
    int  p_id; // player id
    int c; //club id 0-> A, 1->B ...
}threads[5]; // Every club use different struct

struct Club 
{
    int budget;
}A,B,C,D,E;

struct Player // Every player's different id, budget ,AgentId, position. After the transfer player takes club's name for to show that the transfer has been made.
{
    int id;
    int Pbudget;
    int AgentID;
    char position[15];
    char club[1];

}players[23]; // 23 different players

struct Agent // Every agent's different player and every player has position this struct keeps track of which position the players play.
{
    int forward ;
    int midfielder;
    int defender;
    int goalkeeper;
}Agent[4]; // 4 different Agents

int main() {
    pthread_t clb[5]; //clubs thread
    pthread_mutex_init(&mutex, NULL); // initialize mutex


// It creates 7 forward player and takes random budget ( 4M to 20M)
    for (int i = 0; i < 7; i++)
    {
        players[i].id = i;
        players[i].Pbudget =(rand() %17 +4);
        strcpy( players[i].position, "Forward");
    }
// It creates 6 midfielder player and takes random budget ( 4M to 20M)   
    for (int i = 7; i < 13; i++)
    {
        players[i].id = i;
        players[i].Pbudget =(rand() %17 +4);
        strcpy( players[i].position, "Midfielder");
    }
// It creates 6 defender player and takes random budget ( 4M to 20M)      
    for (int i = 13; i < 18; i++)
    {
        players[i].id = i;
        players[i].Pbudget =(rand() %17 +4);
        strcpy( players[i].position, "Defender");
    }
// It creates 6 goalkeeper player and takes random budget ( 4M to 20M)      
    for (int i = 18; i < 23; i++)
    {
        players[i].id = i;
        players[i].Pbudget =(rand() %17 +4);
        strcpy( players[i].position, "Goalkeeper");
    }

// For agents, we first set all values ​​to 0.

    for (int i = 0; i < 4; i++)
    {
        Agent[i].forward = 0;
        Agent[i].midfielder = 0;
        Agent[i].defender = 0;
        Agent[i].goalkeeper = 0;
    }
    
//It send each player to agents according to mod 4.
    for( int i =0 ; i <23; i++){

        if(strcmp(players[i].position,"Forward") == 0){

            Agent[i % 4].forward += 1;
            players[i].AgentID = i%4;
            strcpy(players[i].club, "x");


        }
        if(strcmp(players[i].position,"Midfielder") == 0){

            Agent[i % 4].midfielder += 1;
            players[i].AgentID = i%4;
            strcpy(players[i].club, "x");


        }
        if(strcmp(players[i].position,"Defender") == 0){

            Agent[i % 4].defender += 1;
            players[i].AgentID = i%4;
            strcpy(players[i].club, "x");


        }
        if(strcmp(players[i].position,"Goalkeeper") == 0){

            Agent[i % 4].goalkeeper += 1;
            players[i].AgentID = i%4;
            strcpy(players[i].club, "x");


        }

    }

// initialize semaphores
    for (int i = 0; i < 4; i++)
    {
        sem_init( &negotiate[i], 0, 2);
    }
    for (int i = 0; i < 5; i++)
    {
        sem_init( &Club[i], 0, 1);
    }
    
    srand(time(NULL));
    //random budget for the clubs (10, 20, ..., 50) 

    A.budget = (rand() %5 +1)*10;

    B.budget = (rand() %5 +1)*10;

    C.budget = (rand() %5 +1)*10;

    D.budget = (rand() %5 +1)*10;

    E.budget = (rand() %5 +1)*10;


//Threads for every clubs
    pthread_create(&clb[0], NULL, club,"A");
    pthread_create(&clb[1], NULL, club,"B");
    pthread_create(&clb[2], NULL, club,"C");
    pthread_create(&clb[3], NULL, club,"D");
    pthread_create(&clb[4], NULL, club,"E");

    pthread_join(clb[0], NULL);
    pthread_join(clb[1], NULL);
    pthread_join(clb[2], NULL);
    pthread_join(clb[3], NULL);
    pthread_join(clb[4], NULL);

    return 0;
}
// Club threads goes club function with data("A","B","C","D","E")
void *club(void *data){
  //for club A
    if(data == "A"){
        printf("Club-A starts the transfer window with a budget of %d M euros\n", A.budget);
      // Other Clubs waits for "A" to finish the transfer list.
        pthread_mutex_lock(&mutex);
        int *atemp = TransferList(); // Transferlist
        int *alist = Dp(atemp); // Check duplicate item and change in trasnferlist change
        // print transferlist
        printf("Club-A prepares the list: Agent- %d: %s, Agent- %d: %s, Agent- %d: %s, Agent- %d: %s, and Agent- %d: %s\n", 
               players[alist[0]].AgentID,players[alist[0]].position,
               players[alist[1]].AgentID, players[alist[1]].position,
               players[alist[2]].AgentID, players[alist[2]].position,
               players[alist[3]].AgentID, players[alist[3]].position,
               players[alist[4]].AgentID, players[alist[4]].position);

        pthread_t agentA[4]; //transfer thread



// every transferlist have 5 player
        for(int i=0; i<5; i++){

            threads[0].c = 0; //CLub A -> 0;

            threads[0].p_id = alist[i];  // PLayer Id

// check which agency does the player belong to?
            if(players[alist[i]].AgentID == 0){ //Agent 1
                sem_wait(&Club[0]); // clubs cannot make two transfers at the same time.
                pthread_create(&agentA[0], NULL, Agent1 , (void *) &threads[0]);
                sem_post(&negotiate[0]); // After transfer, the semaphore is increased
                pthread_join(agentA[0], NULL);
            }
            else if(players[alist[i]].AgentID == 1){ //Agent 2
                sem_wait(&Club[0]);
                pthread_create(&agentA[1], NULL, Agent2, (void *) &threads[0]);
                sem_post(&negotiate[1]);
                pthread_join(agentA[1], NULL);
            }
            else if(players[alist[i]].AgentID == 2){ //Agent 3
                sem_wait(&Club[0]);
                pthread_create(&agentA[2], NULL, Agent3, (void *) &threads[0]);
                sem_post(&negotiate[2]);
                pthread_join(agentA[2], NULL);
            }
            else if(players[alist[i]].AgentID == 3){ // Agent 4
                sem_wait(&Club[0]);
                pthread_create(&agentA[3], NULL, Agent4, (void *) &threads[0]);
                sem_post(&negotiate[3]);
                pthread_join(agentA[3], NULL);
            }
        }




    }
    if(data == "B"){

        printf("Club-B starts the transfer window with a budget of  %d M euros\n", B.budget);
        pthread_mutex_lock(&mutex);
        int *btemp = TransferList();
        int *blist = Dp(btemp);

        printf("Club-B prepares the list: Agent- %d: %s, Agent-%d: %s, Agent-%d: %s, Agent-%d: %s, and Agent-%d: %s\n",players[blist[0]].AgentID,players[blist[0]].position,
               players[blist[1]].AgentID, players[blist[1]].position,
               players[blist[2]].AgentID, players[blist[2]].position,
               players[blist[3]].AgentID, players[blist[3]].position,
               players[blist[4]].AgentID, players[blist[4]].position);

        pthread_t agentB[4]; //transfer thread





        for(int i=0; i<5; i++){

            threads[1].c = 1; //CLub B -> 1;
            threads[1].p_id = blist[i];  // PLayer Id

            if(players[blist[i]].AgentID == 0){
                sem_wait(&Club[1]);
                pthread_create(&agentB[0], NULL, Agent1 ,(void *) &threads[1]);
                sem_post(&negotiate[0]);
                pthread_join(agentB[0], NULL);

            }
            else if(players[blist[i]].AgentID == 1){
                sem_wait(&Club[1]);
                pthread_create(&agentB[1], NULL, Agent2,(void *) &threads[1]);
                sem_post(&negotiate[1]);
                pthread_join(agentB[1], NULL);
            }
            else if(players[blist[i]].AgentID == 2){
                sem_wait(&Club[1]);
                pthread_create(&agentB[2], NULL, Agent3,(void *) &threads[1]);
                sem_post(&negotiate[2]);
                pthread_join(agentB[2], NULL);
            }
            else if(players[blist[i]].AgentID == 3){
                sem_wait(&Club[1]);
                pthread_create(&agentB[3], NULL, Agent4,(void *) &threads[1]);
                sem_post(&negotiate[3]);
                pthread_join(agentB[3], NULL);
            }
        }

    }
    if(data == "C"){

        printf("Club-C starts the transfer window with a budget of  %d M euros\n", C.budget);
        pthread_mutex_lock(&mutex);
        int *ctemp = TransferList();
        int *clist = Dp(ctemp);

        printf("Club-C prepares the list: Agent- %d: %s, Agent-%d: %s, Agent-%d: %s, Agent-%d: %s, and Agent-%d: %s\n",players[clist[0]].AgentID,players[clist[0]].position,
               players[clist[1]].AgentID, players[clist[1]].position,
               players[clist[2]].AgentID, players[clist[2]].position,
               players[clist[3]].AgentID, players[clist[3]].position,
               players[clist[4]].AgentID, players[clist[4]].position);
        pthread_t agentC[4]; //transfer thread





        for(int i=0; i<5; i++){

            threads[2].c = 2; //CLub C -> 2;
            threads[2].p_id = clist[i];  // PLayer Id

            if(players[clist[i]].AgentID == 0){
                sem_wait(&Club[2]);
                pthread_create(&agentC[0], NULL, Agent1 ,(void *) &threads[2]);
                sem_post(&negotiate[0]);
                pthread_join(agentC[0], NULL);

            }
            else if(players[clist[i]].AgentID == 1){
                sem_wait(&Club[2]);
                pthread_create(&agentC[1], NULL, Agent2,(void *) &threads[2]);
                sem_post(&negotiate[1]);
                pthread_join(agentC[1], NULL);
            }
            else if(players[clist[i]].AgentID == 2){
                sem_wait(&Club[2]);
                pthread_create(&agentC[2], NULL, Agent3,(void *) &threads[2]);
                sem_post(&negotiate[2]);
                pthread_join(agentC[2], NULL);
            }
            else if(players[clist[i]].AgentID == 3){
                sem_wait(&Club[2]);
                pthread_create(&agentC[3], NULL, Agent4,(void *) &threads[2]);
                sem_post(&negotiate[3]);
                pthread_join(agentC[3], NULL);
            }
        }





    }
    if(data == "D"){

        printf("Club-D starts the transfer window with a budget of  %d M euros\n", D.budget);
        pthread_mutex_lock(&mutex);
        int *dtemp = TransferList();
        int *dlist = Dp(dtemp);

        printf("Club-D prepares the list: Agent- %d: %s, Agent-%d: %s, Agent-%d: %s, Agent-%d: %s, and Agent-%d: %s\n",players[dlist[0]].AgentID,players[dlist[0]].position,
               players[dlist[1]].AgentID, players[dlist[1]].position,
               players[dlist[2]].AgentID, players[dlist[2]].position,
               players[dlist[3]].AgentID, players[dlist[3]].position,
               players[dlist[4]].AgentID, players[dlist[4]].position);

        pthread_t agentD[4]; //transfer thread




        for(int i=0; i<5; i++){

            threads[3].c = 3; //CLub D -> 3;
            threads[3].p_id = dlist[i];  // PLayer Id
            if(players[dlist[i]].AgentID == 0){
                sem_wait(&Club[3]);
                pthread_create(&agentD[0], NULL, Agent1 ,(void *) &threads[3]);
                sem_post(&negotiate[0]);
                pthread_join(agentD[0], NULL);

            }
            else if(players[dlist[i]].AgentID == 1){
                sem_wait(&Club[3]);
                pthread_create(&agentD[1], NULL, Agent2,(void *) &threads[3]);
                sem_post(&negotiate[1]);
                pthread_join(agentD[1], NULL);

            }
            else if(players[dlist[i]].AgentID == 2){
                sem_wait(&Club[3]);
                pthread_create(&agentD[2], NULL, Agent3,(void *) &threads[3]);
                sem_post(&negotiate[2]);
                pthread_join(agentD[2], NULL);
            }
            else if(players[dlist[i]].AgentID == 3){
                sem_wait(&Club[3]);
                pthread_create(&agentD[3], NULL, Agent4,(void *) &threads[3]);
                sem_post(&negotiate[3]);
                pthread_join(agentD[3], NULL);
            }
        }





    }
    if(data =="E"){

        printf("Club-E starts the transfer window with a budget of  %d M euros\n", E.budget);
        pthread_mutex_lock(&mutex);
        int *etemp = TransferList();
        int *elist = Dp(etemp);

        printf("Club-E prepares the list: Agent- %d: %s, Agent-%d: %s, Agent-%d: %s, Agent-%d: %s, and Agent-%d: %s\n",players[elist[0]].AgentID,players[elist[0]].position,
               players[elist[1]].AgentID, players[elist[1]].position,
               players[elist[2]].AgentID, players[elist[2]].position,
               players[elist[3]].AgentID, players[elist[3]].position,
               players[elist[4]].AgentID, players[elist[4]].position);

        pthread_t agentE[4]; //transfer thread

        for(int i=0; i<5; i++){

            threads[4].c = 4; //CLub E -> 4;
            threads[4].p_id = elist[i];  // PLayer Id
            if(players[elist[i]].AgentID == 0){
                sem_wait(&Club[4]);
                pthread_create(&agentE[0], NULL, Agent1 ,(void *) &threads[4]);
                sem_post(&negotiate[0]);
                pthread_join(agentE[0], NULL);
            }
            else if(players[elist[i]].AgentID == 1){
                sem_wait(&Club[4]);
                pthread_create(&agentE[1], NULL, Agent2,(void *) &threads[4]);
                sem_post(&negotiate[1]);
                pthread_join(agentE[1], NULL);
            }
            else if(players[elist[i]].AgentID == 2){
                sem_wait(&Club[4]);
                pthread_create(&agentE[2], NULL, Agent3,(void *) &threads[4]);
                sem_post(&negotiate[2]);
                pthread_join(agentE[2], NULL);
            }
            else if(players[elist[i]].AgentID == 3){
                sem_wait(&Club[4]);
                pthread_create(&agentE[3], NULL, Agent4,(void *) &threads[4]);
                sem_post(&negotiate[3]);
                pthread_join(agentE[3], NULL);
            }
        }

    }

}
void *Agent1(void *num){
    struct thread_data *data;
    data = (struct thread_data *) num;

    int clubName = data->c; // Club A -> 0, Club B -> 1, Club C -> 2 ...
    int pID = data->p_id;  // pID takes Player Id
    sem_wait(&negotiate[0]);// Every agent only 2 negotiate at the same time.
    if(clubName == 0){ //clubName 0 shows Club A
        printf("Club A negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);
        //The club name was assigned an x string. X means that player is not in any club.
        if(strcmp(players[pID].club , "x")==0){
            //If Club A's budget is enough to buy the player.
            if(A.budget >= players[pID].Pbudget){
               // Decreasing Club A budged up to price of Players.
                A.budget = A.budget - players[pID].Pbudget;

                // Assign "A" string for player's club instead of x. Player has a team now. 
                strcpy(players[pID].club, "A");
                // Checks how many forward, midfiielder, defender, goalkeeper players are left from the Agent 1. 
                // This value will decrease by 1 after the player transfers.
                if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward > 0){
                    Agent[0].forward = Agent[0].forward - 1;
                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder > 0){
                    Agent[0].midfielder = Agent[0].midfielder - 1;
                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender > 0){
                    Agent[0].defender = Agent[0].defender - 1;
                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper > 0){
                    Agent[0].goalkeeper = Agent[0].goalkeeper - 1;
                }
                printf("Club-A transfers a %s for %dM euros from Agent-1 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,A.budget);
            }
            // the club budget is not enough to buy the player
            else{
                printf("Club-A cannot transfer a %s from Agent-1 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        // Agents has no player
        else{
            if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward == 0){
                printf("Club-A cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder == 0){
                printf("Club-A cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender == 0){
                printf("Club-A cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper == 0){
                printf("Club-A cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-A cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[0]); // Each club can only make 1 transfer at a time, semaphore increases after transfer.

    }
    else if(clubName == 1){ ////clubName 2 shows Club B. Do Same think for Club B.
        printf("Club B negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(B.budget >= players[pID].Pbudget){
                B.budget = B.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "B");

                if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward > 0){

                    Agent[0].forward = Agent[0].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder > 0){

                    Agent[0].midfielder = Agent[0].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender > 0){

                    Agent[0].defender = Agent[0].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper > 0){

                    Agent[0].goalkeeper = Agent[0].goalkeeper - 1;

                }
                printf("Club-B transfers a %s for %dM euros from Agent-1 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,B.budget);
            }
            else{
                printf("Club-B cannot transfer a %s from Agent-1 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward == 0){
                printf("Club-B cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder == 0){
                printf("Club-B cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender == 0){
                printf("Club-B cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper == 0){
                printf("Club-B cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-B cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[1]);


    }
    else if(clubName == 2){
        printf("Club C negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(C.budget >= players[pID].Pbudget){
                C.budget = C.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "C");

                if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward > 0){

                    Agent[0].forward = Agent[0].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder > 0){

                    Agent[0].midfielder = Agent[0].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender > 0){

                    Agent[0].defender = Agent[0].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper > 0){

                    Agent[0].goalkeeper = Agent[0].goalkeeper - 1;

                }
                printf("Club-C transfers a %s for %dM euros from Agent-1 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,C.budget);
            }
            else{
                printf("Club-C cannot transfer a %s from Agent-1 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward == 0){
                printf("Club-C cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder == 0){
                printf("Club-C cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender == 0){
                printf("Club-C cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper == 0){
                printf("Club-C cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-C cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[2]);


    }
    else if(clubName == 3){
        printf("Club D negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(D.budget >= players[pID].Pbudget){
                D.budget = D.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "D");

                if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward > 0){
                    
                    Agent[0].forward = Agent[0].forward - 1;
                    
                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder > 0){
                    
                    Agent[0].midfielder = Agent[0].midfielder - 1;
                    

                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender > 0){
                    
                    Agent[0].defender = Agent[0].defender - 1;
                    
                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper > 0){
                    
                    Agent[0].goalkeeper = Agent[0].goalkeeper - 1;
                    
                }
                printf("Club-D transfers a %s for %dM euros from Agent-1 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,D.budget);
            }
            else{
                printf("Club-D cannot transfer a %s from Agent-1 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward == 0){
                printf("Club-D cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder == 0){
                printf("Club-D cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender == 0){
                printf("Club-D cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper == 0){
                printf("Club-D cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-D cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[3]);


    }
    else if(clubName == 4){
        printf("Club E negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(E.budget >= players[pID].Pbudget){
                E.budget = E.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "E");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward > 0){

                    Agent[0].forward = Agent[0].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder > 0){

                    Agent[0].midfielder = Agent[0].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender > 0){

                    Agent[0].defender = Agent[0].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper > 0){

                    Agent[0].goalkeeper = Agent[0].goalkeeper - 1;

                }
                printf("Club-E transfers a %s for %dM euros from Agent-1 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,E.budget);
            }
            else{
                printf("Club-E cannot transfer a %s from Agent-1 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[0].forward == 0){
                printf("Club-E cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[0].midfielder == 0){
                printf("Club-E cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[0].defender == 0){
                printf("Club-E cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[0].goalkeeper == 0){
                printf("Club-E cannot transfer a forward from Agent-1 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-E cannot transfer player is gone CLub %s\n",players[pID].club);
            }

            }

        sem_post(&Club[4]);
    }
}

// Agent2 does same think with Agent1. Agent1 explaned.
void *Agent2(void *num){
    struct thread_data *data1;
    data1 = (struct thread_data *) num;

    int clubName = data1->c; // Club A -> 0, Club B -> 1, Club C -> 2 ...
    int pID = data1->p_id;  // Player Id
    sem_wait(&negotiate[1]);
    if(clubName == 0){
        printf("Club A negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(A.budget >= players[pID].Pbudget){
                A.budget = A.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "A");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward > 0){

                    Agent[1].forward = Agent[1].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder > 0){

                    Agent[1].midfielder = Agent[1].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender > 0){

                    Agent[1].defender = Agent[1].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper > 0){

                    Agent[1].goalkeeper = Agent[1].goalkeeper - 1;

                }
                printf("Club-A transfers a %s for %dM euros from Agent-2 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,A.budget);
            }
            else{
                printf("Club-A cannot transfer a %s from Agent-2 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward == 0){
                printf("Club-A cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder == 0){
                printf("Club-A cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender == 0){
                printf("Club-A cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper == 0){
                printf("Club-A cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-A cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[0]);

    }
    else if(clubName == 1){
        printf("Club B negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(B.budget >= players[pID].Pbudget){
                B.budget = B.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "B");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward > 0){

                    Agent[1].forward = Agent[1].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder > 0){

                    Agent[1].midfielder = Agent[1].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender > 0){

                    Agent[1].defender = Agent[1].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper > 0){

                    Agent[1].goalkeeper = Agent[1].goalkeeper - 1;

                }
                printf("Club-B transfers a %s for %dM euros from Agent-2 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,B.budget);
            }
            else{
                printf("Club-B cannot transfer a %s from Agent-2 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward == 0){
                printf("Club-B cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder == 0){
                printf("Club-B cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender == 0){
                printf("Club-B cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper == 0){
                printf("Club-B cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-B cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[1]);


    }
    else if(clubName == 2){
        printf("Club C negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(C.budget >= players[pID].Pbudget){
                C.budget = C.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "C");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward > 0){

                    Agent[1].forward = Agent[1].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder > 0){

                    Agent[1].midfielder = Agent[1].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender > 0){

                    Agent[1].defender = Agent[1].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper > 0){

                    Agent[1].goalkeeper = Agent[1].goalkeeper - 1;

                }
                printf("Club-C transfers a %s for %dM euros from Agent-2 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,C.budget);
            }
            else{
                printf("Club-C cannot transfer a %s from Agent-2 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward == 0){
                printf("Club-C cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder == 0){
                printf("Club-C cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender == 0){
                printf("Club-C cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper == 0){
                printf("Club-C cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-C cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[2]);


    }
    else if(clubName == 3){
        printf("Club D negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(D.budget >= players[pID].Pbudget){
                D.budget = D.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "D");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward > 0){

                    Agent[1].forward = Agent[1].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder > 0){

                    Agent[1].midfielder = Agent[1].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender > 0){

                    Agent[1].defender = Agent[1].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper > 0){

                    Agent[1].goalkeeper = Agent[1].goalkeeper - 1;

                }
                printf("Club-D transfers a %s for %dM euros from Agent-2 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,D.budget);
            }
            else{
                printf("Club-D cannot transfer a %s from Agent-2 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward == 0){
                printf("Club-D cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder == 0){
                printf("Club-D cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender == 0){
                printf("Club-D cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper == 0){
                printf("Club-D cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-D cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[3]);


    }
    else if(clubName == 4){
        printf("Club E negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(E.budget >= players[pID].Pbudget){
                E.budget = E.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "E");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward > 0){

                    Agent[1].forward = Agent[1].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder > 0){

                    Agent[1].midfielder = Agent[1].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender > 0){

                    Agent[1].defender = Agent[1].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper > 0){

                    Agent[1].goalkeeper = Agent[1].goalkeeper - 1;

                }
                printf("Club-E transfers a %s for %dM euros from Agent-2 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,E.budget);
            }
            else{
                printf("Club-E cannot transfer a %s from Agent-2 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[1].forward == 0){
                printf("Club-E cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[1].midfielder == 0){
                printf("Club-E cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[1].defender == 0){
                printf("Club-E cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[1].goalkeeper == 0){
                printf("Club-E cannot transfer a forward from Agent-2 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-E cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[4]);


    }
}
// Agent3 does same think with Agent1. Agent1 explaned.
void *Agent3(void *num){
    struct thread_data *data2;
    data2 = (struct thread_data *) num;

    int clubName = data2->c; // Club A -> 0, Club B -> 1, Club C -> 2 ...
    int pID = data2->p_id;  // Player Id
    sem_wait(&negotiate[2]);
    if(clubName == 0){
        printf("Club A negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(A.budget >= players[pID].Pbudget){
                A.budget = A.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "A");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward > 0){

                    Agent[2].forward = Agent[2].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0)  && Agent[2].midfielder > 0){

                    Agent[2].midfielder = Agent[2].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0)  && Agent[2].defender > 0){

                    Agent[2].defender = Agent[2].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0)  && Agent[2].goalkeeper > 0){

                    Agent[2].goalkeeper = Agent[2].goalkeeper - 1;

                }
                printf("Club-A transfers a %s for %dM euros from Agent-3 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,A.budget);
            }
            else{
                printf("Club-A cannot transfer a %s from Agent-3 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward == 0){
                printf("Club-A cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder == 0){
                printf("Club-A cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender == 0){
                printf("Club-A cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper == 0){
                printf("Club-A cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-A cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }


        sem_post(&Club[0]);

    }
    else if(clubName == 1){
        printf("Club B negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(B.budget >= players[pID].Pbudget){
                B.budget = B.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "B");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward > 0){

                    Agent[2].forward = Agent[2].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder > 0){

                    Agent[2].midfielder = Agent[2].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0)  && Agent[2].defender > 0){

                    Agent[2].defender = Agent[2].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0)  && Agent[2].goalkeeper > 0){

                    Agent[2].goalkeeper = Agent[2].goalkeeper - 1;

                }
                printf("Club-B transfers a %s for %dM euros from Agent-3 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,B.budget);
            }
            else{
                printf("Club-B cannot transfer a %s from Agent-3 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward == 0){
                printf("Club-B cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder == 0){
                printf("Club-B cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender == 0){
                printf("Club-B cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper == 0){
                printf("Club-B cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-B cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[1]);


    }
    else if(clubName == 2){
        printf("Club C negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);
        if(strcmp(players[pID].club , "x")==0){
            if(C.budget >= players[pID].Pbudget){
                C.budget = C.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "C");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward > 0){

                    Agent[2].forward = Agent[2].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder > 0){

                    Agent[2].midfielder = Agent[2].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender > 0){

                    Agent[2].defender = Agent[2].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper > 0){

                    Agent[2].goalkeeper = Agent[2].goalkeeper - 1;

                }
                printf("Club-C transfers a %s for %dM euros from Agent-3 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,C.budget);
            }
            else{
                printf("Club-C cannot transfer a %s from Agent-3 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward == 0){
                printf("Club-C cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder == 0){
                printf("Club-C cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender == 0){
                printf("Club-C cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper == 0){
                printf("Club-C cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-C cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[2]);


    }
    else if(clubName == 3){
        printf("Club D negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);
        if(strcmp(players[pID].club , "x")==0){
            if(D.budget >= players[pID].Pbudget){
                D.budget = D.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "D");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward > 0){

                    Agent[2].forward = Agent[2].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder > 0){

                    Agent[2].midfielder = Agent[2].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender > 0){

                    Agent[2].defender = Agent[2].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper > 0){

                    Agent[2].goalkeeper = Agent[2].goalkeeper - 1;

                }
                printf("Club-D transfers a %s for %dM euros from Agent-3 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,D.budget);
            }
            else{
                printf("Club-D cannot transfer a %s from Agent-3 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward == 0){
                printf("Club-D cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder == 0){
                printf("Club-D cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender == 0){
                printf("Club-D cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper == 0){
                printf("Club-D cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-D cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[3]);


    }
    else if(clubName == 4){
        printf("Club E negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);
        if(strcmp(players[pID].club , "x")==0){
            if(E.budget >= players[pID].Pbudget){
                E.budget = E.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "E");
                if((strcmp(players[pID].position , "Forward")==0)&& Agent[2].forward > 0){

                    Agent[2].forward = Agent[2].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0)&& Agent[2].midfielder > 0){

                    Agent[2].midfielder = Agent[2].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0)&& Agent[2].defender > 0){

                    Agent[2].defender = Agent[2].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper > 0){

                    Agent[2].goalkeeper = Agent[2].goalkeeper - 1;

                }
                printf("Club-E transfers a %s for %dM euros from Agent-3 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,E.budget);
            }
            else{
                printf("Club-E cannot transfer a %s from Agent-3 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[2].forward == 0){
                printf("Club-E cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[2].midfielder == 0){
                printf("Club-E cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[2].defender == 0){
                printf("Club-E cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[2].goalkeeper == 0){
                printf("Club-E cannot transfer a forward from Agent-3 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-E cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[4]);


    }
}
// Agent4 does same think with Agent1. Agent1 explaned.
void *Agent4(void *num){
    struct thread_data *data3;
    data3 = (struct thread_data *) num;

    int clubName = data3->c; // Club A -> 0, Club B -> 1, Club C -> 2 ...
    int pID = data3->p_id;  // Player Id
    sem_wait(&negotiate[3]);
    if(clubName == 0){
        printf("Club A negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(A.budget >= players[pID].Pbudget){
                A.budget = A.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "A");
                if( (strcmp(players[pID].position , "Forward")==0) && Agent[3].forward > 0){

                    Agent[3].forward = Agent[3].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0)&& Agent[3].midfielder > 0){

                    Agent[3].midfielder = Agent[3].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender > 0){

                    Agent[3].defender = Agent[3].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0)  && Agent[3].goalkeeper > 0){

                    Agent[3].goalkeeper = Agent[3].goalkeeper - 1;

                }
                printf("Club-A transfers a %s for %dM euros from Agent-4 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,A.budget);
            }
            else{
                printf("Club-A cannot transfer a %s from Agent-4 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward == 0){
                printf("Club-A cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder == 0){
                printf("Club-A cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender == 0){
                printf("Club-A cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper == 0){
                printf("Club-A cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-A cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[0]);

    }
    else if(clubName == 1){
        printf("Club B negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(B.budget >= players[pID].Pbudget){
                B.budget = B.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "B");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward > 0){

                    Agent[3].forward = Agent[3].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder > 0){

                    Agent[3].midfielder = Agent[3].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender > 0){

                    Agent[3].defender = Agent[3].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper > 0){

                    Agent[3].goalkeeper = Agent[3].goalkeeper - 1;

                }
                printf("Club-B transfers a %s for %dM euros from Agent-4 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,B.budget);
            }
            else{
                printf("Club-B cannot transfer a %s from Agent-4 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward == 0){
                printf("Club-B cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder == 0){
                printf("Club-B cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender == 0){
                printf("Club-B cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper == 0){
                printf("Club-B cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-B cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[1]);


    }
    else if(clubName == 2){
        printf("Club C negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);
        if(strcmp(players[pID].club , "x")==0){
            if(C.budget >= players[pID].Pbudget){
                C.budget = C.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "C");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward > 0){

                    Agent[3].forward = Agent[3].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder > 0){

                    Agent[3].midfielder = Agent[3].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender > 0){

                    Agent[3].defender = Agent[3].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper > 0){

                    Agent[3].goalkeeper = Agent[3].goalkeeper - 1;

                }
                printf("Club-C transfers a %s for %dM euros from Agent-4 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,C.budget);
            }
            else{
                printf("Club-C cannot transfer a %s from Agent-4 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward == 0){
                printf("Club-C cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder == 0){
                printf("Club-C cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender == 0){
                printf("Club-C cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper == 0){
                printf("Club-C cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-C cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[2]);

    }
    else if(clubName == 3){
        printf("Club D negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(D.budget >= players[pID].Pbudget){
                D.budget = D.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "D");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward > 0){

                    Agent[3].forward = Agent[3].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder > 0){

                    Agent[3].midfielder = Agent[3].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender > 0){

                    Agent[3].defender = Agent[3].defender - 1;

                }
                else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper > 0){

                    Agent[3].goalkeeper = Agent[3].goalkeeper - 1;

                }
                printf("Club-D transfers a %s for %dM euros from Agent-4 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,D.budget);
            }
            else{
                printf("Club-D cannot transfer a %s from Agent-4 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward == 0){
                printf("Club-D cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder == 0){
                printf("Club-D cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender == 0){
                printf("Club-D cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper == 0){
                printf("Club-D cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-D cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }
        sem_post(&Club[3]);


    }
    else if(clubName == 4){
        printf("Club E negotiates with Agent- %d for a %s player\n",  players[pID].AgentID, players[pID].position);

        if(strcmp(players[pID].club , "x")==0){
            if(E.budget >= players[pID].Pbudget){
                E.budget = E.budget - players[pID].Pbudget;
                strcpy(players[pID].club, "E");
                if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward > 0){

                    Agent[3].forward = Agent[0].forward - 1;

                }
                else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder > 0){

                    Agent[3].midfielder = Agent[3].midfielder - 1;


                }
                else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender > 0){

                    Agent[3].defender = Agent[3].defender - 1;

                }
                else if(strcmp(players[pID].position , "Goalkeeper")==0){

                    Agent[3].goalkeeper = Agent[3].goalkeeper - 1;

                }
                printf("Club-E transfers a %s for %dM euros from Agent-4 (Budget:%dM)\n",players[pID].position,players[pID].Pbudget,E.budget);
            }
            else{
                printf("Club-E cannot transfer a %s from Agent-4 since the player is worth %dM euros\n", players[pID].position,players[pID].Pbudget);
            }
        }
        else{

            if((strcmp(players[pID].position , "Forward")==0) && Agent[3].forward == 0){
                printf("Club-E cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Midfielder")==0) && Agent[3].midfielder == 0){
                printf("Club-E cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);

            }
            else if((strcmp(players[pID].position , "Defender")==0) && Agent[3].defender == 0){
                printf("Club-E cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else if((strcmp(players[pID].position , "Goalkeeper")==0) && Agent[3].goalkeeper == 0){
                printf("Club-E cannot transfer a forward from Agent-4 since no %s left\n",players[pID].position);
            }
            else{
                printf("Club-E cannot transfer player is gone CLub %s\n",players[pID].club);
            }
        }

        sem_post(&Club[4]);


    }
}
