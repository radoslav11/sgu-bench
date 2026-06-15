#include<bits/stdc++.h>
using namespace std;

int main(){
    long long A, B;
    cin >> A >> B;
    
    long long total = A + B;
    
    // Check if total is a power of 2
    if((total & (total - 1)) != 0){
        cout << -1 << endl;
        return 0;
    }
    
    int moves = 0;
    
    while(A != 0 && B != 0){
        if(A > B){
            A -= B;
            B += B;
        } else {
            B -= A;
            A += A;
        }
        moves++;
    }
    
    cout << moves << endl;
    return 0;
}
