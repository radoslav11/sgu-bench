#include <bits/stdc++.h>
using namespace std;
int main(){
    int N, M;
    cin >> N >> M;
    string S1, S2;
    cin >> S1 >> S2;
    // Find lex smallest rotation of S2
    string S2S2 = S2 + S2;
    int best_rot = 0;
    for(int i = 1; i < M; i++){
        if(S2S2.substr(i, M) < S2S2.substr(best_rot, M)) best_rot = i;
    }
    string T2 = S2S2.substr(best_rot, M);
    // For each starting position p in S1, count mismatches
    string S1S1 = S1 + S1;
    int best_p = -1, best_cost = INT_MAX;
    bool best_left = false;
    for(int p = 0; p < N; p++){
        int mis = 0;
        for(int j = 0; j < M && mis <= 1; j++) if(S1S1[p+j] != T2[j]) mis++;
        if(mis <= 1){
            int cost = min(p, N-p);
            bool is_left = (p <= N-p);
            if(cost < best_cost || (cost == best_cost && is_left && !best_left)){
                best_cost = cost; best_p = p; best_left = is_left;
            }
        }
    }
    cout << T2 << "\n" << S1S1.substr(best_p, N) << "\n";
}
