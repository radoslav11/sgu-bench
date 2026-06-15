#include <bits/stdc++.h>
using namespace std;
int main(){
    int K; cin>>K;
    string s; cin>>s;
    int n=s.size();
    string t=s+s;
    int N=2*n;
    // For each pair (i,j), precompute prefix mismatch along diagonals
    // diff[i][j] = (t[i]!=t[j])
    // For start a, length L: sum of t[a+p]!=t[a+L+p] for p=0..L-1
    // = sum over diagonal d=L, positions a..a+L-1
    // Precompute psum[d][i] = sum_{p=0}^{i-1} (t[p]!=t[p+d])
    vector<vector<int>> psum(N+1, vector<int>(N+1,0));
    for(int d=1;d<N;d++){
        for(int i=0;i+d<N;i++){
            psum[d][i+1]=psum[d][i]+(t[i]!=t[i+d]);
        }
    }
    string best="";
    int bestlen=-1;
    for(int i=0;i<N;i++){
        for(int L=1;i+2*L<=N && 2*L<=n;L++){// substring t[i..i+2L-1]
            // mismatches: psum[L][i+L]-psum[L][i]
            int mis=psum[L][i+L]-psum[L][i];
            if(mis<=K){
                string cand=t.substr(i,2*L);
                if(2*L>bestlen||(2*L==bestlen&&cand<best)){
                    bestlen=2*L; best=cand;
                }
            }
        }
    }
    cout<<best<<"\n";
}
