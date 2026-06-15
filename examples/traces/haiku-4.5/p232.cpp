#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    string D;
    cin >> N >> K >> D;
    
    int maxIdx = 0;
    string maxSequence = "";
    
    // Try each starting position
    for(int start = 0; start < N; start++){
        // Generate sequence starting from position 'start'
        string sequence = "";
        for(int j = 0; j < 2*N; j++){
            int pos = (start + (long long)j * K) % N;
            sequence += D[pos];
        }
        
        // Compare with current maximum
        if(start == 0){
            maxSequence = sequence;
            maxIdx = start;
        } else {
            // Compare lexicographically (works for comparing decimal fractions)
            if(sequence > maxSequence){
                maxSequence = sequence;
                maxIdx = start;
            }
        }
    }
    
    // Output first N digits
    cout << maxSequence.substr(0, N) << endl;
    
    return 0;
}
