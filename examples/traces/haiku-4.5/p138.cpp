#include<bits/stdc++.h>
using std::vector;
using std::pair;

int n;
vector<int> games;
vector<pair<int,int>> schedule;
vector<int> remaining;

bool solve(int lastWinner) {
    // Check if we're done
    bool allZero = true;
    for(int i = 0; i < n; i++) {
        if(remaining[i] > 0) {
            allZero = false;
            break;
        }
    }
    if(allZero) return true;
    
    // Try pairing lastWinner with each other player who still has games
    for(int i = 0; i < n; i++) {
        if(i != lastWinner && remaining[i] > 0) {
            // Try lastWinner wins
            remaining[lastWinner]--;
            remaining[i]--;
            schedule.push_back({lastWinner + 1, i + 1});
            
            if(solve(lastWinner)) return true;
            
            schedule.pop_back();
            remaining[lastWinner]++;
            remaining[i]++;
            
            // Try i wins
            remaining[lastWinner]--;
            remaining[i]--;
            schedule.push_back({i + 1, lastWinner + 1});
            
            if(solve(i)) return true;
            
            schedule.pop_back();
            remaining[lastWinner]++;
            remaining[i]++;
        }
    }
    
    return false;
}

int main(){
    std::cin >> n;
    games.resize(n);
    remaining.resize(n);
    
    int total = 0;
    for(int i = 0; i < n; i++) {
        std::cin >> games[i];
        remaining[i] = games[i];
        total += games[i];
    }
    
    // Try starting with each pair
    for(int i = 0; i < n && schedule.empty(); i++) {
        for(int j = i + 1; j < n && schedule.empty(); j++) {
            remaining = games;
            schedule.clear();
            
            // Try i wins first game
            remaining[i]--;
            remaining[j]--;
            schedule.push_back({i + 1, j + 1});
            if(solve(i)) break;
            schedule.pop_back();
            
            // Try j wins first game
            remaining[i]--;
            remaining[j]--;
            schedule.push_back({j + 1, i + 1});
            if(solve(j)) break;
            schedule.pop_back();
            
            schedule.clear();
        }
    }
    
    std::cout << schedule.size() << std::endl;
    for(auto& p : schedule) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    
    return 0;
}
