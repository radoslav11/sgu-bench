#include <bits/stdc++.h>
using namespace std;

int P;
int S;
typedef vector<vector<long long>> Mat;

Mat mul(const Mat&a, const Mat&b){
    int n=a.size();
    Mat c(n, vector<long long>(n,0));
    for(int i=0;i<n;i++)for(int k=0;k<n;k++){
        if(a[i][k]==0)continue;
        long long aik=a[i][k];
        for(int j=0;j<n;j++) c[i][j]=(c[i][j]+aik*b[k][j])%P;
    }
    return c;
}

bool valid(int a,int b,int M){
    for(int i=0;i+1<M;i++){
        int a0=(a>>i)&1, a1=(a>>(i+1))&1;
        int b0=(b>>i)&1, b1=(b>>(i+1))&1;
        if(a0==a1&&a1==b0&&b0==b1) return false;
    }
    return true;
}

int main(){
    string N; int M;
    cin>>N>>M>>P;
    S=1<<M;
    Mat T(S, vector<long long>(S,0));
    for(int a=0;a<S;a++)for(int b=0;b<S;b++)
        if(valid(a,b,M)) T[a][b]=1%P;
    string e=N;
    {
        int i=e.size()-1;
        while(i>=0){ if(e[i]>'0'){e[i]--;break;} else {e[i]='9';i--;} }
        int p=0; while(p+1<(int)e.size()&&e[p]=='0')p++;
        e=e.substr(p);
    }
    Mat result(S, vector<long long>(S,0));
    for(int i=0;i<S;i++)result[i][i]=1%P;
    auto matpow=[&](Mat base,long long ex){
        Mat r(S,vector<long long>(S,0));
        for(int i=0;i<S;i++)r[i][i]=1%P;
        while(ex>0){ if(ex&1)r=mul(r,base); base=mul(base,base); ex>>=1; }
        return r;
    };
    for(char ch: e){
        int d=ch-'0';
        result=matpow(result,10);
        Mat td=matpow(T,d);
        result=mul(result,td);
    }
    long long ans=0;
    for(int a=0;a<S;a++)for(int b=0;b<S;b++)ans=(ans+result[a][b])%P;
    cout<<(ans%P)<<endl;
    return 0;
}
