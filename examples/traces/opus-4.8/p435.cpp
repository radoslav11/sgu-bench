#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    if(!(cin>>n)) return 0;
    vector<double> X(n),Y(n),R(n);
    for(int i=0;i<n;i++) cin>>X[i]>>Y[i]>>R[i];
    
    const double PI=acos(-1.0);
    vector<double> areaGE(n+2,0.0);
    
    for(int i=0;i<n;i++){
        double xi=X[i], yi=Y[i], ri=R[i];
        vector<double> ang;
        for(int j=0;j<n;j++){
            if(j==i) continue;
            double dx=X[j]-xi, dy=Y[j]-yi;
            double d=sqrt(dx*dx+dy*dy);
            double rj=R[j];
            if(d>=ri+rj-1e-12) continue; // disjoint or external tangent
            if(d<=fabs(ri-rj)+1e-12) continue; // one contains other
            double base=atan2(dy,dx);
            double a=(ri*ri+d*d-rj*rj)/(2*ri*d);
            if(a>1) a=1; if(a<-1) a=-1;
            double delta=acos(a);
            double a1=base-delta, a2=base+delta;
            // normalize to [0,2pi)
            a1=fmod(a1,2*PI); if(a1<0) a1+=2*PI;
            a2=fmod(a2,2*PI); if(a2<0) a2+=2*PI;
            ang.push_back(a1);
            ang.push_back(a2);
        }
        sort(ang.begin(),ang.end());
        
        vector<pair<double,double>> arcs;
        if(ang.empty()){
            arcs.push_back({0.0,2*PI});
        } else {
            int m=ang.size();
            for(int t=0;t<m;t++){
                double a=ang[t];
                double b=ang[(t+1)%m];
                if(b<=a) b+=2*PI;
                arcs.push_back({a,b});
            }
        }
        
        for(auto &arc:arcs){
            double a=arc.first, b=arc.second;
            if(b-a<1e-15) continue;
            double mid=(a+b)/2.0;
            double px=xi+ri*cos(mid);
            double py=yi+ri*sin(mid);
            int cnt=0;
            for(int j=0;j<n;j++){
                if(j==i) continue;
                double dx=px-X[j], dy=py-Y[j];
                if(dx*dx+dy*dy < R[j]*R[j]) cnt++;
            }
            double contribution = 0.5*( xi*ri*(sin(b)-sin(a)) + yi*ri*(cos(a)-cos(b)) + ri*ri*(b-a) );
            areaGE[cnt+1]+=contribution;
        }
    }
    
    // A(>=k) = areaGE[k], exact(k)=areaGE[k]-areaGE[k+1]
    double burned=0, inverse=0;
    for(int k=1;k<=n;k++){
        double ex=areaGE[k]-(k+1<=n+1?areaGE[k+1]:0.0);
        if(k%2==1) burned+=ex;
        else inverse+=ex;
    }
    
    cout<<fixed<<setprecision(5)<<burned<<" "<<inverse<<"\n";
    return 0;
}
