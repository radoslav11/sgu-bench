#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<long long> dev(n);
    for(int i = 0; i < n; i++){
        double h; cin >> h;
        long long hum = llround(h * 1e6); // micrometers
        dev[i] = hum - 2000000LL;
    }
    // sort indices by deviation
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b){ return dev[a] < dev[b]; });
    // greedy: two pointers
    int lo = 0, hi = n-1;
    long long sum = 0;
    vector<int> order;
    while(lo <= hi){
        if(sum >= 0){ order.push_back(idx[lo++]); }
        else { order.push_back(idx[hi--]); }
        sum += dev[order.back()];
    }
    cout << "yes\n";
    for(int i = 0; i < n; i++) cout << order[i]+1 << " \n"[i==n-1];
}
