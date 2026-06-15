#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    int half = n/2;
    
    // Outer ring: top half values, arranged alternating high-low
    // Inner ring: bottom half values, arranged alternating low-high (opposite phase)
    
    vector<int> outer_vals, inner_vals;
    for(int i = half+1; i <= n; i++) outer_vals.push_back(i);
    for(int i = 1; i <= half; i++) inner_vals.push_back(i);
    
    // Sort outer descending, inner ascending
    sort(outer_vals.rbegin(), outer_vals.rend());
    sort(inner_vals.begin(), inner_vals.end());
    
    // Arrange outer alternating: pick from high end and low end alternately
    vector<int> outer_arr(half), inner_arr(half);
    int lo = 0, hi = half-1;
    for(int i = 0; i < half; i++){
        if(i%2==0) outer_arr[i] = outer_vals[lo++];
        else outer_arr[i] = outer_vals[hi--];
    }
    lo = 0; hi = half-1;
    for(int i = 0; i < half; i++){
        if(i%2==0) inner_arr[i] = inner_vals[lo++];
        else inner_arr[i] = inner_vals[hi--];
    }
    
    for(int i = 0; i < half; i++){
        if(i > 0) cout << " ";
        cout << outer_arr[i] << " " << inner_arr[i];
    }
    cout << endl;
    return 0;
}
