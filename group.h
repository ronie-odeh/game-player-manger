#ifndef GROUP_H
#define GROUP_H

#include "RankAVL.h"
#include "hashtable.h"
#include "player.h"

class group{
    int groupID; //if =0 then is for whole structure/all players
    int scale;
    int zero_players; //number of players in level zero
    RankAVL<int> levelsTree; //key: level, data: no data
    RankAVL<int>* *scoresArr;//for each score we have a level tree for the players with this score
    int* zeroScoresArr; //array of the number of players in level zero with scores 1,...,scale
    HashTable<player> playersHash; //dynamic, chain hashing, with avl trees - key is PlayerID (int), data is player struct (PlayerID, GroupID, level, score)
    //a hash table with players in each group (for groupID=0 has all players)

public:
    group(int groupID_, int scale_) : groupID(groupID_), scale(scale_), zero_players(0), levelsTree(),scoresArr(new RankAVL<int>*[scale]),zeroScoresArr(new int[scale]), playersHash(){ //TODO do we need to call constructor for tree??
        // remember score i is at arr[i-1]
        for (int i = 0; i < scale; i++){
            scoresArr[i] = new RankAVL<int>();
            zeroScoresArr[i] = 0;
        }
    }

    group(const group& g) : groupID(g.groupID), scale(g.scale), zero_players(g.zero_players){
        if(g.levelsTree.isEmpty())
            levelsTree = RankAVL<int>();
        else{
            levelsTree = RankAVL<int>();
            levelsTree = g.levelsTree;
        }
        scoresArr = new RankAVL<int>*[scale];
        zeroScoresArr = new int[scale];
        for(int i=0; i<scale; i++){
            zeroScoresArr[i] = g.zeroScoresArr[i];
            scoresArr[i] = new RankAVL<int>();
            *scoresArr[i] = *(g.scoresArr[i]);
        }
        playersHash = HashTable<player>();
        playersHash = g.playersHash;
    }

    group& operator=(const group& g){
        if(this == &g)
            return *this;
        this->groupID = g.groupID;
        this->scale = g.scale;
        this->zero_players = g.zero_players;
        this->levelsTree = g.levelsTree;
        for(int i=0; i<scale; i++){
            zeroScoresArr[i] = g.zeroScoresArr[i];
            *scoresArr[i] = *(g.scoresArr[i]);
        }
        playersHash = g.playersHash;
        return *this;
    }

    ~group(){
        for (int i = 0; i < scale; ++i) {
            if (scoresArr[i] != nullptr) {
                delete scoresArr[i];
            }
        }
        delete[] scoresArr;
        delete[] zeroScoresArr;
    }

    RankAVL<int>* getLevelsTree(){
        return &levelsTree;
    }

    int getZeroPlayers(){
        return zero_players;
    }

    player* findPlayer(int playerID){
        player* found_data = playersHash.find(playerID);
        return found_data;
    }

    bool addPlayerToGroup(int playerID, int GroupID, int score, int level){ //returns false in the case of an allocation error
        bool res = true;
        if(level == 0) { //player added with level=0 so need to update zero_players, zeroScoresArr, hashtable
            zero_players++;
            zeroScoresArr[score-1]++;
        } else{ //player added with level !=0 so need to update levelsTree, scoresArr, hashtable
            res = levelsTree.findAndUpdate(level, true);
            if(!res)
                return false;
            res = scoresArr[score-1]->findAndUpdate(level, true);
            if(!res)
                return false;
        }
        player data(playerID, GroupID, score, level);
        res = this->playersHash.insert(playerID, data);
        return res;
    }

    void removePlayerFromGroup(int playerID, int score, int level){
        if(level == 0){
            this->zero_players--;
            zeroScoresArr[score-1]--;
        }
        else{
            levelsTree.findAndUpdate(level, false);
            scoresArr[score-1]->findAndUpdate(level, false);
        }
        this->playersHash.remove(playerID);
    }

