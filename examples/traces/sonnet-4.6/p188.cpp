#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, T;
    cin >> N >> T;
    vector<int> L(N), V(N);
    for(int i=0;i<N;i++) cin>>L[i];
    for(int i=0;i<N;i++) cin>>V[i];
    
    vector<int> B(N,0);
    
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            if((V[i]>0&&V[j]<0)||(V[i]<0&&V[j]>0)){
                int dL = L[i]-L[j];
                int dV = V[i]-V[j];
                // t = (-dL + 1000*k)/dV in [0,T]
                // Find k range
                for(int k=-1100;k<=1100;k++){
                    double num = -dL + 1000.0*k;
                    if(dV==0) continue;
                    double t = num/dV;
                    if(t >= -1e-9 && t <= T+1e-9){
                        B[i]++;
                        B[j]++;
                    }
                }
            }
        }
    }
    
    for(int i=0;i<N;i++){
        if(i) cout<<" ";
        cout<<B[i];
    }
    cout<<endl;
}
