#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    if(n==1){
        printf("0\n0\n");
        return 0;
    }
    int m = n;
    bool odd=false;
    if(m%2==1){ m++; odd=true; } // add dummy player index m-1
    int rounds = m-1;
    
    vector<vector<int>> res(n, vector<int>(n,0));
    
    // circle method: players 0..m-1, dummy is m-1 (if odd)
    // positions: arr of size m, arr[0] fixed, rotate rest
    vector<int> arr(m);
    for(int i=0;i<m;i++) arr[i]=i;
    
    for(int r=0;r<rounds;r++){
        // pair arr[i] with arr[m-1-i]
        for(int i=0;i<m/2;i++){
            int a=arr[i], b=arr[m-1-i];
            // skip dummy
            if(a<n && b<n){
                res[a][b]=r+1;
                res[b][a]=r+1;
            }
        }
        // rotate: keep arr[0], rotate arr[1..m-1]
        int last=arr[m-1];
        for(int i=m-1;i>1;i--) arr[i]=arr[i-1];
        arr[1]=last;
    }
    
    printf("%d\n", rounds);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d", res[i][j]);
            if(j+1<n) printf(" ");
        }
        printf("\n");
    }
    return 0;
}
