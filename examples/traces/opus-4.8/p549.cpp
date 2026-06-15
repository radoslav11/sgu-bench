#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,k;
    scanf("%d %d",&n,&k);
    vector<vector<int>> byMass(4001);
    for(int i=0;i<n;i++){
        int m,c; scanf("%d %d",&m,&c);
        byMass[m].push_back(c);
    }
    vector<vector<long long>> pref;
    for(int m=1;m<=4000;m++){
        if(byMass[m].empty()) continue;
        sort(byMass[m].rbegin(), byMass[m].rend());
        vector<long long> p(byMass[m].size()+1,0);
        for(size_t j=0;j<byMass[m].size();j++) p[j+1]=p[j]+byMass[m][j];
        pref.push_back(move(p));
    }
    int M=pref.size();
    int bestT=0;
    int upper = n/k;
    for(int t=upper;t>=1;t--){
        int cnt=0;
        for(int i=0;i<M;i++) if((int)pref[i].size()-1>=t) cnt++;
        if(cnt>=k){ bestT=t; break;}
    }
    if(bestT==0){ printf("0 0\n"); return 0;}
    int t=bestT;
    vector<long long> sums;
    for(int i=0;i<M;i++){
        if((int)pref[i].size()-1>=t) sums.push_back(pref[i][t]);
    }
    sort(sums.rbegin(), sums.rend());
    long long total=0;
    for(int i=0;i<k;i++) total+=sums[i];
    printf("%d %lld\n", t, total);
    return 0;
}
