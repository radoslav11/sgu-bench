"""Evaluate a Claude model on sgu-bench.

    pip install sgu-bench[anthropic]
    python -m sgu_bench fetch
    ANTHROPIC_API_KEY=... python examples/anthropic_solver.py --model claude-sonnet-4-6 p100 p123 p172
"""

import argparse
import re

from anthropic import Anthropic

from sgu_bench import Benchmark

PROMPT = """\
Solve this competitive programming problem in C++17. Read from standard input
and write to standard output (even if the statement mentions files). Reply with
exactly one ```cpp code block containing the full program.

{statement}
"""


def make_solver(model: str):
    client = Anthropic()

    def solver(statement: str) -> str:
        msg = client.messages.create(
            model=model,
            max_tokens=8192,
            messages=[{"role": "user", "content": PROMPT.format(statement=statement)}],
        )
        reply = "".join(b.text for b in msg.content if b.type == "text")
        blocks = re.findall(r"```(?:cpp|c\+\+)?\n(.*?)```", reply, re.S)
        return blocks[-1] if blocks else reply

    return solver


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="claude-sonnet-4-6")
    ap.add_argument("problems", nargs="*", default=["p100", "p123", "p172"])
    args = ap.parse_args()

    bench = Benchmark()
    report = bench.evaluate(
        make_solver(args.model),
        problems=args.problems or "all",
        on_result=lambda e: print(f"{e.problem_id}: {e.verdict}"),
    )
    print("\n" + str(report))


if __name__ == "__main__":
    main()
