import sys
from itertools import product


# Token constants.
T_ZERO = 0
T_ONE = 1
T_VAR = 2
T_LPAR = 3
T_RPAR = 4
T_NOT = 5
T_AND = 6
T_OR = 7
T_IMP = 8
T_EQ = 9

# Expression node operation constants.
OP_CONST = 0
OP_VAR = 1
OP_NOT = 2
OP_AND = 3
OP_OR = 4
OP_IMP = 5
OP_EQ = 6


class Parser:
    def __init__(self, tokens, zero_idx, one_idx):
        # Token list of the current formula.
        self.tokens = tokens

        # Current parser position.
        self.pos = 0

        # Expression tree nodes.
        # Each node is a tuple: (operation, first_field, second_field).
        self.nodes = []

        # Algebra index of logical 0.
        self.zero_idx = zero_idx

        # Algebra index of logical 1.
        self.one_idx = one_idx

        # Variables used in this formula.
        self.used = [False] * 26

    def make_node(self, op, a, b=0):
        # Append a new expression-tree node.
        self.nodes.append((op, a, b))

        # Return index of the new node.
        return len(self.nodes) - 1

    def parse_atom(self):
        # Read current token.
        typ, val = self.tokens[self.pos]

        if typ == T_LPAR:
            # Skip '('.
            self.pos += 1

            # Parse full expression inside parentheses.
            node = self.parse_equiv()

            # Skip ')'.
            self.pos += 1

            # Return parsed subtree.
            return node

        if typ == T_ZERO:
            # Skip constant token.
            self.pos += 1

            # Store algebra element index of logical 0.
            return self.make_node(OP_CONST, self.zero_idx)

        if typ == T_ONE:
            # Skip constant token.
            self.pos += 1

            # Store algebra element index of logical 1.
            return self.make_node(OP_CONST, self.one_idx)

        # Otherwise this must be a variable.
        self.pos += 1

        # Mark variable as used.
        self.used[val] = True

        # Create variable node.
        return self.make_node(OP_VAR, val)

    def parse_neg(self):
        # Negation is unary and has high precedence.
        typ, _ = self.tokens[self.pos]

        if typ == T_NOT:
            # Skip '~'.
            self.pos += 1

            # Parse another negation-expression recursively.
            return self.make_node(OP_NOT, self.parse_neg())

        # If there is no '~', parse an atom.
        return self.parse_atom()

    def parse_conj(self):
        # Parse first operand.
        left = self.parse_neg()

        # Conjunction is left-associative.
        while self.tokens[self.pos][0] == T_AND:
            # Skip '&'.
            self.pos += 1

            # Combine current left expression with next operand.
            left = self.make_node(OP_AND, left, self.parse_neg())

        return left

    def parse_disj(self):
        # Parse first operand.
        left = self.parse_conj()

        # Disjunction is left-associative.
        while self.tokens[self.pos][0] == T_OR:
            # Skip '|'.
            self.pos += 1

            # Combine current left expression with next operand.
            left = self.make_node(OP_OR, left, self.parse_conj())

        return left

    def parse_impl(self):
        # Parse left operand.
        left = self.parse_disj()

        # Implication is right-associative.
        if self.tokens[self.pos][0] == T_IMP:
            # Skip '=>'.
            self.pos += 1

            # Recursively parse the right side.
            return self.make_node(OP_IMP, left, self.parse_impl())

        return left

    def parse_equiv(self):
        # Parse first term.
        terms = [self.parse_impl()]

        # Equivalence has lowest precedence.
        while self.tokens[self.pos][0] == T_EQ:
            # Skip '='.
            self.pos += 1

            # Parse next term.
            terms.append(self.parse_impl())

        # If there was no equivalence, return the single term.
        if len(terms) == 1:
            return terms[0]

        # Expand A=B=C into (A=B)&(B=C).
        node = self.make_node(OP_EQ, terms[0], terms[1])

        for i in range(2, len(terms)):
            pair_eq = self.make_node(OP_EQ, terms[i - 1], terms[i])
            node = self.make_node(OP_AND, node, pair_eq)

        return node


def tokenize(s):
    # Result token list.
    tokens = []

    # Current character index.
    i = 0

    while i < len(s):
        c = s[i]

        # Ignore whitespace.
        if c in " \t\r\n":
            i += 1
            continue

        if c == "0":
            tokens.append((T_ZERO, 0))
        elif c == "1":
            tokens.append((T_ONE, 0))
        elif "A" <= c <= "Z":
            tokens.append((T_VAR, ord(c) - ord("A")))
        elif c == "(":
            tokens.append((T_LPAR, 0))
        elif c == ")":
            tokens.append((T_RPAR, 0))
        elif c == "~":
            tokens.append((T_NOT, 0))
        elif c == "&":
            tokens.append((T_AND, 0))
        elif c == "|":
            tokens.append((T_OR, 0))
        elif c == "=":
            # Either implication '=>' or equivalence '='.
            if i + 1 < len(s) and s[i + 1] == ">":
                tokens.append((T_IMP, 0))
                i += 1
            else:
                tokens.append((T_EQ, 0))

        i += 1

    # Sentinel token.
    tokens.append((-1, 0))

    return tokens


