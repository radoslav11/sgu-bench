import sys
sys.setrecursionlimit(10**7)

class DSU:
    def __init__(self, n):
        # parent[i] = parent of i; size[i] = size of tree rooted at i
        self.parent = list(range(n))
        self.size = [1] * n

    def find(self, x):
        # Path compression
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, a, b):
        # Union by size; return True if merged, False if already same set
        ra = self.find(a)
        rb = self.find(b)
        if ra == rb:
            return False
        if self.size[ra] > self.size[rb]:
            ra, rb = rb, ra
        # attach smaller tree ra under rb
        self.parent[ra] = rb
        self.size[rb] += self.size[ra]
        return True

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    M = int(next(it))

    # We'll assign IDs 0.. up to 2*M−1 for endpoints
    dsu = DSU(2 * M)
    point_id = {}    # map (x,y) -> unique id
    next_id = 0

    for i in range(1, M + 1):
        x1 = int(next(it)); y1 = int(next(it))
        x2 = int(next(it)); y2 = int(next(it))
        # Assign/get ID for (x1,y1)
        if (x1,y1) not in point_id:
            point_id[(x1,y1)] = next_id
            next_id += 1
        u = point_id[(x1,y1)]
        # Assign/get ID for (x2,y2)
        if (x2,y2) not in point_id:
            point_id[(x2,y2)] = next_id
            next_id += 1
        v = point_id[(x2,y2)]

        # If they are in the same DSU set, we found the first cycle
        if not dsu.union(u, v):
            print(i)
            return

    # No cycle ever formed
    print(0)

if __name__ == "__main__":
    main()
