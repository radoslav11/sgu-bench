#include <bits/stdc++.h>
using namespace std;

int main(){
    long long x1,y1,x2,y2,n;
    if(!(cin>>x1>>y1>>x2>>y2>>n)) return 0;
    long long dx=x2-x1, dy=y2-y1;
    if(dx==0 || dy==0){
        cout<<"no solution"<<endl;
        return 0;
    }
    long long sx = dx>0?1:-1;
    long long sy = dy>0?1:-1;
    long long a = llabs(dx);
    long long b = llabs(dy);
    long long cx = (sx>0)? x1 : x1-1;
    long long cy = (sy>0)? y1 : y1-1;
    long long count=1;
    if(count==n){
        cout<<cx<<" "<<cy<<endl;
        return 0;
    }
    long long i=0,j=0;
    while(true){
        bool xv = (i+1)<a;
        bool yv = (j+1)<b;
        if(!xv && !yv) break;
        if(xv && yv){
            long long lx=(i+1)*b;
            long long ly=(j+1)*a;
            if(lx<ly){ cx+=sx; i++; }
            else if(ly<lx){ cy+=sy; j++; }
            else { cx+=sx; cy+=sy; i++; j++; }
        } else if(xv){ cx+=sx; i++; }
        else { cy+=sy; j++; }
        count++;
        if(count==n){
            cout<<cx<<" "<<cy<<endl;
            return 0;
        }
    }
    cout<<"no solution"<<endl;
    return 0;
}
