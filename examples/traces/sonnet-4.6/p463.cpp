#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    int b[100][100];
    for(int i=0;i<n;i++){
        string s; cin>>s;
        for(int j=0;j<m;j++) b[i][j]=s[j]-'0';
    }
    string path; cin>>path;
    
    int pass[100][100]={};
    int total=0;
    
    auto passBlock=[&](int i,int j){
        if(i<0||i>=n||j<0||j>=m) return;
        if(pass[i][j]==0) total+=b[i][j];
        else total+=b[i][j]/2;
        pass[i][j]++;
    };
    
    // directions: 0=East,1=South,2=West,3=North
    int dr[]={0,1,0,-1};
    int dc[]={1,0,-1,0};
    int r=0,c=0,dir=0;
    
    for(char ch:path){
        if(ch=='L') dir=(dir+3)%4;
        else if(ch=='R') dir=(dir+1)%4;
        else { // M
            int nr=r+dr[dir], nc=c+dc[dir];
            if(dir==0){passBlock(r,c);passBlock(r-1,c);}
            else if(dir==2){passBlock(r,nc);passBlock(r-1,nc);}
            else if(dir==1){passBlock(r,c);passBlock(r,c-1);}
            else{passBlock(nr,c);passBlock(nr,c-1);}
            r=nr;c=nc;
        }
    }
    cout<<total<<endl;
}
