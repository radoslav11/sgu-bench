#include<bits/stdc++.h>
using namespace std;

struct Team {
    string name;
    int solved;
    int penalty;
    vector<pair<int,int>> problems; // (solved, penalty_for_problem)
};

int calculateRank(vector<Team>& teams, Team myTeam) {
    int rank = 1;
    for(auto& t : teams) {
        if(t.name == myTeam.name) continue;
        if(t.solved > myTeam.solved) {
            rank++;
        } else if(t.solved == myTeam.solved && t.penalty < myTeam.penalty) {
            rank++;
        }
    }
    return rank;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    while(getline(cin, line)) {
        if(line.empty()) continue;
        
        vector<string> problems;
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while(ss >> token) tokens.push_back(token);
        
        // Parse problem letters (skip "Rank", "Team", "=", "Penalty")
        for(int i = 4; i < tokens.size(); i++) {
            problems.push_back(tokens[i]);
        }
        
        vector<Team> teams;
        
        while(getline(cin, line)) {
            if(line.empty()) break;
            
            stringstream ss(line);
            string rank_str, team_name_part;
            ss >> rank_str;
            
            if(rank_str == "Rank") break; // Header line
            
            vector<string> parts;
            parts.push_back(rank_str);
            
            string remaining;
            getline(ss, remaining);
            stringstream ss2(remaining);
            string part;
            while(ss2 >> part) parts.push_back(part);
            
            if(parts.size() < 3) break;
            
            int solved = stoi(parts[parts.size()-problems.size()-1]);
            int penalty = stoi(parts[parts.size()-problems.size()]);
            
            string teamName = "";
            for(int i = 1; i < parts.size()-problems.size()-1; i++) {
                if(i > 1) teamName += " ";
                teamName += parts[i];
            }
            
            Team t;
            t.name = teamName;
            t.solved = solved;
            t.penalty = penalty;
            
            teams.push_back(t);
        }
        
        int balloons;
        cin >> balloons;
        cin.ignore();
        
        string myTeam;
        getline(cin, myTeam);
        
        // Find my team
        Team me;
        for(auto& t : teams) {
            if(t.name == myTeam) {
                me = t;
                break;
            }
        }
        
        // Best case: solve problems as early as possible (at 241)
        Team best = me;
        best.solved += balloons;
        best.penalty += balloons * 241;
        
        // Worst case: solve problems as late as possible (at 300)
        Team worst = me;
        worst.solved += balloons;
        worst.penalty += balloons * 300;
        
        int minRank = calculateRank(teams, best);
        int maxRank = calculateRank(teams, worst);
        
        cout << minRank << " " << maxRank << "\n";
    }
    
    return 0;
}
