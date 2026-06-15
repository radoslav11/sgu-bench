#include <bits/stdc++.h>
using namespace std;

int n;
int adj[20];
long long pop[20];

int neighOf(int set){
    int r=0;
    for(int i=0;i<n;i++) if(set&(1<<i)) r|=adj[i];
    return r & ~set;
}
long long popOf(int set){
    long long s=0;
    for(int i=0;i<n;i++) if(set&(1<<i)) s+=pop[i];
    return s;
}

int main(){
    int s,t;
    cin>>n>>s>>t; s--; t--;
    for(int i=0;i<n;i++){
        string row; cin>>row;
        adj[i]=0;
        for(int j=0;j<n;j++) if(row[j]=='1') adj[i]|=(1<<j);
    }
    for(int i=0;i<n;i++) cin>>pop[i];
    
    int A=1<<s, B=1<<t;
    int full=(1<<n)-1;
    
    while(true){
        bool changed=false;
        // A's turn
        {
            int free = full & ~A & ~B;
            int nA = neighOf(A);
            int nB = neighOf(B);
            long long pA = popOf(A), pB = popOf(B);
            int newA=0;
            for(int c=0;c<n;c++){
                int bit=1<<c;
                if(!(free&bit)) continue;
                if(!(nA&bit)) continue;
                bool contested = (nB&bit)!=0;
                if(!contested || pA>=pB) newA|=bit;
            }
            if(newA){ A|=newA; changed=true; }
        }
        // B's turn
        {
            int free = full & ~A & ~B;
            int nA = neighOf(A);
            int nB = neighOf(B);
            long long pA = popOf(A), pB = popOf(B);
            int newB=0;
            for(int c=0;c<n;c++){
                int bit=1<<c;
                if(!(free&bit)) continue;
                if(!(nB&bit)) continue;
                bool contested = (nA&bit)!=0;
                if(!contested || pB>=pA) newB|=bit;
            }
            if(newB){ B|=newB; changed=true; }
        }
        if(!changed) break;
    }
    
    cout << (popOf(A)-popOf(B)) << endl;
    return 0;
}
