#include <bits/stdc++.h>
using namespace std;

double det5(double m[5][5]) {
    double a[5][5];
    for(int i=0;i<5;i++) for(int j=0;j<5;j++) a[i][j]=m[i][j];
    double result = 1.0;
    for(int col=0;col<5;col++){
        int pivot = -1;
        for(int row=col;row<5;row++) if(fabs(a[row][col])>1e-12){pivot=row;break;}
        if(pivot==-1) return 0;
        if(pivot!=col){ for(int j=0;j<5;j++) swap(a[col][j],a[pivot][j]); result=-result; }
        result *= a[col][col];
        for(int row=col+1;row<5;row++){
            double factor = a[row][col]/a[col][col];
            for(int j=col;j<5;j++) a[row][j] -= factor*a[col][j];
        }
    }
    return result;
}

int main(){
    double ab,ac,ad,bc,bd,cd;
    cin>>ab>>ac>>ad>>bc>>bd>>cd;
    double d01=ab*ab, d02=ac*ac, d03=ad*ad;
    double d12=bc*bc, d13=bd*bd, d23=cd*cd;
    
    double m[5][5] = {
        {0, 1,   1,   1,   1  },
        {1, 0,   d01, d02, d03},
        {1, d01, 0,   d12, d13},
        {1, d02, d12, 0,   d23},
        {1, d03, d13, d23, 0  }
    };
    
    double cm = det5(m);
    double v2 = cm / 288.0;
    double v = 0.0;
    if(v2 > 0) v = sqrt(v2);
    
    printf("%.4f\n", v);
    return 0;
}
