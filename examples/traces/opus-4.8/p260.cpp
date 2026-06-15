#include <bits/stdc++.h>
using namespace std;

int N;

int main(){
    scanf("%d",&N);
    vector<bitset<256>> A(N+1);
    for(int i=0;i<N;i++){
        int k; scanf("%d",&k);
        for(int t=0;t<k;t++){
            int c; scanf("%d",&c); c--;
            A[c].set(i);
        }
    }
    vector<int> init(N);
    for(int i=0;i<N;i++) scanf("%d",&init[i]);
    
    for(int target=0;target<=1;target++){
        vector<bitset<256>> M(N);
        for(int j=0;j<N;j++){
            M[j]=A[j];
            if(init[j]^target) M[j].set(N);
        }
        vector<int> where(N,-1);
        int row=0;
        for(int col=0;col<N && row<N;col++){
            int sel=-1;
            for(int r=row;r<N;r++){
                if(M[r][col]){sel=r;break;}
            }
            if(sel==-1) continue;
            swap(M[sel],M[row]);
            for(int r=0;r<N;r++){
                if(r!=row && M[r][col]){
                    M[r]^=M[row];
                }
            }
            where[col]=row;
            row++;
        }
        bool ok=true;
        for(int r=row;r<N;r++){
            if(M[r][N]){ok=false;break;}
        }
        if(!ok) continue;
        vector<int> x(N,0);
        for(int col=0;col<N;col++){
            if(where[col]!=-1){
                x[col]=M[where[col]][N];
            }
        }
        vector<int> res;
        for(int i=0;i<N;i++) if(x[i]) res.push_back(i+1);
        printf("%d\n",(int)res.size());
        for(int i=0;i<(int)res.size();i++){
            printf("%d%c",res[i], i+1<(int)res.size()?' ':'\n');
        }
        if(res.empty()) printf("\n");
        return 0;
    }
    printf("-1\n");
    return 0;
}
