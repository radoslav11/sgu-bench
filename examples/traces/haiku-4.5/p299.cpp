#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> segments(n);
    for(int i = 0; i < n; i++){
        cin >> segments[i];
    }
    
    // Sort the segments
    sort(segments.begin(), segments.end());
    
    // Check consecutive triplets
    // If segments are sorted as a <= b <= c, we only need to check a + b > c
    for(int i = 0; i < n - 2; i++){
        long long a = segments[i];
        long long b = segments[i + 1];
        long long c = segments[i + 2];
        
        if(a + b > c){
            cout << a << " " << b << " " << c << endl;
            return 0;
        }
    }
    
    cout << "0 0 0" << endl;
    return 0;
}
