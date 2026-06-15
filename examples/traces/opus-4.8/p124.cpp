#include <bits/stdc++.h>
using namespace std;

int main(){
    int k;
    scanf("%d",&k);
    vector<int> X1(k),Y1(k),X2(k),Y2(k);
    for(int i=0;i<k;i++){
        scanf("%d %d %d %d",&X1[i],&Y1[i],&X2[i],&Y2[i]);
    }
    int x0,y0;
    scanf("%d %d",&x0,&y0);
    
    // Check border
    for(int i=0;i<k;i++){
        int xa=X1[i],ya=Y1[i],xb=X2[i],yb=Y2[i];
        int xmin=min(xa,xb),xmax=max(xa,xb);
        int ymin=min(ya,yb),ymax=max(ya,yb);
        if(x0>=xmin&&x0<=xmax&&y0>=ymin&&y0<=ymax){
            // point on the line of segment
            // segment is axis parallel
            if(xa==xb){
                // vertical, x must equal xa
                if(x0==xa && y0>=ymin && y0<=ymax){
                    printf("BORDER\n");
                    return 0;
                }
            } else {
                // horizontal
                if(y0==ya && x0>=xmin && x0<=xmax){
                    printf("BORDER\n");
                    return 0;
                }
            }
        }
    }
    
    // Ray casting with vertical segments
    int cnt=0;
    for(int i=0;i<k;i++){
        if(X1[i]!=X2[i]) continue; // only vertical
        int xv=X1[i];
        int ya=min(Y1[i],Y2[i]);
        int yb=max(Y1[i],Y2[i]);
        if(xv>x0 && ya<=y0 && y0<yb){
            cnt++;
        }
    }
    
    if(cnt%2==1) printf("INSIDE\n");
    else printf("OUTSIDE\n");
    return 0;
}
