#include <bits/stdc++.h>
using namespace std;
int main(){
    int K; 
    if(!(cin>>K))return 0;
    while(K--){
        long long N; int M;
        cin>>N>>M;
        vector<int> moves;
        moves.push_back(1);
        for(int i=0;i<M;i++){int p;cin>>p;moves.push_back(p);}
        int LIM=200;
        vector<char> win(LIM,0);
        win[0]=1;
        for(int n=1;n<LIM;n++){
            bool w=false;
            for(int m:moves){ if(m<=n && !win[n-m]){w=true;break;} }
            win[n]=w;
        }
        bool res;
        if(N<LIM) res=win[(int)N];
        else{
            int period=-1;
            for(int p=1;p<=50 && period<0;p++){
                bool ok=true;
                for(int i=100;i+p<LIM;i++){ if(win[i]!=win[i+p]){ok=false;break;} }
                if(ok) period=p;
            }
            if(period<0) period=1;
            int base=100;
            long long idx=base + ((N-base)%period);
            res=win[(int)idx];
        }
        cout<<(res?"FIRST PLAYER MUST WIN":"SECOND PLAYER MUST WIN")<<"\n";
    }
    return 0;
}
