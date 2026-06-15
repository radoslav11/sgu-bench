#include <bits/stdc++.h>
using namespace std;
int main(){
    double x0,y0,r;
    cin>>x0>>y0>>r;
    double mass=acos(-1.0)*r*r*log(x0*x0+y0*y0);
    printf("%.15f\n",mass);
}
