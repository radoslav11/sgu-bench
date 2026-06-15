#include<bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    
    int count = 0;
    
    // Iterate through all pairs (A, B) with A < B
    for(int A = 1; A < N; A++){
        for(int B = A + 1; B <= N; B++){
            // Check if there exist x, y >= 1 such that A*x + B*y = N
            bool found = false;
            
            // Try all possible values of x >= 1
            for(int x = 1; A * x < N; x++){
                int remainder = N - A * x;
                // Check if remainder is divisible by B and gives y >= 1
                if(remainder % B == 0){
                    int y = remainder / B;
                    if(y >= 1){
                        found = true;
                        break;
                    }
                }
            }
            
            if(found){
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
