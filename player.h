#ifndef PLAYER_H
#define PLAYER_H

class player{
    int playerID;
    int groupID;
    int score;
    int level;

public:
    player() = default;
    player(int playerID_, int groupID_, int score_, int level_=0) : playerID(playerID_), groupID(groupID_),
                                        score(score_), level(level_){}
    player(const player &p) = default;
    player& operator=(const player &p) = default;
    ~player() = default;

    int getGroupID() const{
        return groupID;
    }

    int getScore() const{
        return score;
    }

    int getLevel() const{
        return level;
    }
};

#endif //PLAYER_H
