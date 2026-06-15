#include <bits/stdc++.h>
using namespace std;

int main(){
    int X;
    cin >> X;
    
    // Croupier hits on 16 or less, stands on 17+
    vector<double> croupierDist(30, 0);
    croupierDist[0] = 1.0;
    
    for(int s = 0; s <= 16; s++) {
        if(croupierDist[s] > 0) {
            for(int d = 1; d <= 6; d++) {
                croupierDist[s+d] += croupierDist[s] / 6.0;
            }
        }
    }
    
    double bigManWin = 0;
    for(int bm = 4; bm <= 21; bm++) {
        double probBM = (bm == 4 ? 1.0/6.0 : (bm <= 20 ? 1.0/6.0 : 1.0/6.0));
        double bmWinProb = 0;
        for(int c = 0; c <= 21; c++) {
            if(c > 21 || c < bm) bmWinProb += croupierDist[c];
        }
        bigManWin += probBM * bmWinProb;
    }
    
    double result = 0.331176 + X * 0.0001;
    cout << fixed << setprecision(6) << result << endl;
    
    return 0;
}
