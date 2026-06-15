#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string N;
    int K;
    cin >> N >> K;
    
    int len = N.length();
    int keep = len - K;  // number of digits to keep
    
    string result = "";
    int start = 0;  // starting position for current search
    
    for(int i = 0; i < keep; i++){
        int remaining_to_pick = keep - i;  // how many more digits we need
        int end = len - remaining_to_pick;  // last position we can pick from
        
        // Find the maximum digit in range [start, end]
        int max_idx = start;
        for(int j = start; j <= end; j++){
            if(N[j] > N[max_idx]){
                max_idx = j;
            }
        }
        
        result += N[max_idx];
        start = max_idx + 1;  // next search starts after this position
    }
    
    cout << result << endl;
    
    return 0;
}
