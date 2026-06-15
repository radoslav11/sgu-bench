import sys


def main():
    # Read all lines from standard input.
    lines = sys.stdin.read().splitlines()

    # Pointer to the current line.
    idx = 0

    # First line contains N and M.
    n, m = map(int, lines[idx].split())
    idx += 1

    # Second line contains Peter's scores for M subjects.
    scores = list(map(int, lines[idx].split()))
    idx += 1

    # This list will store final accepted output lines.
    accepted = []

    # Process each faculty block.
    for _ in range(n):
        # Faculty name may contain spaces, so read the entire line.
        faculty_name = lines[idx]
        idx += 1

        # Next line contains K followed by K subject indices.
        data = list(map(int, lines[idx].split()))
        idx += 1

        # First number is K.
        k = data[0]

        # Remaining K numbers are subject indices.
        subjects = data[1:]

        # Next line contains 3*K integers: X, Y, Z for each required subject.
        threshold_values = list(map(int, lines[idx].split()))
        idx += 1

        # Convert the flat list into triples.
        thresholds = []
        for i in range(k):
            x = threshold_values[3 * i]
            y = threshold_values[3 * i + 1]
            z = threshold_values[3 * i + 2]
            thresholds.append((x, y, z))

        # Compute Peter's total grade for this faculty.
        total = 0

        # For every required subject, convert the test score to a grade.
        for i in range(k):
            # Subject indices in input are 1-based.
            subject_index = subjects[i] - 1

            # Peter's test score for this subject.
            t = scores[subject_index]

            # Thresholds for this subject.
            x, y, z = thresholds[i]

            # Convert score to grade.
            if t < x:
                grade = 2
            elif t < y:
                grade = 3
            elif t < z:
                grade = 4
            else:
                grade = 5

            # Add this grade to the total.
            total += grade

        # Next line contains number of specialties.
        s = int(lines[idx])
        idx += 1

        # Process all specialties.
        for _ in range(s):
            # Specialty name may contain spaces.
            specialty_name = lines[idx]
            idx += 1

            # Required total grade sum.
            required = int(lines[idx])
            idx += 1

            # If Peter's total is enough, the application is accepted.
            if total >= required:
                accepted.append(f"{faculty_name} {specialty_name}")

    # If there are accepted applications, print them.
    if accepted:
        print("\n".join(accepted))
    else:
        # Otherwise print Army.
        print("Army")


if __name__ == "__main__":
    main()
