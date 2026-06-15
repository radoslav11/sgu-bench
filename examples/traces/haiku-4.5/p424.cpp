#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    if(n == 1) {
        cout << "1 0\n";
        return 0;
    }
    
    if(n == 2) {
        cout << "2 1\n";
        cout << "1 2\n";
        return 0;
    }
    
    // For n >= 3, create a cycle of all n vertices
    // A cycle is a cactus (beautiful graph)
    cout << n << " " << n << "\n";
    
    for(int i = 1; i < n; i++) {
        cout << i << " " << (i + 1) << "\n";
    }
    cout << n << " " << 1 << "\n";
    
    return 0;
}
