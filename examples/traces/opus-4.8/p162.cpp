#include <bits/stdc++.h>
using namespace std;

int main(){
    double AB,AC,AD,BC,BD,CD;
    cin>>AB>>AC>>AD>>BC>>BD>>CD;
    double a2=AB*AB, b2=AC*AC, c2=AD*AD;
    double d2=BC*BC, e2=BD*BD, f2=CD*CD;
    // Cayley-Menger determinant
    // 288 V^2 = det
    double M[5][5];
    for(int i=0;i<5;i++)for(int j=0;j<5;j++)M[i][j]=0;
    // row0/col0 = 0 and 1s
    M[0][0]=0;
    M[0][1]=1;M[0][2]=1;M[0][3]=1;M[0][4]=1;
    M[1][0]=1;M[2][0]=1;M[3][0]=1;M[4][0]=1;
    // points 1=A,2=B,3=C,4=D
    M[1][1]=0; M[1][2]=a2; M[1][3]=b2; M[1][4]=c2;
    M[2][1]=a2;M[2][2]=0;  M[2][3]=d2; M[2][4]=e2;
    M[3][1]=b2;M[3][2]=d2; M[3][3]=0;  M[3][4]=f2;
    M[4][1]=c2;M[4][2]=e2; M[4][3]=f2; M[4][4]=0;
    
    // compute determinant of 5x5
    double det=1;
    int n=5;
    for(int col=0;col<n;col++){
        int piv=-1;
        for(int r=col;r<n;r++) if(fabs(M[r][col])>1e-12){piv=r;break;}
        if(piv<0){det=0;break;}
        if(piv!=col){for(int k=0;k<n;k++)swap(M[piv][k],M[col][k]);det=-det;}
        det*=M[col][col];
        for(int r=col+1;r<n;r++){
            double f=M[r][col]/M[col][col];
            for(int k=col;k<n;k++)M[r][k]-=f*M[col][k];
        }
    }
    double V2=det/288.0;
    if(V2<0)V2=0;
    double V=sqrt(V2);
    printf("%.4f\n",V);
    return 0;
}
