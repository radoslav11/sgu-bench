pop x
if mand[x]>=3 || deg[x]<2: fail
if mand[x]==2 && !delDone[x]:
   delDone[x]=true
   for each undecided edge e of x: deleteEdge(e)  // updates and pushes
if deg[x]==2 && mand[x]<2 && !... :
   make all undecided mandatory
