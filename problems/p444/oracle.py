def find_decimal_info(x, y):
    # visited[r] = index of digit when remainder r first appeared (-1 if unseen)
    visited = [-1] * y

    # initial remainder after integer part
    rem = x % y
    step = 0  # number of digits generated so far

    # long division simulation
    while rem != 0 and visited[rem] == -1:
        visited[rem] = step
        rem = (rem * 10) % y
        step += 1

    # if remainder became zero, decimal terminates
    if rem == 0:
        return step, 0  # non-recurring = step, cycle length = 0

    # found a cycle: starts at visited[rem], length = current step - start
    start = visited[rem]
    cycle_len = step - start
    return start, cycle_len

def main():
    x, y = map(int, input().split())
    non_rec, cycle = find_decimal_info(x, y)
    print(non_rec, cycle)

if __name__ == "__main__":
    main()
