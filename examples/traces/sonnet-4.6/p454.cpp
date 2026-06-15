#include <bits/stdc++.h>
using namespace std;
int n,m;
struct Run{int target,size;};
vector<Run> runs;
int h_run[6][6],v_run[6][6],cell_val[6][6];
int run_sum[400],run_mask[400],run_filled[400];
vector<pair<int,int>> whites;
bool iswhite[6][6];
int main(){
    cin>>n>>m;
    vector<vector<string>> grid(n,vector<string>(m));
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)cin>>grid[i][j];
    memset(iswhite,0,sizeof(iswhite));
    memset(h_run,-1,sizeof(h_run));
    memset(v_run,-1,sizeof(v_run));
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)if(grid[i][j]==".....")iswhite[i][j]=true;
    // parse clues and build runs
    // horizontal runs
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(!iswhite[i][j]&&grid[i][j]!="XXXXX"){
                string s=grid[i][j];
                string bb=s.substr(3,2);
                if(bb!="XX"){
                    int clue=stoi(bb);
                    Run r;r.target=clue;r.size=0;
                    int rid=runs.size();
                    runs.push_back(r);
                    int jj=j+1;
                    while(jj<m&&iswhite[i][jj]){h_run[i][jj]=rid;runs[rid].size++;jj++;}
                }
            }
        }
    }
    // vertical runs
    for(int j=0;j<m;j++){
        for(int i=0;i<n;i++){
            if(!iswhite[i][j]&&grid[i][j]!="XXXXX"){
                string s=grid[i][j];
                string aa=s.substr(0,2);
                if(aa!="XX"){
                    int clue=stoi(aa);
                    Run r;r.target=clue;r.size=0;
                    int rid=runs.size();
                    runs.push_back(r);
                    int ii=i+1;
                    while(ii<n&&iswhite[ii][j]){v_run[ii][j]=rid;runs[rid].size++;ii++;}
                }
            }
        }
    }
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)if(iswhite[i][j])whites.push_back({i,j});
    memset(run_sum,0,sizeof(run_sum));
    memset(run_mask,0,sizeof(run_mask));
    memset(run_filled,0,sizeof(run_filled));
    // backtrack
    function<bool(int)> bt=[&](int i)->bool{
        if(i==(int)whites.size())return true;
        auto[r,c]=whites[i];
        int hr=h_run[r][c],vr=v_run[r][c];
        for(int d=1;d<=9;d++){
            int bit=1<<d;
            if((hr>=0&&run_mask[hr]&bit)||(vr>=0&&run_mask[vr]&bit))continue;
            if(hr>=0&&run_sum[hr]+d>runs[hr].target)continue;
            if(vr>=0&&run_sum[vr]+d>runs[vr].target)continue;
            bool hfull=(hr>=0&&run_filled[hr]+1==runs[hr].size);
            bool vfull=(vr>=0&&run_filled[vr]+1==runs[vr].size);
            if(hfull&&run_sum[hr]+d!=runs[hr].target)continue;
            if(vfull&&run_sum[vr]+d!=runs[vr].target)continue;
            cell_val[r][c]=d;
            if(hr>=0){run_sum[hr]+=d;run_mask[hr]|=bit;run_filled[hr]++;}
            if(vr>=0){run_sum[vr]+=d;run_mask[vr]|=bit;run_filled[vr]++;}
            if(bt(i+1))return true;
            cell_val[r][c]=0;
            if(hr>=0){run_sum[hr]-=d;run_mask[hr]^=bit;run_filled[hr]--;}
            if(vr>=0){run_sum[vr]-=d;run_mask[vr]^=bit;run_filled[vr]--;}
        }
        return false;
    };
    bt(0);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(j)cout<<' ';
            if(iswhite[i][j])cout<<cell_val[i][j];
            else cout<<'_';
        }
        cout<<'\n';
    }
}
