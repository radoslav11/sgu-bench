import sys
import math
from array import array


def solve() -> None:
    # Read all input tokens.
    data = sys.stdin.read().strip().split()

    # Empty input guard.
    if not data:
        return

    # Parse first line.
    n = int(data[0])
    dist_total = float(data[1])
    c_change = float(data[2])

    # Arrays for lane parameters.
    a_arr = [0.0] * n
    b_arr = [0.0] * n
    delta_arr = [0.0] * n

    # Parse lane descriptions.
    ptr = 3
    for i in range(n):
        a_arr[i] = float(data[ptr])
        b_arr[i] = float(data[ptr + 1])
        delta_arr[i] = float(data[ptr + 2])
        ptr += 3

    # Number of distance grid steps.
    M = 200000

    # Size of one distance step.
    step_dist = dist_total / M

    # Flat indexing helper for DP arrays.
    #
    # State (i, lane) is stored at:
    #     index = i * n + lane
    def idx(i: int, lane: int) -> int:
        return i * n + lane

    # Exact driving-time solver using bisection.
    #
    # It solves:
    #   b * dt - a * (cos(t_start + dt + delta)
    #                 - cos(t_start + delta)) = dx
    def drive_time_exact(lane: int, t_start: float, dx: float) -> float:
        # No distance means no driving time.
        if dx <= 0.0:
            return 0.0

        # Lane parameters.
        a = a_arr[lane]
        b = b_arr[lane]
        d = delta_arr[lane]

        # Cosine at starting phase.
        cos_t0 = math.cos(t_start + d)

        # Lower bound: pretend speed is maximum b + a.
        lo = dx / (b + a)

        # Upper bound: pretend speed is minimum b - a.
        # Since b > a, this is positive.
        hi = dx / (b - a) if a > 0.0 else lo

        # Safety.
        if hi < lo:
            hi = lo

        # Bisection. 100 iterations is enough for double precision.
        for _ in range(100):
            mid = 0.5 * (lo + hi)

            # Traveled distance with time mid minus requested dx.
            f = b * mid - a * (math.cos(t_start + mid + d) - cos_t0) - dx

            # Too much distance: time is too large.
            if f > 0.0:
                hi = mid
            else:
                lo = mid

        # Return midpoint.
        return 0.5 * (lo + hi)

    # Fast Newton solver for one tiny grid distance.
    def drive_time(lane: int, t_start: float, dx: float) -> float:
        # Lane parameters.
        a = a_arr[lane]
        b = b_arr[lane]
        d = delta_arr[lane]

        # Starting phase sin/cos.
        phase = t_start + d
        cos_t = math.cos(phase)
        sin_t = math.sin(phase)

        # Initial estimate using instantaneous speed.
        dt = dx / (b + a * sin_t)

        # Four Newton iterations.
        for _ in range(4):
            dt2 = dt * dt

            # Taylor approximation of cos(dt).
            cos_dt = (
                1.0
                - dt2
                * 0.5
                * (
                    1.0
                    - dt2
                    / 12.0
                    * (1.0 - dt2 / 30.0 * (1.0 - dt2 / 56.0))
                )
            )

            # Taylor approximation of sin(dt).
            sin_dt = dt * (
                1.0
                - dt2
                / 6.0
                * (
                    1.0
                    - dt2
                    / 20.0
                    * (
                        1.0
                        - dt2
                        / 42.0
                        * (1.0 - dt2 / 72.0)
                    )
                )
            )

            # cos(t_start + dt + delta).
            cos_a = cos_t * cos_dt - sin_t * sin_dt

            # sin(t_start + dt + delta).
            sin_a = sin_t * cos_dt + cos_t * sin_dt

            # Function value: distance(dt) - dx.
            f = b * dt - a * (cos_a - cos_t) - dx

            # Derivative equals final speed.
            fp = b + a * sin_a

            # Newton update.
            dt -= f / fp

        return dt

    # Infinity value for DP.
    INF = 1.0e100

    # Total number of DP states.
    total_states = (M + 1) * n

    # DP time array, stored flat for memory efficiency.
    time_arr = array("d", [INF]) * total_states

    # Parent array:
    #   -1 means start/unset,
    #    same lane means driving parent,
    #    different lane means lane-change parent.
    #
    # Signed char is enough because lane indices are 0..4 and -1 is needed.
    par = array("b", [-1]) * total_states

    # Start state: distance 0, lane 0, time 0.
    time_arr[idx(0, 0)] = 0.0

    # Precompute switch costs.
    switch_cost = [[0.0] * n for _ in range(n)]
    for lane in range(n):
        for prev_lane in range(n):
            switch_cost[lane][prev_lane] = c_change * abs(lane - prev_lane)

    # Main DP.
    for i in range(M + 1):
        base = i * n

        # Snapshot before lane-change relaxation at this distance.
        drive_arrival = [time_arr[base + lane] for lane in range(n)]

        # Try all single lane changes at the same position.
        for lane in range(n):
            best = time_arr[base + lane]
            best_parent = par[base + lane]

            for prev_lane in range(n):
                if prev_lane == lane:
                    continue

                cand = drive_arrival[prev_lane] + switch_cost[lane][prev_lane]

                if cand < best:
                    best = cand
                    best_parent = prev_lane

            time_arr[base + lane] = best
            par[base + lane] = best_parent

        # Drive to the next grid position.
        if i < M:
            next_base = (i + 1) * n

            for lane in range(n):
                t = time_arr[base + lane]
                dt = drive_time(lane, t, step_dist)

                time_arr[next_base + lane] = t + dt
                par[next_base + lane] = lane

    # Choose best final lane.
    best_lane = 0
    final_base = M * n
    for lane in range(1, n):
        if time_arr[final_base + lane] < time_arr[final_base + best_lane]:
            best_lane = lane

    # Reconstruct lane sequence and switch times.
    lane_seq = [best_lane]
    switch_times = []

    cur_i = M
    cur_lane = best_lane

    while True:
        p = par[idx(cur_i, cur_lane)]

        # Reached the start.
        if p == -1:
            break

        if p == cur_lane:
            # Driving step.
            cur_i -= 1
        else:
            # Lane change from p to cur_lane.
            switch_times.append(time_arr[idx(cur_i, p)])

            # Continue reconstruction in parent lane.
            cur_lane = p
            lane_seq.append(cur_lane)

    # Reverse because reconstruction was backwards.
    lane_seq.reverse()
    switch_times.reverse()

    # Store original DP switch times as fallback.
    dp_times = switch_times[:]

    # Refine switch times using local optimality.
    for i in range(len(switch_times)):
        prev_lane = lane_seq[i]
        next_lane = lane_seq[i + 1]

        # Duration of this lane change.
        c_dur = c_change * abs(next_lane - prev_lane)

        # Lane parameters.
        a_p = a_arr[prev_lane]
        b_p = b_arr[prev_lane]
        d_p = delta_arr[prev_lane]

        a_n = a_arr[next_lane]
        b_n = b_arr[next_lane]
        d_n = delta_arr[next_lane]

        # Start from DP estimate.
        t_dp = switch_times[i]
        t = t_dp

        # Newton solve:
        #   v_prev(t) - v_next(t + c_dur) = 0
        for _ in range(60):
            f = (
                b_p
                + a_p * math.sin(t + d_p)
                - b_n
                - a_n * math.sin(t + c_dur + d_n)
            )

            fp = (
                a_p * math.cos(t + d_p)
                - a_n * math.cos(t + c_dur + d_n)
            )

            # Avoid unstable division.
            if abs(fp) < 1e-13:
                break

            # Clamp Newton step.
            step_t = f / fp
            if step_t < -0.3:
                step_t = -0.3
            elif step_t > 0.3:
                step_t = 0.3

            t -= step_t

            if abs(step_t) < 1e-16:
                break

        # Reject suspicious stationary point.
        derivative = (
            a_p * math.cos(t + d_p)
            - a_n * math.cos(t + c_dur + d_n)
        )

        if derivative >= 0.0:
            t = t_dp

        # Ensure chronological feasibility.
        if i == 0:
            min_t = 0.0
        else:
            min_t = (
                switch_times[i - 1]
                + c_change * abs(lane_seq[i] - lane_seq[i - 1])
            )

        switch_times[i] = max(t, min_t)

    # Exact schedule evaluation.
    def compute(times):
        total_dist = 0.0
        cur_time = 0.0

        # By default, all switches are used.
        used_switches = len(times)

        for i, t_sw in enumerate(times):
            prev_lane = lane_seq[i]
            next_lane = lane_seq[i + 1]

            a = a_arr[prev_lane]
            b = b_arr[prev_lane]
            d = delta_arr[prev_lane]

            # Distance driven before this switch.
            seg = (
                b * (t_sw - cur_time)
                - a * (math.cos(t_sw + d) - math.cos(cur_time + d))
            )

            # If target is reached before this switch, stop here.
            if total_dist + seg >= dist_total:
                used_switches = i
                break

            total_dist += seg

            # Add switch duration.
            cur_time = t_sw + c_change * abs(next_lane - prev_lane)

        # Remaining distance to finish.
        rem = max(0.0, dist_total - total_dist)

        # Current lane is lane_seq[used_switches].
        finish_lane = lane_seq[used_switches]

        # Drive remaining distance exactly.
        t_end = cur_time + drive_time_exact(finish_lane, cur_time, rem)

        return t_end, used_switches

    # Compare refined and unrefined schedules.
    t_refined, len_refined = compute(switch_times)
    t_dp, len_dp = compute(dp_times)

    if t_dp < t_refined:
        chosen_times = dp_times
        t_end = t_dp
        schedule_len = len_dp
    else:
        chosen_times = switch_times
        t_end = t_refined
        schedule_len = len_refined

    # Output result.
    out = []
    out.append(f"{t_end:.17f}")
    out.append(str(schedule_len))

    for i in range(schedule_len):
        # Convert lane index to 1-based.
        out.append(f"{lane_seq[i + 1] + 1} {chosen_times[i]:.17f}")

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