def evaluate(root, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl):
    # Recursive evaluator of expression tree.
    op, a, b = nodes[root]

    if op == OP_CONST:
        # Constant stores algebra index directly.
        return a

    if op == OP_VAR:
        # Variable value comes from current assignment.
        return assignment[a]

    if op == OP_NOT:
        # Evaluate child and apply negation table.
        return not_tbl[evaluate(a, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)]

    if op == OP_AND:
        # Evaluate both operands and use precomputed conjunction table.
        x = evaluate(a, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        y = evaluate(b, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        return and_tbl[x][y]

    if op == OP_OR:
        # Evaluate both operands and use precomputed disjunction table.
        x = evaluate(a, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        y = evaluate(b, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        return or_tbl[x][y]

    if op == OP_IMP:
        # Evaluate both operands and use precomputed implication table.
        x = evaluate(a, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        y = evaluate(b, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
        return imp_tbl[x][y]

    # Remaining operation is equivalence.
    x = evaluate(a, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
    y = evaluate(b, nodes, assignment, not_tbl, and_tbl, or_tbl, imp_tbl, eq_tbl)
    return eq_tbl[x][y]


def main():
    data = sys.stdin.read().splitlines()

    # Read first line.
    ptr = 0
    n, m = map(int, data[ptr].split())
    ptr += 1

    # up[x] is a bitmask of vertices reachable from x.
    up = [0] * n

    # Every vertex reaches itself.
    for i in range(n):
        up[i] |= 1 << i

    # Read edges.
    for _ in range(m):
        s, t = map(int, data[ptr].split())
        ptr += 1
        s -= 1
        t -= 1
        up[s] |= 1 << t

    # Compute transitive closure.
    for k in range(n):
        bit_k = 1 << k
        for i in range(n):
            if up[i] & bit_k:
                up[i] |= up[k]

    # down[x] is a bitmask of vertices z such that z <= x.
    down = [0] * n

    for x in range(n):
        mask = 0
        for z in range(n):
            if up[z] & (1 << x):
                mask |= 1 << z
        down[x] = mask

    # Read number of formulae.
    K = int(data[ptr].strip())
    ptr += 1

    # Read formula lines.
    formulas = []
    while len(formulas) < K and ptr < len(data):
        line = data[ptr]
        ptr += 1
        if line.strip():
            formulas.append(line)

    # Full vertex set.
    full = (1 << n) - 1

    # Enumerate all ideals.
    H = [0]

    # Map ideal bitmask to its index.
    index = {0: 0}

    front = 0

    while front < len(H):
        cur = H[front]
        front += 1

        for x in range(n):
            bit_x = 1 << x

            # Skip already present vertices.
            if cur & bit_x:
                continue

            # Vertices below x that are not yet in current ideal.
            need = down[x] & (full ^ cur)

            # We may add x iff the only missing predecessor is x itself.
            if need.bit_count() != 1:
                continue

            # Add x to form a new ideal.
            nxt = cur | bit_x

            # Store if unseen.
            if nxt not in index:
                index[nxt] = len(H)
                H.append(nxt)

    # Number of algebra elements.
    d = len(H)

    # Logical 1 is empty ideal; logical 0 is full ideal.
    one_idx = index[0]
    zero_idx = index[full]

    # Allocate operation tables.
    and_tbl = [[0] * d for _ in range(d)]
    or_tbl = [[0] * d for _ in range(d)]
    imp_tbl = [[0] * d for _ in range(d)]
    eq_tbl = [[0] * d for _ in range(d)]
    not_tbl = [0] * d

    # Precompute conjunction, disjunction, and implication.
    for i in range(d):
        A = H[i]

        for j in range(d):
            B = H[j]

            # Conjunction corresponds to union of ideals.
            and_tbl[i][j] = index[A | B]

            # Disjunction corresponds to intersection of ideals.
            or_tbl[i][j] = index[A & B]

            # Compute implication.
            res = 0

            for x in range(n):
                bit_x = 1 << x

                # x must be in B.
                if not (B & bit_x):
                    continue

                # x must not be in A.
                if A & bit_x:
                    continue

                # x must be maximal in B.
                if (up[x] & B).bit_count() != 1:
                    continue

                # Add downward closure of x.
                res |= down[x]

            imp_tbl[i][j] = index[res]

    # Negation is implication to zero.
    for i in range(d):
        not_tbl[i] = imp_tbl[i][zero_idx]

    # Equivalence is mutual implication plus conjunction.
    for i in range(d):
        for j in range(d):
            eq_tbl[i][j] = and_tbl[imp_tbl[i][j]][imp_tbl[j][i]]

    output = []

    # Process formulae.
    for formula in formulas:
        # Convert string to tokens.
        tokens = tokenize(formula)

        # Parse expression.
        parser = Parser(tokens, zero_idx, one_idx)
        root = parser.parse_equiv()

        # Extract used variables.
        vars_used = [v for v in range(26) if parser.used[v]]

        # Current variable assignment.
        assignment = [0] * 26

        # Initially assume valid.
        valid = True

        # Try all assignments of used variables to algebra elements.
        for values in product(range(d), repeat=len(vars_used)):
            # Fill assignment array.
            for var, value in zip(vars_used, values):
                assignment[var] = value

            # Formula must evaluate to logical 1.
            result = evaluate(
                root,
                parser.nodes,
                assignment,
                not_tbl,
                and_tbl,
                or_tbl,
                imp_tbl,
                eq_tbl,
            )

            if result != one_idx:
                valid = False
                break

        output.append("valid" if valid else "invalid")

    print("\n".join(output))


if __name__ == "__main__":
    main()
