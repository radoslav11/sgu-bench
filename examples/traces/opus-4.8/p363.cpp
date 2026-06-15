#include <bits/stdc++.h>
using namespace std;

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    vector<vector<int>> leaves(m+1);
    vector<int> lastLeave(m+1,0);
    for(int i=0;i<n;i++){
        int k,t;
        scanf("%d %d",&k,&t);
        int best=-1, bestCount=INT_MAX;
        for(int w=1;w<=m;w++){
            int cnt=0;
            for(int x:leaves[w]) if(x>k) cnt++;
            if(cnt<bestCount){bestCount=cnt;best=w;}
        }
        int start=max(k,lastLeave[best]);
        int lv=start+t;
        leaves[best].push_back(lv);
        lastLeave[best]=lv;
        printf("%d %d\n",best,lv);
    }
    return 0;
}
