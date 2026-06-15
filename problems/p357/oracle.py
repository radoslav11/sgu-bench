import sys
import heapq

def main():
    # Read all integers from stdin in one go, to be robust to line breaks/spaces
    data = list(map(int, sys.stdin.read().split()))
    it = iter(data)

    # button[i] indicates whether button i works:
    # 0..9 digits '0'..'9', 10:'Up', 11:'Down', 12:'--'
    button = [0] * 13

    # First 4 integers: "1", "2", "3", "↑"
    button[1] = next(it)
    button[2] = next(it)
    button[3] = next(it)
    button[10] = next(it)

    # Next 4 integers: "4", "5", "6", "↓"
    button[4] = next(it)
    button[5] = next(it)
    button[6] = next(it)
    button[11] = next(it)

    # Next 3 integers: "7", "8", "9"
    button[7] = next(it)
    button[8] = next(it)
    button[9] = next(it)

    # Next 2 integers: "--", "0"
    button[12] = next(it)
    button[0] = next(it)

    # Finally: X and Y
    X = next(it)
    Y = next(it)

    # Dijkstra's algorithm on 100 nodes (channels 0..99)
    INF = 10**9
    dist = [INF] * 100
    dist[X] = 0

    # Min-heap of (distance, node)
    heap = [(0, X)]

    while heap:
        d, u = heapq.heappop(heap)
        # Skip if this is a stale entry
        if d != dist[u]:
            continue

        # Up: (u + 1) mod 100, cost 1
        if button[10]:
            v = (u + 1) % 100
            nd = d + 1
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(heap, (nd, v))

        # Down: (u + 99) mod 100, cost 1
        if button[11]:
            v = (u + 99) % 100
            nd = d + 1
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(heap, (nd, v))

        # Single-digit jumps: to channel d in 1 click if digit button works
        for dig in range(10):  # 0..9
            if button[dig]:
                v = dig
                nd = d + 1
                if nd < dist[v]:
                    dist[v] = nd
                    heapq.heappush(heap, (nd, v))

        # Two-digit jumps via "--" + two digits: cost 3
        if button[12]:
            for d1 in range(1, 10):  # tens digit 1..9
                if not button[d1]:
                    continue
                for d2 in range(10):  # ones digit 0..9
                    if not button[d2]:
                        continue
                    v = d1 * 10 + d2  # 10..99
                    nd = d + 3
                    if nd < dist[v]:
                        dist[v] = nd
                        heapq.heappush(heap, (nd, v))

    # Output result
    print(-1 if dist[Y] >= INF else dist[Y])

if __name__ == "__main__":
    main()
