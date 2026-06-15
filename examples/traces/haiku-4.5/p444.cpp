#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long x, y;
    cin >> x >> y;
    
    map<long long, long long> remainderPos;
    
    long long remainder = x % y;
    long long position = 0;
    
    if(remainder == 0){
        cout << "0 0" << endl;
        return 0;
    }
    
    while(remainder != 0){
        if(remainderPos.find(remainder) != remainderPos.end()){
            long long cycleStart = remainderPos[remainder];
            long long cycleLength = position - cycleStart;
            cout << cycleStart << " " << cycleLength << endl;
            return 0;
        }
        
        remainderPos[remainder] = position;
        remainder = (remainder * 10) % y;
        position++;
    }
    
    cout << position << " 0" << endl;
    
    return 0;
}
