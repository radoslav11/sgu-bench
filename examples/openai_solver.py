"""Evaluate an OpenAI model on sgu-bench.

    pip install sgu-bench[openai]
    python -m sgu_bench fetch                 # one-time test download
    OPENAI_API_KEY=... python examples/openai_solver.py --model gpt-5-mini p100 p123 p172

A "solver" is just a function statement -> C++ source. That's the only thing
a new user has to write to plug in their own inference.
"""

import argparse
import re

from openai import OpenAI

from sgu_bench import Benchmark

PROMPT = """\
Solve this competitive programming problem in C++17. Read from standard input
and write to standard output (even if the statement mentions files). Reply with
exactly one ```cpp code block containing the full program.

{statement}
"""


def make_solver(model: str, effort: str):
    client = OpenAI()

    def solver(statement: str) -> str:
        resp = client.chat.completions.create(
            model=model,
            reasoning_effort=effort,
            messages=[{"role": "user", "content": PROMPT.format(statement=statement)}],
        )
        reply = resp.choices[0].message.content or ""
        blocks = re.findall(r"```(?:cpp|c\+\+)?\n(.*?)```", reply, re.S)
        return blocks[-1] if blocks else reply

    return solver


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="gpt-5-mini")
    ap.add_argument("--effort", default="low")
    ap.add_argument("problems", nargs="*", default=["p100", "p123", "p172"])
    args = ap.parse_args()

    bench = Benchmark()
    solver = make_solver(args.model, args.effort)
    report = bench.evaluate(
        solver,
        problems=args.problems or "all",
        on_result=lambda e: print(f"{e.problem_id}: {e.verdict}"),
    )
    print("\n" + str(report))


if __name__ == "__main__":
    main()
