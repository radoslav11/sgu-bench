#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
lll gcdll(lll a, lll b){ if(a<0)a=-a; if(b<0)b=-b; while(b){lll t=a%b;a=b;b=t;} return a;}
struct Fr{
    lll n,d;
    Fr(lll nn=0,lll dd=1){ if(dd<0){nn=-nn;dd=-dd;} lll g=gcdll(nn,dd); if(g==0)g=1; n=nn/g; d=dd/g;}
};
Fr operator+(Fr a,Fr b){return Fr(a.n*b.d+b.n*a.d,a.d*b.d);}
Fr operator-(Fr a,Fr b){return Fr(a.n*b.d-b.n*a.d,a.d*b.d);}
Fr operator*(Fr a,Fr b){return Fr(a.n*b.n,a.d*b.d);}
Fr operator/(Fr a,Fr b){return Fr(a.n*b.d,a.d*b.n);}
bool operator<(Fr a,Fr b){return a.n*b.d<b.n*a.d;}
bool operator>(Fr a,Fr b){return a.n*b.d>b.n*a.d;}
bool operator==(Fr a,Fr b){return a.n*b.d==b.n*a.d;}
int main(){
    int N; int cs=0;
    while(cin>>N){
        cs++;
        int n=N;
        vector<vector<Fr>> A(n, vector<Fr>(n));
        for(int a=0;a<n;a++)for(int b=0;b<n;b++){
            int val=(abs(a-b)==1)?1:-1;
            A[a][b]=Fr(val+2);
        }
        int m=n,vars=n;
        vector<vector<Fr>> T(m, vector<Fr>(vars+m+1));
        for(int i=0;i<m;i++){
            for(int j=0;j<vars;j++)T[i][j]=A[i][j];
            T[i][vars+i]=Fr(1);
            T[i][vars+m]=Fr(1);
        }
        vector<Fr> obj(vars+m+1,Fr(0));
        for(int j=0;j<vars;j++)obj[j]=Fr(1);
        vector<int> basis(m); for(int i=0;i<m;i++)basis[i]=vars+i;
        while(true){
            int pc=-1;
            for(int j=0;j<vars+m;j++) if(obj[j]>Fr(0)){pc=j;break;}
            if(pc<0)break;
            int pr=-1; Fr best;
            for(int i=0;i<m;i++) if(T[i][pc]>Fr(0)){
                Fr r=T[i][vars+m]/T[i][pc];
                if(pr<0||r<best){best=r;pr=i;}
            }
            if(pr<0)break;
            Fr piv=T[pr][pc];
            for(int j=0;j<=vars+m;j++)T[pr][j]=T[pr][j]/piv;
            for(int i=0;i<m;i++) if(i!=pr&&!(T[i][pc]==Fr(0))){
                Fr f=T[i][pc];
                for(int j=0;j<=vars+m;j++)T[i][j]=T[i][j]-f*T[pr][j];
            }
            Fr f=obj[pc];
            for(int j=0;j<=vars+m;j++)obj[j]=obj[j]-f*T[pr][j];
            basis[pr]=pc;
        }
        Fr opt = Fr(0)-obj[vars+m];
        Fr value = Fr(1)/opt;
        Fr ans = value - Fr(2);
        long long nn=(long long)ans.n, dd=(long long)ans.d;
        if(dd<0){dd=-dd;nn=-nn;}
        cout<<"Case #"<<cs<<": "<<nn<<"/"<<dd<<"\n";
    }
}
