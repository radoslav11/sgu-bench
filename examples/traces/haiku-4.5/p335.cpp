#include<bits/stdc++.h>
using cmp = long long;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long H, W;
    long long Rc, Cc, Rt, Ct;
    char first;
    
    cin >> H >> W >> Rc >> Cc >> Rt >> Ct >> first;
    
    long long dist = abs(Rc - Rt) + abs(Cc - Ct);
    
    bool cop_catches = false;
    
    if(first == 'C'){
        // Cop moves first
        // Cop catches if distance is even
        if(dist % 2 == 0){
            cop_catches = true;
        }
    } else {
        // Thief moves first
        // Cop catches if distance is odd
        if(dist % 2 == 1){
            cop_catches = true;
        }
    }
    
    if(!cop_catches){
        cout << 0 << endl;
    } else {
        cout << dist << endl;
    }
    
    return 0;
}
