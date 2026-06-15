#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    vector<int> S(n),B(n),idx(n);
    for(int i=0;i<n;i++){scanf("%d %d",&S[i],&B[i]);idx[i]=i;}
    // sort by S asc, B desc
    sort(idx.begin(),idx.end(),[&](int a,int b){
        if(S[a]!=S[b])return S[a]<S[b];
        return B[a]>B[b];
    });
    // strictly increasing LIS on B
    vector<int> tails; // stores B values
    vector<int> tailsIdx; // index in idx order position
    vector<int> prevPos(n,-1); // predecessor index (original idx)
    // We'll keep tails as positions in idx array
    vector<int> tail; // B value for binary search
    vector<int> tailPos; // position (original member index) at each length
    
    for(int k=0;k<n;k++){
        int i=idx[k];
        int b=B[i];
        // find first element in tail >= b (strictly increasing -> lower_bound)
        int lo=lower_bound(tail.begin(),tail.end(),b)-tail.begin();
        if(lo==(int)tail.size()){
            tail.push_back(b);
            tailPos.push_back(i);
        } else {
            tail[lo]=b;
            tailPos[lo]=i;
        }
        if(lo>0) prevPos[i]=tailPos[lo-1];
        else prevPos[i]=-1;
    }
    
    int len=tail.size();
    printf("%d\n",len);
    // reconstruct from tailPos[len-1]
    vector<int> res;
    int cur=tailPos[len-1];
    while(cur!=-1){
        res.push_back(cur+1);
        cur=prevPos[cur];
    }
    for(int i=res.size()-1;i>=0;i--){
        printf("%d%c",res[i],i==0?'\n':' ');
    }
    return 0;
}
