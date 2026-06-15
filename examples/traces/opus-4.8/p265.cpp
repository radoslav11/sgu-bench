#include <bits/stdc++.h>
using namespace std;
typedef array<array<double,4>,4> Mat;

Mat mul(const Mat&A,const Mat&B){
    Mat C{};
    for(int i=0;i<4;i++)for(int j=0;j<4;j++){double s=0;for(int k=0;k<4;k++)s+=A[i][k]*B[k][j];C[i][j]=s;}
    return C;
}
Mat ident(){Mat M{};for(int i=0;i<4;i++)M[i][i]=1;return M;}

int main(){
    int n; 
    if(scanf("%d",&n)!=1)return 0;
    Mat M=ident();
    for(int t=0;t<n;t++){
        char c; scanf(" %c",&c);
        if(c=='T'){
            double x,y,z; scanf("%lf %lf %lf",&x,&y,&z);
            Mat T=ident(); T[0][3]=x;T[1][3]=y;T[2][3]=z;
            M=mul(T,M);
        } else if(c=='S'){
            double x,y,z; scanf("%lf %lf %lf",&x,&y,&z);
            Mat S=ident(); S[0][0]=x;S[1][1]=y;S[2][2]=z;
            M=mul(S,M);
        } else {
            double x,y,z,a; scanf("%lf %lf %lf %lf",&x,&y,&z,&a);
            double len=sqrt(x*x+y*y+z*z);
            double u=x/len,v=y/len,w=z/len;
            double th=a*M_PI/180.0;
            double cosA=cos(th), sinA=sin(th);
            Mat R=ident();
            R[0][0]=cosA+u*u*(1-cosA);
            R[0][1]=u*v*(1-cosA)-w*sinA;
            R[0][2]=u*w*(1-cosA)+v*sinA;
            R[1][0]=v*u*(1-cosA)+w*sinA;
            R[1][1]=cosA+v*v*(1-cosA);
            R[1][2]=v*w*(1-cosA)-u*sinA;
            R[2][0]=w*u*(1-cosA)-v*sinA;
            R[2][1]=w*v*(1-cosA)+u*sinA;
            R[2][2]=cosA+w*w*(1-cosA);
            M=mul(R,M);
        }
    }
    int m; scanf("%d",&m);
    string out;
    char buf[128];
    while(m--){
        double x,y,z; scanf("%lf %lf %lf",&x,&y,&z);
        double nx=M[0][0]*x+M[0][1]*y+M[0][2]*z+M[0][3];
        double ny=M[1][0]*x+M[1][1]*y+M[1][2]*z+M[1][3];
        double nz=M[2][0]*x+M[2][1]*y+M[2][2]*z+M[2][3];
        sprintf(buf,"%.2f %.2f %.2f\n",nx,ny,nz);
        out+=buf;
    }
    fputs(out.c_str(),stdout);
    return 0;
}
