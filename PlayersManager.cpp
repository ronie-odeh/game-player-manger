#include "PlayersManager.h"

Status PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    if(GroupID1 <= 0 || GroupID1 > k || GroupID2 <= 0 || GroupID2 > k){
        return INVALID_INPUT_;
    }
    Node* root1 = Find(GroupID1);
    Node* root2 = Find(GroupID2);
    if(root1 == root2){ //this checks pointers //TODO should we check the actual node/group structs aka make a == operator
        return SUCCESS_;
    }
    Unite((*root1)->index, (*root2)->index);
    return SUCCESS_;
}

Status PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if (GroupID <= 0 || GroupID > k || PlayerID <= 0 || score > scale || score <= 0) {
        return INVALID_INPUT_;
    }
    Node* root = Find(GroupID);
    player* found_player = getData(*groupsArr[0])->findPlayer(PlayerID); //TODO
    if(found_player != nullptr){ //if this playerID is already in the structure
        return FAILURE_;
    }
    bool res = getData(*root)->addPlayerToGroup(PlayerID, GroupID, score, 0);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    //add to entire structure info (i.e. groupID=0)
    res = getData(*groupsArr[0])->addPlayerToGroup(PlayerID, GroupID, score, 0);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    return SUCCESS_;
}

Status PlayersManager::RemovePlayer(int PlayerID){
    if(PlayerID <=0){
        return INVALID_INPUT_;
    }
    player* found = getData(*groupsArr[0])->findPlayer(PlayerID);
    if(found == nullptr){
        return FAILURE_;
    }
    int groupID = found->getGroupID();
    int score = found->getScore();
    int level = found->getLevel();
    //remove from entire structure
    getData(*groupsArr[0])->removePlayerFromGroup(PlayerID, score, level);
    //remove from the group that it is in
    Node* root = Find(groupID);
    getData(*root)->removePlayerFromGroup(PlayerID, score, level);
    return SUCCESS_;
}

Status PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if(PlayerID <= 0 || LevelIncrease <= 0){
        return INVALID_INPUT_;
    }
    player* found = getData(*groupsArr[0])->findPlayer(PlayerID);
    if(found == nullptr){
        return FAILURE_;
    }
    int groupID = found->getGroupID();
    int score = found->getScore();
    int old_level = found->getLevel();
    int new_level = old_level + LevelIncrease;
    //update level of player in the entire structure
    getData(*groupsArr[0])->removePlayerFromGroup(PlayerID, score, old_level);
    bool res = getData(*groupsArr[0])->addPlayerToGroup(PlayerID, groupID, score, new_level);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    //update level of player in its group
    Node* root = Find(groupID);
    getData(*root)->removePlayerFromGroup(PlayerID, score, old_level); //(*root)->data->removePlayerFromGroup(PlayerID, score, old_level);
    res = getData(*root)->addPlayerToGroup(PlayerID, groupID, score, new_level);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    return SUCCESS_;
}

Status PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore){
    if(PlayerID <= 0 || NewScore > scale || NewScore <= 0){
        return INVALID_INPUT_;
    }
    player* found = getData(*groupsArr[0])->findPlayer(PlayerID);
    if(found == nullptr){
        return FAILURE_;
    }
    int old_score = found->getScore();
    int groupID = found->getGroupID();
    int level = found->getLevel();
    Node* root = Find(groupID);
    bool res = getData(*groupsArr[0])->updatePlayerScore(PlayerID, groupID, level, old_score, NewScore);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    res = getData(*root)->updatePlayerScore(PlayerID, groupID, level, old_score, NewScore);
    if(!res){
        return ALLOCATION_ERROR_;
    }
    return SUCCESS_;
}

Status PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel,
                                                            int higherLevel, double* players){
    //we can assume lowerLevel<=higherLevel
    //return failure if there are no players between lowerLevel and higherLevel

    if(GroupID > k || GroupID < 0){
        return INVALID_INPUT_;
    }
    if(lowerLevel > higherLevel){
        return FAILURE_;
    }
    if(score < 1 || score > scale){
        *players = 0;
        return SUCCESS_;
    }
    Node* root = groupsArr[0];
    if(GroupID != 0){
        root = Find(GroupID);
    }
    int total_in_range = getData(*root)->numPlayersInRange(lowerLevel, higherLevel);
    if(total_in_range == 0){
        return FAILURE_;
    }
    int with_given_score = getData(*root)->numPlayersInRangeWithScore(score, lowerLevel, higherLevel);
    double calc = (double(with_given_score) / double(total_in_range)) * 100;
    *players = calc;
    return SUCCESS_;
}

Status PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level){
    if(GroupID > k || GroupID < 0 || m <= 0 || !level){
        return INVALID_INPUT_;
    }
    Node* root = groupsArr[0];
    if(GroupID != 0){
        root = Find(GroupID);
    }
    int zero_p = getData(*root)->getZeroPlayers();
    int sum = getData(*root)->getLevelsTree()->select_m(m, zero_p);
    if(sum == -1)
        return FAILURE_;
    *level = double(sum)/double(m);
    return SUCCESS_;
}

Status PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                       int * LowerBoundPlayers, int * HigherBoundPlayers){
    return SUCCESS_;
}

