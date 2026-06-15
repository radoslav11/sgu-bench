#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, K;
    cin >> N >> K;
    
    vector<int> result(N, K/N);
    int extra = K % N;
    
    for(int i = 0; i < extra; i++){
        result[i]++;
    }
    
    for(int i = 0; i < N; i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
