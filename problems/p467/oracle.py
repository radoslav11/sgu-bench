import sys

def min_shots(p):
    """
    Given p in [0..100], return the minimum number of radius-0.5 circles
    needed to cover at least p% of the unit square.
    """
    if p == 0:
        return 0
    # One circle covers ~78.54%
    if p <= 78:
        return 1
    # Two circles cover ~95%
    if p <= 95:
        return 2
    # Three circles cover ~99%
    if p <= 99:
        return 3
    # Only four cover 100%
    return 4

def main():
    case_num = 1
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        p = int(line)
        result = min_shots(p)
        print(f"Case #{case_num}: {result}")
        case_num += 1

if __name__ == "__main__":
    main()
