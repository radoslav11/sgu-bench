#include <bits/stdc++.h>
using namespace std;
bool solve(vector<vector<int>>& mat, vector<int>& rhs, int N, vector<int>& x) {
    vector<vector<int>> M(N, vector<int>(N+1));
    for(int i=0;i<N;i++){for(int j=0;j<N;j++) M[i][j]=mat[i][j]; M[i][N]=rhs[i];}
    vector<int> pivot_col(N,-1);
    int row=0;
    for(int col=0;col<N && row<N;col++){
        int found=-1;
        for(int r=row;r<N;r++) if(M[r][col]){found=r;break;}
        if(found==-1) continue;
        swap(M[row],M[found]);
        pivot_col[row]=col;
        for(int r=0;r<N;r++) if(r!=row && M[r][col]) for(int c=0;c<=N;c++) M[r][c]^=M[row][c];
        row++;
    }
    for(int r=row;r<N;r++) if(M[r][N]) return false;
    x.assign(N,0);
    for(int r=0;r<row;r++) if(pivot_col[r]!=-1) x[pivot_col[r]]=M[r][N];
    return true;
}
int main(){
    int N; cin>>N;
    vector<vector<int>> A(N, vector<int>(N,0));
    for(int i=0;i<N;i++){int k; cin>>k; for(int m=0;m<k;m++){int c; cin>>c; c--; A[i][c]=1;}}
    vector<int> init(N);
    for(int i=0;i<N;i++) cin>>init[i];
    vector<vector<int>> mat(N, vector<int>(N,0));
    for(int j=0;j<N;j++) for(int i=0;i<N;i++) mat[j][i]=A[i][j];
    for(int t=0;t<=1;t++){
        vector<int> rhs(N);
        for(int j=0;j<N;j++) rhs[j]=init[j]^t;
        vector<int> x;
        if(solve(mat,rhs,N,x)){
            vector<int> clicks;
            for(int i=0;i<N;i++) if(x[i]) clicks.push_back(i+1);
            cout<<clicks.size()<<"\n";
            for(int i=0;i<(int)clicks.size();i++){if(i) cout<<" "; cout<<clicks[i];}
            if(!clicks.empty()) cout<<"\n";
            return 0;
        }
    }
    cout<<-1<<"\n";
}
