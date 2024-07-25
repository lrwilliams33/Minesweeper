#pragma once
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

struct Leaderboard{
    multimap<int, string> _leadersMap;

// reads the file
    void readFile(){
        string minString;
        string secString;
        string name;
        int minInt;
        int secInt;
        int timeInSecs;

        ifstream file;
        file.open("files/leaderboard.txt");
        if (file.is_open()){
            while (!file.eof()) {
                // read the data
                getline(file, minString, ':');
                getline(file, secString, ',');
                getline(file, name);

                if (!minString.empty()) {
                    minInt = stoi(minString);
                    secInt = stoi(secString);
                    timeInSecs = (minInt * 60) + secInt;
                    _leadersMap.emplace(timeInSecs, name);
                }
            }
        }
        else {cout << "could not read leaderboard file";}
    }

    // test to make sure the leaderboard is printing correct
    void testPrint(){
        for (auto iter=_leadersMap.begin();iter != _leadersMap.end(); ++iter){
            cout << "Time: " << iter->first << " Name: " << iter->second << endl;
        }
    }

    // writes all the times to the file
    void writeLeaders(){
        int index = 0;
        fstream file;
        file.open("files/leaderboard.txt", ios_base::out | ios_base::trunc);
        for (auto iter=_leadersMap.begin();iter != _leadersMap.end(); ++iter){
            if (index < 5){
                // adds a 0 in front if it needs to. Else convert to string
                int timeMinsInt = iter->first / 60;
                string timeMinString = to_string(timeMinsInt);
                if (timeMinsInt < 10){timeMinString.insert(0, "0");}
                int timeSecsInt = iter->first % 60;
                string timeSecsString = to_string(timeSecsInt);
                if (timeSecsInt < 10){timeSecsString.insert(0, "0");}

                // write to the file
                file << timeMinString << ":" << timeSecsString << "," << iter->second;
                if(index < 4){file << endl;}
            }
            index++;
        }

    }

    // writes all the leaders as one string
    string getStringOfLeaders(){
        string allLeadersStr;
        int index = 1;
        for (auto iter=_leadersMap.begin();iter != _leadersMap.end(); ++iter){
            if (index <= 5) {
                // adds a 0 in front if it needs to. Else convert to string
                int timeMinsInt = iter->first / 60;
                string timeMinString = to_string(timeMinsInt);
                if (timeMinsInt < 10) { timeMinString.insert(0, "0"); }
                int timeSecsInt = iter->first % 60;
                string timeSecsString = to_string(timeSecsInt);
                if (timeSecsInt < 10) { timeSecsString.insert(0, "0"); }
                if (iter->second.back() == '*') { iter->second.pop_back(); }
                if (iter->second.back() == '!') { iter->second.pop_back(); }

                allLeadersStr.append(to_string(index));
                allLeadersStr.append(".\t");
                allLeadersStr.append(timeMinString);
                allLeadersStr.append(":");
                allLeadersStr.append(timeSecsString);
                allLeadersStr.append("\t");
                allLeadersStr.append(iter->second);
                allLeadersStr.append("\n\n");

                index++;
            }
        }
        return allLeadersStr;
    }

    // adds the player to the map;
    void addNewPlayer(string& playerName, int timeSecs){
        string name = playerName;
        name.append("*");
        name.append("!");
        _leadersMap.emplace(timeSecs, name);
    }



};
