#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    while(K--) {
        int N, M;
        cin >> N >> M;
        
        vector<int> moves;
        moves.push_back(1);
        for(int i = 0; i < M; i++) {
            int p;
            cin >> p;
            moves.push_back(p);
        }
        
        sort(moves.begin(), moves.end());
        moves.erase(unique(moves.begin(), moves.end()), moves.end());
        
        int limit = min(N, 200);
        vector<int> g(limit + 1);
        g[0] = 0;
        
        for(int i = 1; i <= limit; i++) {
            set<int> reachable;
            bool canWin = false;
            for(int move : moves) {
                if(move < i) {
                    reachable.insert(g[i - move]);
                    if(g[i - move] == 0) canWin = true;
                }
            }
            g[i] = canWin ? 1 : 0;
        }
        
        int pos = min(N, limit);
        if(g[pos] == 0) {
            cout << "SECOND PLAYER MUST WIN\n";
        } else {
            cout << "FIRST PLAYER MUST WIN\n";
        }
    }
    
    return 0;
}
