#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    string token;
    while(true){
        // Try to read N
        if(!(cin >> token)) break;
        // skip non-numeric tokens
        while(!isdigit(token[0]) && cin >> token);
        if(!isdigit(token[0])) break;
        N = stoi(token);
        cin >> M;
        
        vector<int> P(M), Q(M);
        for(int i = 0; i < M; i++){
            cin >> P[i] >> Q[i];
        }
        
        for(int j = 1; j <= N; j++){
            int pos = j;
            for(int op = M-1; op >= 0; op--){
                if(pos >= P[op] && pos <= Q[op]){
                    pos = P[op] + Q[op] - pos;
                }
            }
            if(j > 1) cout << ' ';
            cout << pos;
        }
        cout << '\n';
    }
    return 0;
}
