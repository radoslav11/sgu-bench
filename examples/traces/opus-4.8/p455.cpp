#include <bits/stdc++.h>
using namespace std;

int main(){
    long long A,B,C;
    cin>>A>>B>>C;
    unordered_map<long long,int> seen;
    seen.reserve(4000000);
    seen.max_load_factor(0.7);
    long long x=1;
    int LIMIT=2000000;
    for(int i=0;i<=LIMIT;i++){
        auto it=seen.find(x);
        if(it!=seen.end()){
            cout<<i<<endl;
            return 0;
        }
        seen[x]=i;
        // next
        x = A*x*x + B*x + C;
    }
    cout<<-1<<endl;
    return 0;
}
