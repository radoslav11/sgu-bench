import sys
sys.setrecursionlimit(10**7)

def read_input():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    C = int(next(it))
    P = int(next(it))
    T = int(next(it))
    A = [0] + [int(next(it)) for _ in range(n)]
    return n, C, P, T, A

def can_deliver(x, n, C, P, T, A):
    # 1) Assign x people from the lowest floors upward
    rem = []   # list of (floor, people_to_take)
    need = x
    for i in range(1, n+1):
        if need <= 0:
            break
        take = min(need, A[i])
        if take > 0:
            rem.append([i, take])
            need -= take
    if need > 0:
        return False  # not enough people

    total_time = 0

    # 2) Handle full-C loads separately per floor
    for item in rem:
        floor, cnt = item
        full_trips = cnt // C
        total_time += full_trips * 2 * floor * P
        if total_time > T:
            return False
        # leftover fewer than C, stored back for the mixed-trip phase
        item[1] = cnt % C

    # rebuild leftover list filtering zeros
    leftover = []
    for floor, cnt in rem:
        if cnt > 0:
            leftover.append([floor, cnt])

    # 3) Mixed trips for remainders
    # Always pick from the highest floors first to minimize height
    while leftover:
        top_floor, _ = leftover[-1]
        total_time += 2 * top_floor * P
        if total_time > T:
            return False

        cap = 0
        # fill up to C by taking from back
        while cap < C and leftover:
            floor, cnt = leftover[-1]
            take = min(C - cap, cnt)
            cap += take
            cnt -= take
            if cnt == 0:
                leftover.pop()  # floor exhausted
            else:
                leftover[-1][1] = cnt

    return total_time <= T

def solve():
    n, C, P, T, A = read_input()
    # Binary search on x
    low, high = 0, sum(A)
    answer = 0
    while low <= high:
        mid = (low + high) // 2
        if can_deliver(mid, n, C, P, T, A):
            answer = mid
            low = mid + 1
        else:
            high = mid - 1
    print(answer)

if __name__ == "__main__":
    solve()
