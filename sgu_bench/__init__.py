"""sgu_bench — an offline, verifiable benchmark of 452 ACM SGU problems.

Quick start:
    from sgu_bench import Benchmark, fetch_tests
    fetch_tests()                       # one-time: download the hidden tests
    bench = Benchmark()
    report = bench.evaluate(lambda statement: my_model(statement))
    print(report)
"""

from .benchmark import Benchmark, Report, Solver
from .fetch import fetch_tests
from .judge import Evaluation, TestVerdict, evaluate
from .meta import ProblemMeta, parse_statement

__all__ = [
    "Benchmark",
    "Report",
    "Solver",
    "fetch_tests",
    "evaluate",
    "Evaluation",
    "TestVerdict",
    "parse_statement",
    "ProblemMeta",
]
