#include <bits/stdc++.h>
using default_type = double;
using namespace std;
int main(){
    int N,A,B;
    scanf("%d%d%d",&N,&A,&B);
    // deduplicate by (a,b), keep max c
    map<pair<int,int>,int> best;
    for(int i=0;i<N;i++){
        int a,b,c; scanf("%d%d%d",&a,&b,&c);
        auto key=make_pair(a,b);
        best[key]=max(best[key],c);
    }
    vector<tuple<int,int,int>> fuels;
    for(auto&p:best) fuels.push_back({p.first.first,p.first.second,p.second});
    int n=fuels.size();
    double ans=0;
    for(int i=0;i<n;i++){
        auto[ai,bi,ci]=fuels[i];
        // use only fuel i
        double m=1e18;
        if(ai>0) m=min(m,(double)A/ai);
        if(bi>0) m=min(m,(double)B/bi);
        if(ai==0&&bi==0) m=0; // can't get intensity if both 0? c might be 0 too
        ans=max(ans,m*ci);
        for(int j=i+1;j<n;j++){
            auto[aj,bj,cj]=fuels[j];
            double det=(double)ai*bj-(double)aj*bi;
            if(abs(det)>1e-9){
                double mi=((double)A*bj-(double)aj*B)/det;
                double mj=((double)ai*B-(double)A*bi)/det;
                if(mi>=0&&mj>=0) ans=max(ans,mi*ci+mj*cj);
            }
        }
    }
    printf("%.6f\n",ans);
}
