import sys

def main() -> None:
    # Read 7 integers: red, yellow, green, brown, blue, pink, black
    balls = list(map(int, sys.stdin.readline().split()))
    state = sys.stdin.readline().strip()

    reds = balls[0]

    # Sum of remaining colours when they are finally cleared (each at most 1 in input)
    # Index mapping: balls[1] is yellow worth 2, ..., balls[6] is black worth 7
    colour_sum = 0
    for i in range(1, 7):
        colour_sum += balls[i] * (i + 1)

    if reds > 0:
        # While reds exist, optimal play is to alternate each red with a black:
        # each (red+black) gives 1+7 = 8 points.
        score = reds * 8 + colour_sum

        # If the last potted ball was red, next must be a colour.
        # Best is black (7). With reds existing, that colour is re-spotted,
        # so it doesn't reduce the later final colour clearance.
        if state == "RED":
            score += 7
    else:
        # No reds: we are in the final colour-clearance situation
        score = colour_sum

        # If last was red, we must pot a colour next.
        # With no reds, we can choose the best remaining colour to maximize points.
        if state == "RED":
            # Find highest value colour still present.
            for i in range(6, 0, -1):  # black down to yellow
                if balls[i] > 0:
                    score += (i + 1)
                    break

    print(score)

if __name__ == "__main__":
    main()
