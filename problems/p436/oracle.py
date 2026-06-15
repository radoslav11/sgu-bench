import sys


def main():
    # Read the entire input as one string.
    text = sys.stdin.read()

    # Diputs digits from least significant to most significant.
    diputs_chars = "_.,-~='^\""

    # Maximum allowed counts for each Diputs digit.
    max_counts = [2, 3, 5, 7, 11, 13, 17, 19, 23]

    # products[i] is the mixed-radix place value of digit i.
    products = [1]

    # Build place values.
    # products[i + 1] = products[i] * number_of_possible_counts_for_digit_i
    for cnt in max_counts:
        products.append(products[-1] * (cnt + 1))

    # Maximum representable value.
    max_num = products[9] - 1

    # Map each Diputs character to its significance index.
    sig_of = {ch: i for i, ch in enumerate(diputs_chars)}

    def diputs_to_decimal(s):
        """
        Convert a valid nonzero Diputs representation to decimal.
        """
        value = 0

        # Each character contributes its place value.
        for ch in s:
            value += products[sig_of[ch]]

        return value

    def decimal_to_diputs(n):
        """
        Convert a decimal integer to Diputs notation.
        """
        # Zero is special.
        if n == 0:
            return "O"

        counts = [0] * 9

        # Extract mixed-radix digits.
        for i in range(9):
            counts[i] = (n // products[i]) % (max_counts[i] + 1)

        result = []

        # Output from most significant digit to least significant digit.
        for i in range(8, -1, -1):
            if counts[i]:
                result.append(diputs_chars[i] * counts[i])

        return "".join(result)

    # Each token is represented as:
    # [start_position, length, numeric_value, was_decimal]
    tokens = []

    n = len(text)
    i = 0

    # Greedily scan input from left to right.
    while i < n:
        c = text[i]

        # Case 1: decimal number.
        if c.isdigit():
            j = i
            value = 0

            # Decimal numbers may not have leading zeroes.
            # Therefore '0' is always a token of length 1.
            if c == "0":
                j = i + 1
            else:
                # Extend while the token remains a valid decimal number
                # not exceeding max_num.
                while j < n and text[j].isdigit():
                    next_value = value * 10 + (ord(text[j]) - ord("0"))

                    # Stop before exceeding the representable range.
                    if next_value > max_num:
                        break

                    value = next_value
                    j += 1

            # Store decimal token.
            tokens.append([i, j - i, value, True])

            # Continue after token.
            i = j

        # Case 2: Diputs zero.
        elif c == "O":
            tokens.append([i, 1, 0, False])
            i += 1

        # Case 3: nonzero Diputs number.
        elif c in sig_of:
            j = i

            # Previous significance index.
            # A valid Diputs representation must have non-increasing
            # significance from left to right.
            last_sig = 8

            # Counts used in this token.
            counts = [0] * 9

            # Greedily extend the Diputs token.
            while j < n:
                ch = text[j]

                # Stop if not a Diputs digit.
                if ch not in sig_of:
                    break

                s = sig_of[ch]

                # Stop if significance increases.
                if s > last_sig:
                    break

                # Stop if this digit has reached its maximum allowed count.
                if counts[s] >= max_counts[s]:
                    break

                # Accept this character.
                counts[s] += 1

                # Future characters must be no more significant.
                last_sig = s

                # Advance end position.
                j += 1

            # Convert the parsed substring to decimal.
            value = diputs_to_decimal(text[i:j])

            # Store Diputs token.
            tokens.append([i, j - i, value, False])

            # Continue after token.
            i = j

        # Case 4: ordinary non-number text.
        else:
            i += 1

    # If there is an odd number of numeric tokens, sort their values.
    if len(tokens) % 2 == 1:
        values = sorted(token[2] for token in tokens)

        # Put sorted values back into token positions.
        # The original notation type at each position is preserved.
        for idx, value in enumerate(values):
            tokens[idx][2] = value

    # Build the final output.
    result = []
    cursor = 0

    for start, length, value, was_decimal in tokens:
        # Copy unchanged text before this token.
        if cursor < start:
            result.append(text[cursor:start])

        # Convert token to the opposite notation.
        if was_decimal:
            result.append(decimal_to_diputs(value))
        else:
            result.append(str(value))

        # Skip original token.
        cursor = start + length

    # Copy remaining text after the last token.
    if cursor < n:
        result.append(text[cursor:])

    # Write final answer.
    sys.stdout.write("".join(result))


if __name__ == "__main__":
    main()
