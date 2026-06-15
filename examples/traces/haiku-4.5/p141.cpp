#include<bits/stdc++.h>
using namespace std;

int main(){
    long long x1, x2, P, K;
    cin >> x1 >> x2 >> P >> K;
    
    // Try all reasonable values of a
    for(long long a = -80000; a <= 80000; a++){
        long long remainder = P - a * x1;
        if(remainder % x2 != 0) continue;
        
        long long b = remainder / x2;
        
        long long min_j1 = abs(a);
        long long min_j2 = abs(b);
        
        if(min_j1 + min_j2 > K) continue;
        
        long long j1 = min_j1;
        if((j1 + a) % 2 != 0) j1++;
        
        long long j2 = K - j1;
        
        if(j2 < min_j2 || (j2 + b) % 2 != 0){
            if(j2 >= min_j2 + 1 && (j2 - 1 + b) % 2 == 0){
                j1 += 2; j2 -= 2;
            } else continue;
        }
        
        if(j2 < 0) continue;
        
        long long P1 = (j1 + a) / 2;
        long long N1 = (j1 - a) / 2;
        long long P2 = (j2 + b) / 2;
        long long N2 = (j2 - b) / 2;
        
        if(P1 >= 0 && N1 >= 0 && P2 >= 0 && N2 >= 0){
            cout << "YES\n";
            cout << P1 << " " << N1 << " " << P2 << " " << N2 << "\n";
            return 0;
        }
    }
    
    cout << "NO\n";
    return 0;
}
