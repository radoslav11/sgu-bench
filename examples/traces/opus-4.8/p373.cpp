#include <bits/stdc++.h>
using namespace std;

typedef complex<double> P;
double R;

double crossp(const P&a,const P&b){return real(a)*imag(b)-imag(a)*real(b);}
double dotp(const P&a,const P&b){return real(a)*real(b)+imag(a)*imag(b);}
double normsq(const P&a){return real(a)*real(a)+imag(a)*imag(a);}

double sgnAngle(const P&u,const P&v){
    return atan2(crossp(u,v), dotp(u,v));
}

// area of intersection of disk (radius R, origin) and triangle (O,a,b), signed
double areaCT(P a, P b){
    double R2=R*R;
    double na=normsq(a), nb=normsq(b);
    bool ina = na <= R2 + 1e-9;
    bool inb = nb <= R2 + 1e-9;
    if(ina && inb) return crossp(a,b)/2.0;
    P d = b-a;
    double A = normsq(d);
    if(A < 1e-18){
        // a==b
        return 0;
    }
    double Bc = 2.0*dotp(a,d);
    double Cc = na - R2;
    double disc = Bc*Bc - 4.0*A*Cc;
    if(ina && !inb){
        double sq=sqrt(max(0.0,disc));
        double t = (-Bc + sq)/(2.0*A);
        P p = a + t*d;
        return crossp(a,p)/2.0 + R2*sgnAngle(p,b)/2.0;
    }
    if(!ina && inb){
        double sq=sqrt(max(0.0,disc));
        double t = (-Bc - sq)/(2.0*A);
        P p = a + t*d;
        return R2*sgnAngle(a,p)/2.0 + crossp(p,b)/2.0;
    }
    // both outside
    if(disc <= 0){
        return R2*sgnAngle(a,b)/2.0;
    }
    double sq=sqrt(disc);
    double t1=(-Bc - sq)/(2.0*A);
    double t2=(-Bc + sq)/(2.0*A);
    if(t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1){
        P p = a + t1*d;
        P q = a + t2*d;
        return R2*sgnAngle(a,p)/2.0 + crossp(p,q)/2.0 + R2*sgnAngle(q,b)/2.0;
    } else {
        return R2*sgnAngle(a,b)/2.0;
    }
}

double circlePolyArea(const vector<P>& poly){
    int n=poly.size();
    if(n<3) return 0;
    double s=0;
    for(int i=0;i<n;i++){
        s += areaCT(poly[i], poly[(i+1)%n]);
    }
    return fabs(s);
}

vector<P> clip(const vector<P>& poly, double a,double b,double c,double s){
    vector<P> res;
    int n=poly.size();
    if(n==0) return res;
    for(int i=0;i<n;i++){
        P cur=poly[i], nxt=poly[(i+1)%n];
        double dc = s*(a*real(cur)+b*imag(cur)+c);
        double dn = s*(a*real(nxt)+b*imag(nxt)+c);
        if(dc >= 0) res.push_back(cur);
        if((dc<0) != (dn<0)){
            double t = dc/(dc-dn);
            res.push_back(cur + t*(nxt-cur));
        }
    }
    return res;
}

int main(){
    int N;
    if(!(cin>>N>>R)) return 0;
    vector<array<double,3>> lines(N);
    for(int i=0;i<N;i++){
        cin>>lines[i][0]>>lines[i][1]>>lines[i][2];
    }
    double M=10000.0;
    vector<P> base = {P(-M,-M),P(M,-M),P(M,M),P(-M,M)};
    
    vector<double> areas;
    for(int mask=0; mask<(1<<N); mask++){
        vector<P> poly = base;
        for(int i=0;i<N && !poly.empty();i++){
            double s = (mask&(1<<i))? 1.0 : -1.0;
            poly = clip(poly, lines[i][0], lines[i][1], lines[i][2], s);
        }
        if(poly.size()>=3){
            double ar = circlePolyArea(poly);
            if(ar>1e-9) areas.push_back(ar);
        }
    }
    
    sort(areas.rbegin(), areas.rend());
    double carl=0, pooh=0;
    for(size_t i=0;i<areas.size();i++){
        if(i%2==0) carl+=areas[i];
        else pooh+=areas[i];
    }
    
    cout<<fixed<<setprecision(10)<<carl<<" "<<pooh<<"\n";
    return 0;
}
