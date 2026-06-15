"""Evaluate an *agent* (feedback loop) rather than a one-shot model.

Shows the other shape sgu-bench supports: instead of a pure
statement -> source function, drive your own loop that reads the verdict and
retries. Here we retry up to N times, feeding the failing verdict back to the
model. Swap `propose` for any backend.

    python -m sgu_bench fetch
    OPENAI_API_KEY=... python examples/agent_solver.py --attempts 3 p100 p172
"""

import argparse
import re

from openai import OpenAI

from sgu_bench import Benchmark

client = OpenAI()


def propose(messages):
    resp = client.chat.completions.create(model="gpt-5-mini", messages=messages)
    reply = resp.choices[0].message.content or ""
    blocks = re.findall(r"```(?:cpp|c\+\+)?\n(.*?)```", reply, re.S)
    return (blocks[-1] if blocks else reply), reply


def solve_with_feedback(bench, pid, max_attempts):
    statement = bench.statement(pid)
    messages = [
        {"role": "user", "content": f"Solve in C++17, read stdin/write stdout, "
                                     f"reply with one ```cpp block.\n\n{statement}"}
    ]
    last = None
    for attempt in range(1, max_attempts + 1):
        source, reply = propose(messages)
        result = bench.evaluate_source(pid, source)
        last = result
        if result.passed:
            return attempt, result
        # feed the first failing test back and retry
        fail = next((t for t in result.tests if t.verdict != "AC"), None)
        detail = (result.compile_log or (fail.detail if fail else ""))[:1500]
        verdict = result.verdict
        messages += [
            {"role": "assistant", "content": reply},
            {"role": "user", "content": f"That got {verdict}. Detail:\n{detail}\nFix it; reply with one ```cpp block."},
        ]
    return max_attempts, last


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--attempts", type=int, default=3)
    ap.add_argument("problems", nargs="*", default=["p100", "p172"])
    args = ap.parse_args()

    bench = Benchmark()
    solved = 0
    for pid in args.problems:
        used, result = solve_with_feedback(bench, pid, args.attempts)
        ok = result.passed
        solved += ok
        print(f"{pid}: {'AC' if ok else result.verdict} in {used} attempt(s)")
    print(f"\nsolved {solved}/{len(args.problems)}")


if __name__ == "__main__":
    main()
