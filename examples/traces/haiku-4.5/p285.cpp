#include<bits/stdc++.h>
using namespace std;

int N;
vector<double> probs;
map<pair<pair<int,int>, set<int>>, vector<double>> memo;

vector<double> solve(int club_pts, int viewer_pts, set<int> available) {
    vector<double> result(12, 0.0);
    
    if(club_pts == 6) {
        result[viewer_pts] = 1.0;
        return result;
    }
    if(viewer_pts == 6) {
        result[6 + (5 - club_pts)] = 1.0;
        return result;
    }
    
    auto key = make_pair(make_pair(club_pts, viewer_pts), available);
    if(memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    for(int start = 1; start <= 13; start++) {
        int sector = start;
        while(available.find(sector) == available.end()) {
            sector++;
            if(sector > 13) sector = 1;
        }
        
        set<int> next_available = available;
        next_available.erase(sector);
        
        double prob_answer;
        if(sector <= 12) {
            prob_answer = probs[sector - 1];
        } else {
            prob_answer = 0;
            for(int i = 0; i < N; i++) {
                prob_answer += probs[12 + i];
            }
            prob_answer /= N;
        }
        
        vector<double> club_wins = solve(club_pts + 1, viewer_pts, next_available);
        vector<double> viewer_wins = solve(club_pts, viewer_pts + 1, next_available);
        
        for(int i = 0; i < 12; i++) {
            result[i] += (1.0/13.0) * (prob_answer * club_wins[i] + (1.0 - prob_answer) * viewer_wins[i]);
        }
    }
    
    memo[key] = result;
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    probs.resize(12 + N);
    
    for(int i = 0; i < 12 + N; i++) {
        cin >> probs[i];
    }
    
    set<int> available;
    for(int i = 1; i <= 13; i++) {
        available.insert(i);
    }
    
    vector<double> result = solve(0, 0, available);
    
    for(int i = 0; i < 12; i++) {
        cout << fixed << setprecision(3) << result[i] << "\n";
    }
    
    return 0;
}
