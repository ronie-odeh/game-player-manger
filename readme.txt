project that manages the game of K groups of players, for each player theres a score which ranges between 1 and 200, and a level.
function for managing the game:
init(int k, int scale): initialize the game data structures with the number of the groups and max scale. time complexity: O(K)
mergeGroups: merge two different groups into one group. time complexity: O(log*(k) + n) where n is number of players in both groups.
addPlayer: adds new player with level=0 with initial score. time complexity: O(K)
removePlayer: remove an exisiting player. time complexity: O(log*(k) + log(n)) where n is number of players in the game.
increasePlayerIDLevel: increasing a specific player level. time complexity: O(log*(k) + log(n)) where n is n is number of players in the game.
changePlayerIDScore: change a specific player score. time complexity: O(log*(k) + log(n)) where n is number of players in the game.
getPercentOfPlayersWithScoreInBounds: get percentage of players with score equal to given score, players must have level in range [lower_bound, high_bound] which both are given. time complexity: O(log*(k) + log(n)) where n is number of players.
averageHighestPlayerLevelByGroup: get average level of the highest m (m is given) players in group. time complexity: O(log*(k) + log(n)) where n is number of players in group.
  