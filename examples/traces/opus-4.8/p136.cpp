#include <bits/stdc++.h>
using namespace std;

long long parse(const string&s){
    // parse number with up to 3 decimals, return *1000 as long long
    bool neg=false; size_t i=0;
    if(i<s.size()&&(s[i]=='-'||s[i]=='+')){neg=(s[i]=='-');i++;}
    long long intp=0; 
    while(i<s.size()&&isdigit(s[i])){intp=intp*10+(s[i]-'0');i++;}
    long long frac=0; int cnt=0;
    if(i<s.size()&&s[i]=='.'){
        i++;
        while(i<s.size()&&isdigit(s[i])&&cnt<3){frac=frac*10+(s[i]-'0');cnt++;i++;}
    }
    while(cnt<3){frac*=10;cnt++;}
    long long val=intp*1000+frac;
    return neg?-val:val;
}

int main(){
    int n;
    if(!(cin>>n))return 0;
    vector<long long> mx(n),my(n);
    for(int i=0;i<n;i++){
        string a,b; cin>>a>>b;
        mx[i]=parse(a); my[i]=parse(b);
    }
    vector<long long> vx(n),vy(n);
    if(n%2==1){
        // v_1 = sum_{i=1}^{N} (-1)^{i-1} m_i  (i 1-indexed)
        long long sx=0,sy=0;
        for(int i=0;i<n;i++){
            long long sign = (i%2==0)?1:-1; // i=0 -> i_1 -> +
            sx+=sign*mx[i]; sy+=sign*my[i];
        }
        vx[0]=sx; vy[0]=sy;
        for(int i=0;i<n-1;i++){
            vx[i+1]=2*mx[i]-vx[i];
            vy[i+1]=2*my[i]-vy[i];
        }
    } else {
        // check alternating sum ==0: sum (-1)^i m_i (i 1-indexed) 
        // (-1)^i with i starting at 1: i=1 -> -, i=2 -> +...
        long long sx=0,sy=0;
        for(int i=0;i<n;i++){
            long long sign = ((i+1)%2==0)?1:-1; // 1-indexed i+1
            sx+=sign*mx[i]; sy+=sign*my[i];
        }
        if(sx!=0||sy!=0){
            printf("NO\n");
            return 0;
        }
        vx[0]=0; vy[0]=0;
        for(int i=0;i<n-1;i++){
            vx[i+1]=2*mx[i]-vx[i];
            vy[i+1]=2*my[i]-vy[i];
        }
    }
    printf("YES\n");
    for(int i=0;i<n;i++){
        // vx are in units of /1000
        double x=vx[i]/1000.0, y=vy[i]/1000.0;
        printf("%.3f %.3f\n",x,y);
    }
    return 0;
}
