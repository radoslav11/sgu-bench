"""High-level API: plug in your inference, get verdicts.

    from sgu_bench import Benchmark

    bench = Benchmark()                 # uses the bundled problems/

    def solver(statement: str) -> str:  # your model / agent goes here
        return my_llm_generate_cpp(statement)

    report = bench.evaluate(solver, problems="all")
    print(report.solved, "/", report.total)

A solver is any callable that takes the problem statement and returns source
code (C++ by default). The benchmark compiles it, runs it in a resource-
limited sandbox over the hidden tests, and grades it (AC/WA/TLE/MLE/RE/CE).
"""

import re
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, List, Sequence, Union

from . import paths
from .fetch import ensure_data
from .judge import Evaluation, evaluate as _evaluate_source

Solver = Callable[[str], str]


@dataclass
class Report:
    results: List[Evaluation] = field(default_factory=list)

    @property
    def total(self) -> int:
        return len(self.results)

    @property
    def solved(self) -> int:
        return sum(1 for e in self.results if e.passed)

    @property
    def by_verdict(self) -> dict:
        out: dict = {}
        for e in self.results:
            out[e.verdict] = out.get(e.verdict, 0) + 1
        return out

    def failures(self) -> List[Evaluation]:
        return [e for e in self.results if not e.passed]

    def __str__(self) -> str:
        lines = [f"solved {self.solved}/{self.total}", f"  {self.by_verdict}"]
        for e in self.failures():
            lines.append(f"  {e.problem_id}: {e.verdict}")
        return "\n".join(lines)


_SUFFIX = {"cpp": ".cpp", "py": ".py"}


class Benchmark:
    def __init__(self, data_root: Union[str, Path, None] = None, auto_fetch: bool = True):
        if data_root is not None:
            import os

            os.environ["SGU_BENCH_DATA"] = str(data_root)
        self.auto_fetch = auto_fetch

    @property
    def problems_dir(self) -> Path:
        return paths.problems_dir()

    def problem_ids(self) -> List[str]:
        self._ensure_tests()
        ids = [
            d.name
            for d in self.problems_dir.iterdir()
            if re.fullmatch(r"p\d+", d.name) and (d / "config.json").exists()
        ]
        return sorted(ids, key=lambda s: int(s[1:]))

    def statement(self, pid: str) -> str:
        self._ensure_tests()
        return (self.problems_dir / pid / "statement.txt").read_text(
            errors="replace"
        )

    def _resolve(self, problems: Union[str, Sequence[str]]) -> List[str]:
        if problems == "all":
            return self.problem_ids()
        if isinstance(problems, str):
            return [problems]
        return list(problems)

    def _ensure_tests(self):
        if paths.tests_present():
            return
        if self.auto_fetch:
            ensure_data()  # downloads + unpacks on first use
        else:
            raise RuntimeError(
                "test data not found — run `python -m sgu_bench fetch` "
                "(or call sgu_bench.fetch_tests()) to download it."
            )

    def evaluate_source(
        self, pid: str, source: str, language: str = "cpp"
    ) -> Evaluation:
        """Grade already-written source for one problem."""
        self._ensure_tests()
        workdir = Path(tempfile.mkdtemp(prefix=f"sgu_src_{pid}_"))
        src_path = workdir / f"{pid}{_SUFFIX[language]}"
        src_path.write_text(source)
        return _evaluate_source(self.problems_dir / pid, src_path)

    def evaluate(
        self,
        solver: Solver,
        problems: Union[str, Sequence[str]] = "all",
        language: str = "cpp",
        on_result: Union[Callable[[Evaluation], None], None] = None,
    ) -> Report:
        """Run `solver(statement) -> source` over the chosen problems."""
        self._ensure_tests()
        report = Report()
        for pid in self._resolve(problems):
            source = solver(self.statement(pid))
            result = self.evaluate_source(pid, source, language)
            report.results.append(result)
            if on_result:
                on_result(result)
        return report
