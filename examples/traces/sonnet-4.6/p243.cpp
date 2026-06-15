#include <bits/stdc++.h>
using namespace std;
int N; char board[6][6]; bool used[27];
struct Piece { char id; vector<pair<int,int>> cells; };
vector<Piece> pieces;
vector<pair<int,int>> normalize(vector<pair<int,int>> v){int mr=INT_MAX,mc=INT_MAX;for(auto&p:v){mr=min(mr,p.first);mc=min(mc,p.second);}for(auto&p:v){p.first-=mr;p.second-=mc;}sort(v.begin(),v.end());return v;}
vector<pair<int,int>> rot90(vector<pair<int,int>> v){for(auto&p:v){int r=p.first,c=p.second;p.first=c;p.second=-r;}return normalize(v);}
bool solve(int pos){while(pos<N*N&&board[pos/N][pos%N]!='.')pos++;if(pos==N*N)return true;int tr=pos/N,tc=pos%N;for(int i=0;i<(int)pieces.size();i++){if(used[i])continue;auto cur=pieces[i].cells;for(int rot=0;rot<4;rot++){set<pair<int,int>> tried;for(auto&anc:cur){int or_=tr-anc.first,oc=tc-anc.second;if(tried.count({or_,oc}))continue;tried.insert({or_,oc});bool ok=true;for(auto&p:cur){int nr=or_+p.first,nc=oc+p.second;if(nr<0||nr>=N||nc<0||nc>=N||board[nr][nc]!='.')ok=false;}if(!ok)continue;used[i]=true;for(auto&p:cur)board[or_+p.first][oc+p.second]=pieces[i].id;if(solve(pos+1))return true;for(auto&p:cur)board[or_+p.first][oc+p.second]='.';used[i]=false;}cur=rot90(cur);}}return false;}
int main(){cin>>N;string grid[20];for(int i=0;i<20;i++)cin>>grid[i];map<char,vector<pair<int,int>>>pmap;for(int i=0;i<20;i++)for(int j=0;j<20;j++)if(grid[i][j]!='.')pmap[grid[i][j]].push_back({i,j});for(auto&kv:pmap){Piece p;p.id=kv.first;p.cells=normalize(kv.second);pieces.push_back(p);}for(int i=0;i<N;i++)for(int j=0;j<N;j++)board[i][j]='.';memset(used,false,sizeof(used));solve(0);for(int i=0;i<N;i++){for(int j=0;j<N;j++)cout<<board[i][j];cout<<"\n";}}
