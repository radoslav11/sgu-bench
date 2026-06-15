#include<bits/stdc++.h>
using namespace std;

int main(){
    long long N;
    cin >> N;
    
    int k = 0;
    long long power = 1;
    while(power - 1 < N) {
        power *= 2;
        k++;
    }
    
    cout << k - 1 << endl;
    
    return 0;
}
