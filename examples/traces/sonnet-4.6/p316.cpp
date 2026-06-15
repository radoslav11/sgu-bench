#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<int> hp(N+1, 100);
    vector<int> score(N+1, 0);
    
    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        
        // a shoots b
        if (hp[b] > 0) {
            score[a] += 3;
        }
        hp[b] -= 8;
    }
    
    for (int i = 1; i <= N; i++) {
        if (hp[i] > 0) {
            score[i] += hp[i] / 2;
        }
        cout << hp[i] << " " << score[i] << "\n";
    }
    
    return 0;
}
