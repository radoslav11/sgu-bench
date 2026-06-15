#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m; cin>>n>>m;
    vector<string> grid(n);
    for(int i=0;i<n;i++) cin>>grid[i];
    int r,c;
    for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(grid[i][j]=='P'){r=i;c=j;grid[i][j]='.';}
    int dir=0;
    set<tuple<int,int,int>> vis;
    while(true){
        auto st=make_tuple(r,c,dir);
        if(vis.count(st)){cout<<-1;return 0;}
        vis.insert(st);
        if(dir==0){
            if(r+1>=n){cout<<c+1;return 0;}
            char b=grid[r+1][c];
            if(b=='.'||b=='P'){r++;dir=0;}
            else if(b=='/'){r++;dir=1;}
            else{r++;dir=2;}
        } else if(dir==1){
            if(c-1<0){cout<<-1;return 0;}
            char nx=grid[r][c-1];
            if(nx=='.'){c--;dir=0;}
            else if(nx==grid[r][c]){c--;dir=1;}
            else{cout<<-1;return 0;}
        } else {
            if(c+1>=m){cout<<-1;return 0;}
            char nx=grid[r][c+1];
            if(nx=='.'){c++;dir=0;}
            else if(nx==grid[r][c]){c++;dir=2;}
            else{cout<<-1;return 0;}
        }
    }
}
