#include <bits/stdc++.h>
using namespace std;

int main(){
    int K;
    cin>>K;
    while(K--){
        int N,M;
        cin>>N>>M;
        vector<int> moves = {1};
        for(int i=0;i<M;i++){int p;cin>>p;moves.push_back(p);}
        sort(moves.begin(),moves.end());
        moves.erase(unique(moves.begin(),moves.end()),moves.end());
        
        int LIMIT = 20000;
        vector<int> pos(LIMIT+1,0); // 0=L,1=W
        pos[0]=1; // W: opponent took last match
        for(int i=1;i<=LIMIT;i++){
            for(int mv:moves){
                if(mv<=i && pos[i-mv]==0){pos[i]=1;break;}
            }
        }
        // find period
        int period=-1;
        for(int p=1;p<=1000;p++){
            bool ok=true;
            for(int j=10000;j<=10000+p*10&&j+p<=LIMIT;j++){
                if(pos[j]!=pos[j+p]){ok=false;break;}
            }
            if(ok){period=p;break;}
        }
        int ans;
        if(N<=LIMIT) ans=pos[N];
        else ans=pos[10000+(N-10000)%period];
        cout<<(ans?"FIRST PLAYER MUST WIN":"SECOND PLAYER MUST WIN")<<"\n";
    }
}
