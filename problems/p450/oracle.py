import sys
import heapq
from collections import deque

INF = 2_000_000_042  # "infinity" span used in comparisons


def solve() -> None:
    data = sys.stdin.read().strip().split()
    it = iter(data)

    n = int(next(it))  # number of counters
    m = int(next(it))  # number of groups
    Tclose = int(next(it))  # closing time

    cap = [int(next(it)) for _ in range(n)]  # seats per counter

    start = [0] * m
    group_size = [0] * m
    deadline = [0] * m
    eat_time = [0] * m
    for i in range(m):
        start[i] = int(next(it))
        group_size[i] = int(next(it))
        deadline[i] = int(next(it))
        eat_time[i] = int(next(it))

    # cc[c][s] = 1 if occupied, else 0
    cc = [[0] * cap[c] for c in range(n)]

    # satisfaction per group, default -1 (means "gave up")
    satisfaction = [-1.0] * m

    # opts[i] stores (counter_idx, pos, group_len) to remove later
    # We'll also store spans if we want, but only counter and pos are necessary.
    opts = [None] * m

    # Event heap: (time, id)
    # id meanings:
    #   0..m-1 : arrival of group id
    #   <0     : departure of group (-id-1)
    #   == m   : dummy wake-up at start[i] + deadline[i]
    ev = []
    for i in range(m):
        heapq.heappush(ev, (start[i], i))
        heapq.heappush(ev, (start[i] + deadline[i], m))  # dummy

    waiting = deque()

    def better(a, b):
        """Return True if option a is better than b.
        option = (min_dist, max_dist, counter_idx, pos)
        """
        if a[0] != b[0]:
            return a[0] > b[0]
        if a[1] != b[1]:
            return a[1] > b[1]
        if a[2] != b[2]:
            return a[2] < b[2]
        return a[3] < b[3]

    def can_seat(g):
        """Try to seat group g now. If success, occupy seats and save opts[g]."""
        P = group_size[g]
        found = False
        best = None  # (min_dist, max_dist, c, pos)

        for c in range(n):
            C = cap[c]
            if P > C:
                continue

            # Sliding window sum over occupancy
            window_sum = 0
            last_left = -1
            last_right = P  # will move to first occupied seat to the right

            # init window [0, P-1]
            for j in range(P):
                window_sum += cc[c][j]

            # For each possible start i
            for i0 in range(0, C - P + 1):
                if i0 > 0:
                    # slide window by 1:
                    window_sum += cc[c][i0 + P - 1] - cc[c][i0 - 1]

                    # if seat leaving on the left was occupied, update last_left
                    if cc[c][i0 - 1]:
                        last_left = i0 - 1

                    # last_right must be at least end of window
                    if last_right < i0 + P:
                        last_right = i0 + P

                # move last_right to the next occupied seat or end
                while last_right < C and cc[c][last_right] == 0:
                    last_right += 1

                # If window is empty, compute option score
                if window_sum == 0:
                    left_span = INF if last_left == -1 else (i0 - 1 - last_left)
                    right_span = INF if last_right == C else (last_right - i0 - P)

                    mn = left_span if left_span < right_span else right_span
                    mx = right_span if left_span < right_span else left_span
                    cur = (mn, mx, c, i0)

                    if (not found) or better(cur, best):
                        found = True
                        best = cur

        if not found:
            return False

        # Occupy seats according to best option
        _, _, c, pos = best
        for k in range(group_size[g]):
            cc[c][pos + k] = 1

        opts[g] = (c, pos, group_size[g])
        return True

    def remove_group(g):
        """Free seats occupied by group g."""
        c, pos, P = opts[g]
        for k in range(P):
            cc[c][pos + k] = 0

    # Main simulation loop
    while ev:
        cur_time, eid = ev[0]

        # cannot start seating at/after closing time
        if cur_time >= Tclose:
            break

        # pop all events at cur_time
        while ev and ev[0][0] == cur_time:
            _, eid = heapq.heappop(ev)

            if eid < 0:
                g = -eid - 1
                remove_group(g)
            elif 0 <= eid < m:
                waiting.append(eid)
            # eid == m: dummy, do nothing

        # after applying all changes at this time, try to seat from the front
        while waiting:
            g = waiting[0]
            # If still within patience, try to seat now
            if cur_time <= start[g] + deadline[g]:
                if can_seat(g):
                    # schedule departure
                    heapq.heappush(ev, (cur_time + eat_time[g], -(g + 1)))

                    # compute satisfaction (per customer)
                    satisfaction[g] = (start[g] + deadline[g] - cur_time) / deadline[g]

                    waiting.popleft()
                    continue

            # If patience expired, group leaves now
            if cur_time >= start[g] + deadline[g]:
                waiting.popleft()
                continue

            # Otherwise cannot seat now and still waiting => stop (FIFO blocks)
            break

    # Weighted average satisfaction
    tot = 0.0
    tot_sz = 0
    for i in range(m):
        tot += satisfaction[i] * group_size[i]
        tot_sz += group_size[i]

    ans = tot / tot_sz
    # print with enough precision
    sys.stdout.write(f"{ans:.16f}\n")


if __name__ == "__main__":
    solve()
