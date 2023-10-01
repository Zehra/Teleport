// Teleport.cpp

#include "bzfsAPI.h"
#include "../src/bzfs/bzfs.h"
#include "../src/bzfs/GameKeeper.h"

int SetPlayerDead(int playerID) {
    int status = 0;
    GameKeeper::Player *p = GameKeeper::Player::getPlayerByIndex(playerID);
    printf("playerID: %d\n", playerID);
    if (p == NULL) {
        //puts("null");
        status= -1;
    } else {
        if (p->player.isObserver()) {
            //puts("obs");
            status= -2;
        } else {
            if (p->player.isAlive()) {
                 //puts("alive");
                status=1;
                p->player.setDead();
                //TODO: Check for teams.
                // We do it in the logic, but if we want to use this as a library, we'll need to do it elsewhere.
                p->player.setRestartOnBase(false);
            } else {
                 //puts("dead");
                status=2;
            }
        }
    }
    return status;
}


void ForcePlayerSpawn(int playerID) {
    playerAlive(playerID);
}


// Utility functions.
int checkRange(int min, int max, int amount);
int checkPlayerSlot(int player);

class Teleport : public bz_Plugin, bz_CustomSlashCommandHandler
{
public:
  const char* Name(){return "Teleport";}
  void Init ( const char* /*config*/ );
  void Event(bz_EventData *eventData );
  void Cleanup ( void );
  bool SlashCommand ( int playerID, bz_ApiString command, bz_ApiString message, bz_APIStringList* params );
    int spawn = -1;
    float playerPos[4];

};

BZ_PLUGIN(Teleport)

void Teleport::Init (const char* commandLine) {
  bz_registerCustomSlashCommand ( "teleport", this );
  //bz_registerCustomSlashCommand ( "", this );
  //bz_registerCustomSlashCommand ( "", this );
  //Register(bz_ePlayerJoinEvent);
  //Register(bz_ePlayerPartEvent);
  //Register(bz_ePlayerDieEvent);
  //Register();
  //Register();
  //Register();
  Register(bz_eGetPlayerSpawnPosEvent);
}

void Teleport::Cleanup (void) {
  bz_removeCustomSlashCommand ( "teleport" );
  //bz_removeCustomSlashCommand ( "" );
  //bz_removeCustomSlashCommand ( "" );
  Flush();
}

bool Teleport::SlashCommand ( int playerID, bz_ApiString command, bz_ApiString message, bz_APIStringList* params ) {
  if (command == "teleport"){
        bz_BasePlayerRecord *playRec = bz_getPlayerByIndex ( playerID );
        if (playRec != NULL) {
            if (playRec->spawned) {
                playerPos[0]= playRec->lastKnownState.pos[0];
                playerPos[1]= playRec->lastKnownState.pos[1];
                playerPos[2]= playRec->lastKnownState.pos[2];
                playerPos[3]= playRec->lastKnownState.rotation;
                spawn = playerID;
                SetPlayerDead(playerID);  
               //bz_killPlayer(playerID, false, BZ_SERVER);
                ForcePlayerSpawn(playerID);
            }
        }
        bz_freePlayerRecord(playRec);
    return true;
  }
  return false;
}

void Teleport::Event(bz_EventData *eventData ){
  switch (eventData->eventType) {
    /*case bz_ePlayerJoinEvent: {
      bz_PlayerJoinPartEventData_V1* joinData = (bz_PlayerJoinPartEventData_V1*)eventData;
      int player = joinData->playerID;
        
    }break;*/

    /*case bz_ePlayerPartEvent: {
      bz_PlayerJoinPartEventData_V1* partData = (bz_PlayerJoinPartEventData_V1*)eventData;
      int player = partData->playerID;
        
    }break;*/

      /*case bz_ePlayerDieEvent: {
      bz_PlayerDieEventData_V2* deathData = (bz_PlayerDieEventData_V2*)eventData;

// Data
// ---
// (int)          playerID - ID of the player who was killed.
// (bz_eTeamType) team - The team the killed player was on.
// (int)          killerID - The owner of the shot that killed the player, or BZ_SERVER for server side kills
// (bz_eTeamType) killerTeam - The team the owner of the shot was on.
// (bz_ApiString) flagKilledWith - The flag name the owner of the shot had when the shot was fired.
// (int)          flagHeldWhenKilled - The ID of the flag the victim was holding when they died.
// (int)          shotID - The shot ID that killed the player, if the player was not killed by a shot, the id will be -1.
// (bz_PlayerUpdateState) state - The state record for the killed player at the time of the event
// (double)       eventTime - Time of the event on the server.
        
    }break;*/
     
      /*case : {
      
        
    }break;*/

     
    case bz_eGetPlayerSpawnPosEvent: {
      bz_GetPlayerSpawnPosEventData_V1* spawnPosData = (bz_GetPlayerSpawnPosEventData_V1*)eventData;
      puts("spawn");
      int player = spawnPosData->playerID;
      //if (spawn != -1) {
        if (player == spawn) {
            puts("handled");
            spawnPosData->handled = true;
            spawnPosData->pos[0]= playerPos[0];
            spawnPosData->pos[1]= playerPos[1];
            spawnPosData->pos[2]= (playerPos[2] + 50.0);
            spawnPosData->rot   = playerPos[3];
            spawn = -1;
        }
      //}
        
    }break;

    default:{ 
    }break;
  }
}


int checkRange(int min, int max, int amount) {
    int num = 0;
    if ((amount >= min) && (amount <= max)) {
        num = 1;
    } else if ((amount < min) || (amount > max)) {
        num = 0;
    } else {
        num = -1;
    }
    return num;
}

int checkPlayerSlot(int player) {
    return checkRange(0,199,player); // 199 because of array.
}

// Various code samples/snipplets:

/* //Loop with proper alloc/dealloc of playerlist, also base player records.
    bz_APIIntList *playerList = bz_newIntList();
    if (playerList) {
    
    bz_getPlayerIndexList(playerList);
    for ( unsigned int i = 0; i < playerList->size(); i++) {
    int targetID = (*playerList)[i];
    bz_BasePlayerRecord *playRec = bz_getPlayerByIndex ( targetID );

        if (playRec != NULL) {
            if (playRec->spawned) {
                bz_killPlayer(targetID, true, BZ_SERVER);
                if (noLoss)
                    bz_incrementPlayerLosses(targetID, -1);
            }
        }
        bz_freePlayerRecord(playRec);
    }
    // Originally deleted playerlist here.
    }
    bz_deleteIntList(playerList);

*/




