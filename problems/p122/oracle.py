import sys
from collections import deque

def read_graph():
    data = sys.stdin.read().strip().splitlines()
    n = int(data[0])
    adj = []
    for i in range(1, n+1):
        # Convert to zero-based integers
        nbrs = [int(x)-1 for x in data[i].split()]
        adj.append(nbrs)
    return n, adj

def find_hamiltonian_cycle(n, adj):
    # pos[v] = insertion time of v into 'cycle'; -1 if unvisited
    pos = [-1]*n
    cycle = [0]     # start with vertex 0
    pos[0] = 0

    for time in range(1, n):
        u = cycle[-1]
        extended = False

        # 2a. Try to append an unvisited neighbor of u
        for v in adj[u]:
            if pos[v] == -1:
                pos[v] = time
                cycle.append(v)
                extended = True
                break
        if extended:
            continue

        # 2b. Rotation step
        # Mark the successors in the cycle of each visited neighbor of u
        marked = [False]*n
        for v in adj[u]:
            idx = pos[v]
            if idx + 1 < len(cycle):
                marked[cycle[idx+1]] = True

        # Find an unvisited x that is adjacent to some marked y
        for x in range(n):
            if pos[x] != -1:
                continue
            for y in adj[x]:
                if marked[y]:
                    # y = successor of some neighbour v of u; rotate the
                    # path p0..v,y..u into p0..v,u..y (reverse the tail
                    # starting AT y) so x can be appended after y.
                    j = pos[y]
                    tail = list(reversed(cycle[j:]))
                    cycle = cycle[:j] + tail
                    # Update pos[] for reversed segment
                    for k in range(j, len(cycle)):
                        pos[cycle[k]] = k
                    # Finally append x
                    pos[x] = time
                    cycle.append(x)
                    break
            else:
                continue
            break

    # Close the Hamiltonian path into a cycle. If the endpoints are not
    # adjacent, Dirac's condition guarantees an index i with
    # path[0] ~ path[i+1] and path[-1] ~ path[i]; reversing the tail
    # path[i+1:] then yields a closed cycle (crossover argument).
    if n > 2 and len(cycle) == n and cycle[0] not in adj[cycle[-1]]:
        first_adj = set(adj[cycle[0]])
        last_adj = set(adj[cycle[-1]])
        for i in range(1, n - 1):
            if cycle[i + 1] in first_adj and cycle[i] in last_adj:
                cycle = cycle[:i + 1] + cycle[i + 1:][::-1]
                break

    # Convert to 1-based and close the cycle
    return [v+1 for v in cycle] + [cycle[0]+1]

def main():
    n, adj = read_graph()
    if n == 0:
        print("No solution")
        return
    tour = find_hamiltonian_cycle(n, adj)
    if len(tour) != n+1:
        print("No solution")
    else:
        print(" ".join(map(str, tour)))

if __name__ == "__main__":
    main()
