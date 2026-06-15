#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<char> type(n+1);
    vector<int> val(n+1);
    for(int i = 2; i <= n; i++){
        cin >> type[i] >> val[i];
    }
    
    // min-heap of killed: (gold, cell)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> killed;
    // max-heap of unkilled dragons seen so far
    priority_queue<pair<int,int>> unkilled;
    
    for(int i = 2; i < n; i++){
        if(type[i] == 'd'){
            killed.push({val[i], i});
        } else {
            int b = val[i];
            while((int)killed.size() >= b){
                auto mn = killed.top(); killed.pop();
                unkilled.push(mn);
            }
        }
    }
    int bn = val[n];
    while((int)killed.size() < bn && !unkilled.empty()){
        killed.push(unkilled.top()); unkilled.pop();
    }
    if((int)killed.size() < bn){ cout << -1; return 0; }
    long long gold = 0;
    vector<int> cells;
    while(!killed.empty()){ auto [g,c]=killed.top();killed.pop();gold+=g;cells.push_back(c);}
    sort(cells.begin(),cells.end());
    cout<<gold<<"\n"<<cells.size()<<"\n";
    for(int c:cells) cout<<c<<" ";
}
