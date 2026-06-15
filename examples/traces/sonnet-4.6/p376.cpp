#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N,K; cin>>N>>K;
    vector<long long>W(N),R(N),P(N);
    for(int i=0;i<N;i++) cin>>W[i]>>R[i]>>P[i];
    vector<long long>A(K),B(N);
    for(int i=0;i<K;i++) cin>>A[i];
    for(int j=0;j<N;j++) cin>>B[j];
    vector<long long>cur(N);
    for(int i=0;i<N;i++) cur[i]=20000000LL-W[i];
    for(int s=0;s<K;s++){
        vector<long long>hr(N);
        for(int j=0;j<N;j++) hr[j]=3LL*A[s]*A[s]+5LL*A[s]*B[j]+2LL*B[j]*B[j];
        sort(hr.begin(),hr.end(),greater<long long>());
        vector<int>ord(N); iota(ord.begin(),ord.end(),0);
        sort(ord.begin(),ord.end(),[&](int a,int b){
            if(cur[a]!=cur[b])return cur[a]<cur[b];
            return P[a]>P[b];
        });
        vector<long long>nt(N);
        for(int r=0;r<N;r++){int idx=ord[r];nt[idx]=cur[idx]+(20000000LL-R[idx]-hr[r]);}
        cur=nt;
    }
    vector<int>fo(N);iota(fo.begin(),fo.end(),0);
    sort(fo.begin(),fo.end(),[&](int a,int b){
        if(cur[a]!=cur[b])return cur[a]<cur[b];
        return P[a]>P[b];
    });
    for(int i=0;i<N;i++){if(i)cout<<' ';cout<<fo[i]+1;}
    cout<<endl;
}
