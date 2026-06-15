#include<bits/stdc++.h>
using namespace std;
// height at x given range R, speed v: h(x)=x*(R-x)*g/R * tan(theta)/... 
// h(x) = x*(R-x)* g/(v^2 * something)
// Actually h(x) = x*tanθ*(1-x/R), and tanθ can vary. For fixed R and v: sinθ=sqrt((1±sqrt(1-(gR/v^2)^2))/2)
