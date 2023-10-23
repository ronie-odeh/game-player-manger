#ifndef HW2_PLAYERSMANAGER_H
#define HW2_PLAYERSMANAGER_H

#include <memory>
#include "group.h"

typedef enum {
    SUCCESS_ = 0,
    FAILURE_ = -1,
    ALLOCATION_ERROR_ = -2,
    INVALID_INPUT_ = -3
} Status;

class PlayersManager{
    typedef struct node_t {
        int numOfSons; //number of elements in the uptree
        int index; //group # aka groupID
        group data;
        node_t* parent;
        node_t(int i, int scale_) : numOfSons(0), index(i),data(group(i,scale_)), parent(nullptr){
        }
        ~node_t() = default;
    }*Node;

    int k;
    int scale;
    Node* *groupsArr; //index 0 has for the whole structure i.e. for all players

    group* getData(Node r){
        if(!r)
            return nullptr;
        return &(r->data);
    }

    int getNumSons(Node r){
        if(!r)
            return -1; //fail
        return r->numOfSons;
    }

//    void setParent(Node* r, Node* p){
//        (*r)->parent = (*p);
//    }

    Node* Find(int id) { //TODO update num of sons? or doesnt matter since we only look at num of sons for roots anyways?
        //will not be given id 0 //TODO check that id is in bounds or do this in playersmanager funcitons
        if(!((*groupsArr[id])->parent)){
            return groupsArr[id];
        }
        Node* current = groupsArr[id];
        int* tmp = new int[k];
        int i = 0;
        tmp[i] = (*current)->index;
        while ((*current)->parent) {
            current = &((*current)->parent);
            i++;
            tmp[i] = (*current)->index;
        }
        for (int j = 0; j < i; j++)
            (*groupsArr[tmp[j]])->parent = *current;
        delete[] tmp;
        return current;
    }

    void Unite(int groupID1, int groupID2){
        Node* root1 = Find(groupID1);
        Node* root2 = Find(groupID2);
        if(root1 == root2)
            return;

        //merge group nodes - merge levelsTree, merge scoresArr, merge hash tables, sum zero_players
        int size1 = getNumSons(*root1);
        int size2 = getNumSons(*root2);

        if(size1 >= size2){
            (*root2)->parent = (*root1);
            (*root1)->numOfSons = (*root1)->numOfSons + (*root2)->numOfSons + 1;
            getData(*root1)->mergeGroupWith(*getData(*root2)); //merges and clears root2 data
        }
        else{
            (*root1)->parent = (*root2);
            (*root2)->numOfSons = (*root2)->numOfSons + (*root1)->numOfSons + 1;
            getData(*root2)->mergeGroupWith(*getData(*root1)); //merges and clears root1 data
        }
    }

    double round(double var){
        double value = (int)((var * 10000) + 0.5);
        return (double)value / 10000;
    }

public:
    PlayersManager(int k_, int scale_) : k(k_), scale(scale_){
        groupsArr = new Node*[k+1];
        for(int i=0; i<k+1; i++){
            Node to_add = new node_t(i, scale);
            groupsArr[i] = new Node(to_add);
        }
    }
    ~PlayersManager(){
        for(int i=0; i<k+1; i++){
            delete (*(groupsArr[i]));
            delete groupsArr[i];
        }
        delete[] groupsArr;
    }

    Status MergeGroups(int GroupID1, int GroupID2);
    Status AddPlayer(int PlayerID, int GroupID, int score);
    Status RemovePlayer(int PlayerID);
    Status IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    Status ChangePlayerIDScore(int PlayerID, int NewScore);
    Status GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players);
    Status AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
    Status GetPlayersBound(int GroupID, int score, int m,
                               int * LowerBoundPlayers, int * HigherBoundPlayers);
};

#endif //HW2_PLAYERSMANAGER_H
