#include "library2.h"
#include "PlayersManager.h"

void *Init(int k, int scale){
    if(k <= 0 || scale > 200 || scale <= 0){
        return nullptr;
    }
    PlayersManager *DS = new PlayersManager (k, scale);
    return (void*)DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->MergeGroups(GroupID1, GroupID2);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->AddPlayer(PlayerID, GroupID, score);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->RemovePlayer(PlayerID);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->ChangePlayerIDScore(PlayerID, NewScore);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID, score,
                                        lowerLevel, higherLevel, players);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, level);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    Status status = ((PlayersManager *)DS)->GetPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
    if(status == FAILURE_){
        return FAILURE;
    }
    if(status == ALLOCATION_ERROR_){
        return ALLOCATION_ERROR;
    }
    if(status == INVALID_INPUT_){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

void Quit(void** DS){
    if(DS == nullptr){
        return;
    }
    PlayersManager** manager = ((PlayersManager **) DS);
    delete *manager;
    *DS = nullptr;
}