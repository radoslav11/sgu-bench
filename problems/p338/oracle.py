import sys


def main():
    # Read Big Man's bet.
    X = float(sys.stdin.readline().strip())

    # We need sums 0..27.
    # 21 is the largest safe sum, and one more roll can add up to 6.
    N = 28

    def roll_avg(values, s):
        """
        Average value after rolling one fair six-sided die
        from current sum s.
        """
        return sum(values[s + d] for d in range(1, 7)) / 6.0

    # ------------------------------------------------------------
    # Step 1: Big Man's stopping EV against croupier alone.
    # ------------------------------------------------------------

    # U_b[Sb] = Big Man's EV if he stops at score Sb.
    # Bust scores remain -1.
    U_b = [-1.0] * N

    for Sb in range(1, 22):
        # f[Sc] = Big Man's EV when croupier currently has score Sc.
        f = [0.0] * N

        # If croupier busts, Big Man wins.
        for Sc in range(22, N):
            f[Sc] = 1.0

        # Compute croupier decisions backwards.
        for Sc in range(21, 0, -1):
            # If croupier stops, Big Man loses ties.
            stop_v = -1.0 if Sc >= Sb else 1.0

            # If croupier rolls, average future values.
            roll_v = roll_avg(f, Sc)

            # Croupier minimizes Big Man's EV.
            # Ties go to rolling.
            f[Sc] = roll_v if roll_v <= stop_v else stop_v

        # Croupier must roll at least once.
        U_b[Sb] = roll_avg(f, 0)

    # ------------------------------------------------------------
    # Step 2: Big Man's optimal policy and final-score distribution.
    # ------------------------------------------------------------

    # V_bm[Sb] = Big Man's optimal EV at current score Sb.
    V_bm = [-1.0] * N

    # action_bm[Sb] = 1 means roll, 0 means stop.
    action_bm = [0] * N

    for Sb in range(N - 1, 0, -1):
        # Bust states remain -1.
        if Sb > 21:
            continue

        # Value if Big Man stops.
        stop_v = U_b[Sb]

        # Value if Big Man rolls.
        roll_v = roll_avg(V_bm, Sb)

        # Big Man maximizes EV; ties go to rolling.
        if roll_v >= stop_v:
            V_bm[Sb] = roll_v
            action_bm[Sb] = 1
        else:
            V_bm[Sb] = stop_v
            action_bm[Sb] = 0

    # reach[Sb] = probability Big Man reaches score Sb.
    reach = [0.0] * N

    # Big Man must roll once initially.
    for d in range(1, 7):
        reach[d] = 1.0 / 6.0

    # dist_b[Sb] = probability Big Man's final score is Sb.
    dist_b = [0.0] * N

    # Propagate probabilities in increasing score order.
    for Sb in range(1, 22):
        if action_bm[Sb] == 1:
            # Big Man rolls, so distribute probability to future scores.
            for d in range(1, 7):
                reach[Sb + d] += reach[Sb] / 6.0
        else:
            # Big Man stops here.
            dist_b[Sb] = reach[Sb]

    # Bust scores are final.
    for Sb in range(22, N):
        dist_b[Sb] = reach[Sb]

    # ------------------------------------------------------------
    # Step 3 and 4: Andrew's optimal EV given Big Man's score.
    # ------------------------------------------------------------

    def andrew_ev_given(Sb):
        """
        Compute Andrew's optimal expected profit,
        assuming Big Man's final score is Sb.
        """

        # stop_andrew[Sa] = Andrew's EV if he stops at score Sa.
        stop_andrew = [-1.0] * N

        for Sa in range(1, 22):
            # g[Sc] = casino's total EV from croupier score Sc.
            # a[Sc] = Andrew's own EV under the croupier policy
            #         that maximizes g.
            g = [0.0] * N
            a = [0.0] * N

            # Values when croupier busts.
            # If Big Man busted, casino wins X from him; otherwise loses X.
            big_bust = X if Sb > 21 else -X

            # If croupier busts, Andrew wins unless he already busted.
            # Sa is non-bust here, but keep formulas general.
            and_bust_casino = 1.0 if Sa > 21 else -1.0
            and_bust_andrew = -1.0 if Sa > 21 else 1.0

            # Boundary states for croupier bust.
            for Sc in range(22, N):
                g[Sc] = big_bust + and_bust_casino
                a[Sc] = and_bust_andrew

            # Croupier chooses action maximizing casino EV.
            for Sc in range(21, 0, -1):
                # Casino profit from Big Man if croupier stops.
                if Sb > 21:
                    stop_big = X
                else:
                    stop_big = X if Sc >= Sb else -X

                # Casino profit from Andrew if croupier stops.
                if Sa > 21:
                    stop_and_c = 1.0
                    stop_and_a = -1.0
                else:
                    if Sc >= Sa:
                        stop_and_c = 1.0
                        stop_and_a = -1.0
                    else:
                        stop_and_c = -1.0
                        stop_and_a = 1.0

                # Total casino EV if stopping.
                stop_casino = stop_big + stop_and_c

                # Total casino EV if rolling.
                roll_casino = roll_avg(g, Sc)

                # Andrew EV if croupier rolls.
                roll_andrew = roll_avg(a, Sc)

                # Croupier maximizes casino EV; ties go to rolling.
                if roll_casino >= stop_casino:
                    g[Sc] = roll_casino
                    a[Sc] = roll_andrew
                else:
                    g[Sc] = stop_casino
                    a[Sc] = stop_and_a

            # Croupier must roll at least once.
            stop_andrew[Sa] = roll_avg(a, 0)

        # V_a[Sa] = Andrew's optimal EV at current score Sa.
        V_a = [-1.0] * N

        for Sa in range(N - 1, 0, -1):
            # Bust states remain -1.
            if Sa > 21:
                continue

            # Andrew can stop or roll.
            stop_v = stop_andrew[Sa]
            roll_v = roll_avg(V_a, Sa)

            # Andrew maximizes EV; ties go to rolling.
            V_a[Sa] = roll_v if roll_v >= stop_v else stop_v

        # Andrew must roll at least once initially.
        return roll_avg(V_a, 0)

    # ------------------------------------------------------------
    # Step 5: Average Andrew's EV over Big Man's final distribution.
    # ------------------------------------------------------------

    total_ev = 0.0

    for Sb in range(1, N):
        if dist_b[Sb] != 0.0:
            total_ev += dist_b[Sb] * andrew_ev_given(Sb)

    # Convert EV to probability.
    probability = (total_ev + 1.0) / 2.0

    # Print answer.
    print(f"{probability:.6f}")


if __name__ == "__main__":
    main()
