import heapq
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    pos = 1
    adj = [[] for _ in range(n + 1)]
    for idx in range(1, n):
        u = int(data[pos])
        v = int(data[pos + 1])
        pos += 2
        t = data[pos]
        pos += 1
        if t == b"almost":
            pos += 1
            adj[u].append((v, idx))
            adj[v].append((u, idx))
        else:
            adj[u].append((v, -1))
            adj[v].append((u, -1))

    # greedy: walking the tree from Kat, whenever more than half of the
    # path edges are almost protected, convert the one closest to the root
    alive = set()
    heap = []
    ans = []

    stack = []

    def enter(u, p, depth):
        if len(alive) * 2 > depth:
            while True:
                key = heap[0]
                if key in alive:
                    break
                heapq.heappop(heap)
            key = heapq.heappop(heap)
            alive.remove(key)
            ans.append(key[1])
        stack.append([u, p, depth, 0, None])

    enter(1, 0, 0)
    while stack:
        fr = stack[-1]
        u, p, depth = fr[0], fr[1], fr[2]
        if fr[4] is not None:
            if fr[4] in alive:
                alive.remove(fr[4])
            fr[4] = None
        if fr[3] < len(adj[u]):
            v, eidx = adj[u][fr[3]]
            fr[3] += 1
            if v == p:
                continue
            if eidx != -1:
                key = (depth, eidx)
                alive.add(key)
                heapq.heappush(heap, key)
                fr[4] = key
            enter(v, u, depth + 1)
        else:
            stack.pop()

    print(len(ans))
    print(*ans)


if __name__ == "__main__":
    main()
