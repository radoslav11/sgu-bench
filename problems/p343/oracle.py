import sys
import math

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    # Read T (number of returns) and N (number of assets)
    T = int(next(it))
    N = int(next(it))

    # Quantities of each asset
    qty = [int(next(it)) for _ in range(N)]

    # Prices for days 0..T (T+1 rows), N columns each
    # price[t][i] is price of asset i at day t
    price = [[0.0] * N for _ in range(T + 1)]
    for t in range(T + 1):
        for i in range(N):
            price[t][i] = float(next(it))

    # ----- Step 1: compute portfolio value VP and weights w -----
    # V_i = qty_i * price_0(i), VP = sum V_i, w_i = V_i / VP
    values = [qty[i] * price[0][i] for i in range(N)]
    VP = sum(values)
    w = [v / VP for v in values]

    # ----- Step 2: compute mean return per asset m[i] -----
    # r_t(i) = (price_{t-1}(i) - price_t(i)) / price_t(i)
    sums = [0.0] * N
    for t in range(1, T + 1):
        prev_row = price[t - 1]
        cur_row = price[t]
        for i in range(N):
            sums[i] += (prev_row[i] - cur_row[i]) / cur_row[i]

    m = [s / T for s in sums]                  # mean return per asset
    mP = sum(w[i] * m[i] for i in range(N))    # mean return of portfolio

    # ----- Step 3: compute portfolio variance directly in O(T*N) -----
    # var = (1/T) * sum_t ( sum_i w_i * (r_t(i) - m_i) )^2
    var = 0.0
    for t in range(1, T + 1):
        prev_row = price[t - 1]
        cur_row = price[t]

        demeaned_portfolio_return = 0.0
        for i in range(N):
            r = (prev_row[i] - cur_row[i]) / cur_row[i]  # r_t(i)
            demeaned_portfolio_return += w[i] * (r - m[i])

        var += demeaned_portfolio_return * demeaned_portfolio_return

    var /= T
    sP = math.sqrt(var)

    # ----- Step 4: compute VaR (95% z = 1.644854) -----
    VaR = -VP * (mP - 1.644854 * sP)

    # Print with 2 decimals
    sys.stdout.write(f"{VaR:.2f}\n")

if __name__ == "__main__":
    solve()
