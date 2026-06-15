#include <bits/stdc++.h>
using namespace std;

int n, m;
int grid[6][6]; // -1 black, 0 white(unfilled)
int vClue[6][6], hClue[6][6]; // clues at black cells

struct Run {
    int sum;
    vector<pair<int,int>> cells;
};
vector<Run> runs;
int cellRuns[6][6][2]; // up to 2 runs per cell index

int main(){
    cin >> n >> m;
    cin.ignore();
    vector<vector<string>> cell(n, vector<string>(m));
    for(int i=0;i<n;i++){
        string line;
        getline(cin,line);
        // split by space
        stringstream ss(line);
        for(int j=0;j<m;j++){
            ss >> cell[i][j];
        }
    }
    // determine cell type
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
        if(cell[i][j]=="....."){
            grid[i][j]=0;
        } else {
            grid[i][j]=-1;
        }
        vClue[i][j]=-1; hClue[i][j]=-1;
    }
    // parse clues
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
        string s=cell[i][j];
        if(s=="XXXXX" || s=="....."){
            continue;
        }
        // form AA\BB
        string AA=s.substr(0,2);
        string BB=s.substr(3,2);
        if(AA!="XX") vClue[i][j]=stoi(AA);
        if(BB!="XX") hClue[i][j]=stoi(BB);
    }
    
    // build runs
    // horizontal runs: scan each row, when find white cell start (left neighbor is black or boundary)
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(grid[i][j]==0 && (j==0 || grid[i][j-1]==-1)){
                // start of horizontal run, find clue at cell (i,j-1)
                Run r;
                r.sum = (j>0)? hClue[i][j-1] : -1;
                int jj=j;
                while(jj<m && grid[i][jj]==0){
                    r.cells.push_back({i,jj});
                    jj++;
                }
                if(r.sum>=0) runs.push_back(r);
            }
        }
    }
    // vertical runs
    for(int j=0;j<m;j++){
        for(int i=0;i<n;i++){
            if(grid[i][j]==0 && (i==0 || grid[i-1][j]==-1)){
                Run r;
                r.sum = (i>0)? vClue[i-1][j] : -1;
                int ii=i;
                while(ii<n && grid[ii][j]==0){
                    r.cells.push_back({ii,j});
                    ii++;
                }
                if(r.sum>=0) runs.push_back(r);
            }
        }
    }
    
    int val[6][6];
    for(int i=0;i<n;i++)for(int j=0;j<m;j++) val[i][j]=0;
    
    // list of white cells in order
    vector<pair<int,int>> whites;
    for(int i=0;i<n;i++)for(int j=0;j<m;j++) if(grid[i][j]==0) whites.push_back({i,j});
    
    // for pruning, map each cell to its runs
    // we'll just check validity incrementally
    
    function<bool(int)> solve=[&](int idx)->bool{
        if(idx==(int)whites.size()){
            // verify all run sums
            for(auto &r:runs){
                int s=0;
                for(auto &c:r.cells) s+=val[c.first][c.second];
                if(s!=r.sum) return false;
            }
            return true;
        }
        int ci=whites[idx].first, cj=whites[idx].second;
        for(int d=1;d<=9;d++){
            val[ci][cj]=d;
            // check runs that contain this cell - duplicates and partial sum
            bool ok=true;
            for(auto &r:runs){
                bool contains=false;
                for(auto &c:r.cells) if(c.first==ci&&c.second==cj){contains=true;break;}
                if(!contains) continue;
                // check duplicates and sum
                int s=0; bool full=true;
                set<int> seen;
                for(auto &c:r.cells){
                    int v=val[c.first][c.second];
                    if(v==0){full=false;continue;}
                    if(seen.count(v)){ok=false;break;}
                    seen.insert(v);
                    s+=v;
                }
                if(!ok) break;
                if(full){
                    if(s!=r.sum){ok=false;break;}
                } else {
                    if(s>=r.sum){ok=false;break;}
                }
            }
            if(ok){
                if(solve(idx+1)) return true;
            }
        }
        val[ci][cj]=0;
        return false;
    };
    
    solve(0);
    
    // output
    for(int i=0;i<n;i++){
        string out="";
        for(int j=0;j<m;j++){
            if(j) out+=" ";
            if(grid[i][j]==-1) out+="_";
            else out+=('0'+val[i][j]);
        }
        cout<<out<<"\n";
    }
    return 0;
}
