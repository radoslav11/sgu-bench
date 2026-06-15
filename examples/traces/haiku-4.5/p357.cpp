#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> buttons(13);
    
    // Read button states
    for(int i = 0; i < 4; i++){
        if(i == 0){
            cin >> buttons[0] >> buttons[1] >> buttons[2] >> buttons[11]; // 1,2,3,↑
        } else if(i == 1){
            cin >> buttons[3] >> buttons[4] >> buttons[5] >> buttons[12]; // 4,5,6,↓
        } else if(i == 2){
            cin >> buttons[6] >> buttons[7] >> buttons[8]; // 7,8,9
        } else {
            cin >> buttons[9] >> buttons[10]; // --,0
        }
    }
    
    int X, Y;
    cin >> X >> Y;
    
    // BFS
    vector<int> dist(100, -1);
    queue<int> q;
    q.push(X);
    dist[X] = 0;
    
    while(!q.empty()){
        int ch = q.front();
        q.pop();
        
        if(ch == Y){
            cout << dist[Y] << endl;
            return 0;
        }
        
        // Try pressing digit buttons 0-9
        for(int digit = 0; digit <= 9; digit++){
            if(buttons[digit] == 1){
                int next_ch = digit;
                if(dist[next_ch] == -1){
                    dist[next_ch] = dist[ch] + 1;
                    q.push(next_ch);
                }
            }
        }
        
        // Try pressing -- followed by two digits (for channels 10-99)
        if(buttons[9] == 1){ // -- button
            for(int d1 = 0; d1 <= 9; d1++){
                for(int d2 = 0; d2 <= 9; d2++){
                    if(buttons[d1] == 1 && buttons[d2] == 1){
                        int next_ch = d1 * 10 + d2;
                        if(next_ch >= 10 && dist[next_ch] == -1){
                            dist[next_ch] = dist[ch] + 3;
                            q.push(next_ch);
                        }
                    }
                }
            }
        }
        
        // Try pressing ↑ (increment)
        if(buttons[11] == 1){
            int next_ch = (ch + 1) % 100;
            if(dist[next_ch] == -1){
                dist[next_ch] = dist[ch] + 1;
                q.push(next_ch);
            }
        }
        
        // Try pressing ↓ (decrement)
        if(buttons[12] == 1){
            int next_ch = (ch - 1 + 100) % 100;
            if(dist[next_ch] == -1){
                dist[next_ch] = dist[ch] + 1;
                q.push(next_ch);
            }
        }
    }
    
    cout << -1 << endl;
    return 0;
}
