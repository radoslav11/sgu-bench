#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    
    while(T--) {
        int N;
        cin >> N;
        vector<long long> a(N);
        for(int i = 0; i < N; i++) {
            cin >> a[i];
        }
        
        long long maxHill = 0, maxDale = 0;
        
        // Check for hills and dales
        for(int j = 1; j < N-1; j++) {
            // Check if j is a peak (potential hill peak)
            if(a[j] > a[j-1] && a[j] > a[j+1]) {
                maxHill = max(maxHill, a[j] - min(a[j-1], a[j+1]));
            }
            // Check if j is a valley (potential dale valley)
            if(a[j] < a[j-1] && a[j] < a[j+1]) {
                maxDale = max(maxDale, max(a[j-1], a[j+1]) - a[j]);
            }
        }
        
        cout << maxHill << " " << maxDale << "\n";
    }
    
    return 0;
}
