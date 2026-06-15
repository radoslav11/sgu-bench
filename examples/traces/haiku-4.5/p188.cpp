#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, T;
    cin >> N >> T;
    
    vector<int> L(N), V(N);
    for(int i = 0; i < N; i++) cin >> L[i];
    for(int i = 0; i < N; i++) cin >> V[i];
    
    vector<int> count(N, 0);
    
    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            if((V[i] > 0 && V[j] > 0) || (V[i] < 0 && V[j] < 0)) continue;
            
            for(int t = 1; t <= T * 1000; t++) {
                double time = t / 1000.0;
                double pos_i = fmod(L[i] + V[i] * time, 1000.0);
                if(pos_i < 0) pos_i += 1000;
                double pos_j = fmod(L[j] + V[j] * time, 1000.0);
                if(pos_j < 0) pos_j += 1000;
                
                if(fabs(pos_i - pos_j) < 0.001) {
                    count[i]++;
                    count[j]++;
                    t += 10;
                }
            }
        }
    }
    
    for(int i = 0; i < N; i++) {
        if(i > 0) cout << " ";
        cout << count[i];
    }
    cout << "\n";
    
    return 0;
}
