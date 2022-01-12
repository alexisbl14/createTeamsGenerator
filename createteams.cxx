#include "priorityqueue.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib> 
#include <cmath>
#include <vector>

using namespace std;
using namespace nlohmann;
int main(int argc, char** argv) {
    ifstream file1;  //declare file1

    json teamData_object;   //declare json input object
    file1.open(argv[1]);    //open file, if opened successfully, put into teamData_object
    if (file1.is_open()){
        file1 >> teamData_object;
    }
    file1.close();

    int numPlayers = teamData_object["metadata"]["numPlayers"];   //initialize number of players
    int numTeams = (numPlayers/2);                                //calculate number of teams
    int max_size = (numPlayers*(numPlayers-1))/2;                //calculate max size of priority_queue - (n(n-1)/2)
    PriorityQueue teams_queue(max_size);                        //initialize team priority queue

    teamData_object.erase("metadata");                         //erasing metadata to iterate through json easier

    for(auto itr = teamData_object["teamStats"].begin(); itr != teamData_object["teamStats"].end(); itr ++){    //start of iterator
        int p1 = itr.value()["playerOne"];                             //set p1 to playerOne in json object
        int p2 = itr.value()["playerTwo"];                              //set p2 to playerTwo in json object
        double winPercentage = itr.value()["winPercentage"];           //set winPercentage to value of winPercentage in json
        double winPercentage_diff = abs(50 - winPercentage);           //using variable winPercentage_diff to find how close winPercentage is to 50
        KeyValuePair kv = std::make_pair(winPercentage_diff, std::make_pair(p1, p2));    //creating a KeyValuePair to insert into queue
        teams_queue.insert(kv);            //insert into min queue ordered by winPercentage_diff (closest to 50)
    }

    vector<pair<int, int>> team_vector;      //initialize vector of teams
    int i = 0;
    bool inTeam;                            //initialize boolean for if player is in a team or not
    while (i < numTeams){                    //iterate until all teams are made
        inTeam = false;                              //set initial value of inTeam to false at start of loop in case it was set to true
        int p1 = teams_queue.min().second.first;     //p1 is the first player in the min winPercentage_diff of team queue
        int p2 = teams_queue.min().second.second;     //p2 is the second player in the min winPercentage_diff of team queue
        for (unsigned int j = 0; j < team_vector.size(); j++){   //iterating through the vector of teams to see if players are in a team
            if(team_vector[j].first == p1 || team_vector[j].second == p1 || team_vector[j].first == p2 || team_vector[j].second == p2){
                inTeam = true;   //if team_vector contains either p1 or p2 defined above, inTeam is set to true
            }
        }
        if(inTeam){
            teams_queue.removeMin();  //if players are inTeam, then just remove min from team queue
        }
        else if (!inTeam){
            pair<int,int> team = make_pair(p1, p2);  //if players are not inTeam, then make a pair called a team
            teams_queue.removeMin();                 //then remove min from team queue
            team_vector.push_back(team);              //then add the pair team to the team vector
            i ++;                                    //increment number of teams created
        }
    }

    json teamJSON_object;                         //create json object for output, set teams value to team_vector
    teamJSON_object["teams"] = team_vector;

    std::cout << teamJSON_object.dump(2) << std::endl;  //ouput json object
}
