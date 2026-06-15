import sys

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return

    it = iter(data)
    # Read number of people N and number of windows M
    n = int(next(it))
    m = int(next(it))

    arrival = [0] * n
    service = [0] * n

    # Read arrival and service times
    for i in range(n):
        arrival[i] = int(next(it))
        service[i] = int(next(it))

    # finish_times[w] will store a list of finish times of all people
    # who have chosen window w (0-based index), in the order they are served.
    finish_times = [[] for _ in range(m)]

    out_lines = []

    # Process each person in input order (arrival times are non-decreasing)
    for i in range(n):
        k = arrival[i]   # arrival time
        t = service[i]   # service duration

        # Compute how many people are currently in each window's queue at time k.
        # A person is in queue if their finish time > k.
        queue_count = [0] * m
        for w in range(m):
            # Count how many finish times are strictly greater than k
            count = 0
            for finish in finish_times[w]:
                if finish > k:
                    count += 1
            queue_count[w] = count

        # Choose the window with minimal queue size, and with smallest index on ties.
        best_window = 0
        for w in range(1, m):
            if queue_count[w] < queue_count[best_window]:
                best_window = w
            # If equal, we keep the older best_window, yielding smallest index.

        # Compute the start time at this window.
        # Person cannot start before arrival time k or before the last finish in that window.
        if finish_times[best_window]:
            last_finish = finish_times[best_window][-1]
            start_time = max(k, last_finish)
        else:
            # No one assigned to this window yet; can start immediately at arrival.
            start_time = k

        # End time is start time plus service duration
        end_time = start_time + t

        # Record this finish time in the chosen window's schedule
        finish_times[best_window].append(end_time)

        # Window number is 1-based in output
        out_lines.append(f"{best_window + 1} {end_time}")

    # Print all lines at once
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()
