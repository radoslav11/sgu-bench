#include <bits/stdc++.h>
using namespace std;

int main(){
    double coverage[] = {0, 78.5398, 98.8, 100};
    
    int P;
    int caseNum = 1;
    
    while(cin >> P) {
        int shots;
        if(P <= 78.5398) shots = 1;
        else if(P <= 98.8) shots = 2;
        else shots = 3;
        
        cout << "Case #" << caseNum << ": " << shots << endl;
        caseNum++;
    }
    
    return 0;
}