    bool updatePlayerScore(int playerID, int GroupID, int level, int old_score, int new_score){
        //update hash table
        this->playersHash.remove(playerID);
        player data(playerID, GroupID, new_score, level);
        bool res = this->playersHash.insert(playerID, data);
        if(!res){
            return false;
        }
        //update scores array (and zero scores array if need to)
        if(level == 0){
            zeroScoresArr[old_score-1]--;
            zeroScoresArr[new_score-1]++;
            return true;
        }
        else{
            scoresArr[old_score-1]->findAndUpdate(level, false);
            res = scoresArr[new_score-1]->findAndUpdate(level, true);
            return res;
        }
    }

    //*this will be the group node with all the info at the end
    //we will clear the info from root2
    void mergeGroupWith(group& root2){
        //TODO check that parents are nullptrs aka that these are indeed roots?

        //add zero_players
        this->zero_players = this->zero_players + root2.zero_players;
        root2.zero_players = 0;

        //merge levelsTrees
        if(this->levelsTree.isEmpty() && !root2.levelsTree.isEmpty()){
            this->levelsTree = root2.levelsTree;
            //todo check if we destroy the root2 tree.
        }
        //if this is not empty and root2 is empty then do nothing
        //if both are empty then do nothing
        else if(!this->levelsTree.isEmpty() && !root2.levelsTree.isEmpty()) {
            this->levelsTree = mergeRankTrees(&(this->levelsTree), &(root2.levelsTree)); //clears the trees too
        }

        //merge scoresArr (aka merge trees in each cell in scoresArr)
        for(int i = 0; i < scale; i++){
            if(this->scoresArr[i]->isEmpty() && !root2.scoresArr[i]->isEmpty()){
                *this->scoresArr[i] = *root2.scoresArr[i];
            }
            else if(!this->scoresArr[i]->isEmpty() && !root2.scoresArr[i]->isEmpty()) {
                *this->scoresArr[i] = mergeRankTrees(this->scoresArr[i], root2.scoresArr[i]); //clears the trees too
            }
        }

        //merge zeroScoresArr //remember score i is al arr[i-1]
        for(int i = 0; i < scale; i++){
            this->zeroScoresArr[i] = this->zeroScoresArr[i] + root2.zeroScoresArr[i];
            root2.zeroScoresArr[i] = 0;
        }

        //merge hash tables
        HashTable<player> merged_hash = mergeHash(this->playersHash, root2.playersHash);
        this->playersHash = merged_hash;
    }

    //find the number of players (with any score) between lowerLevel and higherLevel
    //lowerLevel and higherLevel are not necessarily in the structure
    int numPlayersInRange(int lowerLevel, int higherLevel){
        //check levelsTree, remember about zero level aka zero_players
        int counter = 0;

        if(lowerLevel <= 0 && higherLevel >= 0){
            counter += zero_players;
        }
        int less_than_lower = levelsTree.sumOfPlayersLessThan(lowerLevel);
        int less_than_higher = levelsTree.sumOfPlayersLessThan(higherLevel);
        int in_higher = levelsTree.find(higherLevel); //find return num_players //if cant find level returns 0
        counter = counter + in_higher + less_than_higher - less_than_lower;
        return counter;
    }

    //find the number of players (with the given score) between lowerLevel and higherLevel
    int numPlayersInRangeWithScore(int score, int lowerLevel, int higherLevel){
        //check tree at scoresArr[score-1], remember about zero level aka zeroScoresArr
        int counter = 0;

        if(lowerLevel <= 0 && higherLevel >= 0){
            counter += zeroScoresArr[score-1];
        }
        int less_than_lower = scoresArr[score-1]->sumOfPlayersLessThan(lowerLevel);
        int less_than_higher = scoresArr[score-1]->sumOfPlayersLessThan(higherLevel);
        int in_higher = scoresArr[score-1]->find(higherLevel); //find return num_players //if cant find level returns 0
        counter = counter + in_higher + less_than_higher - less_than_lower;
        return counter;
    }
};

#endif //GROUP_H
