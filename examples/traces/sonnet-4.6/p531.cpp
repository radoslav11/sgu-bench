#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, d;
    cin >> n >> d;
    
    vector<long long> x(n), w(n);
    for(int i = 0; i < n; i++){
        cin >> x[i] >> w[i];
    }
    
    // Banks already sorted by x
    // prefix max from left
    vector<long long> prefMax(n);
    vector<int> prefMaxIdx(n);
    prefMax[0] = w[0];
    prefMaxIdx[0] = 0;
    for(int i = 1; i < n; i++){
        if(w[i] > prefMax[i-1]){
            prefMax[i] = w[i];
            prefMaxIdx[i] = i;
        } else {
            prefMax[i] = prefMax[i-1];
            prefMaxIdx[i] = prefMaxIdx[i-1];
        }
    }
    
    // suffix max from right
    vector<long long> sufMax(n);
    vector<int> sufMaxIdx(n);
    sufMax[n-1] = w[n-1];
    sufMaxIdx[n-1] = n-1;
    for(int i = n-2; i >= 0; i--){
        if(w[i] > sufMax[i+1]){
            sufMax[i] = w[i];
            sufMaxIdx[i] = i;
        } else {
            sufMax[i] = sufMax[i+1];
            sufMaxIdx[i] = sufMaxIdx[i+1];
        }
    }
    
    long long bestVal = -1;
    int ans1 = -1, ans2 = -1;
    
    // For each j, find rightmost i with x[i] <= x[j] - d (i < j)
    // Use binary search
    for(int j = 0; j < n; j++){
        long long threshold = x[j] - d;
        // find largest i with x[i] <= threshold, i < j
        // binary search in [0, j-1]
        if(j == 0) continue;
        int lo = 0, hi = j-1, p = -1;
        while(lo <= hi){
            int mid = (lo+hi)/2;
            if(x[mid] <= threshold){
                p = mid;
                lo = mid+1;
            } else {
                hi = mid-1;
            }
        }
        if(p >= 0){
            long long val = prefMax[p] + w[j];
            if(val > bestVal){
                bestVal = val;
                ans1 = prefMaxIdx[p] + 1;
                ans2 = j + 1;
            }
        }
        
        // For each i, find leftmost j with x[j] >= x[i] + d (j > i)
        // This is covered by the above loop already (symmetric)
    }
    
    // Also check from the other side: for each i, find smallest j with x[j] >= x[i]+d
    // Actually the above loop covers all pairs (i,j) with i<j and x[j]-x[i]>=d
    // since for each j we look for valid i's to the left.
    
    if(ans1 == -1){
        cout << "-1 -1\n";
    } else {
        cout << ans1 << " " << ans2 << "\n";
    }
    
    return 0;
}
