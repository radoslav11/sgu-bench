"""Parse problem metadata out of the repo's statement.txt files.

Statements come in two header flavours:
    time limit per test: 0.25 sec.
    Time limit per test: 0.25 second(s)
and similarly for the memory line. 451/452 problems have both lines
(p235 is missing them and falls back to defaults).
"""

import re
from dataclasses import dataclass
from pathlib import Path

DEFAULT_TIME_LIMIT = 0.25
DEFAULT_MEMORY_KB = 65536

# SGU limits target ~2002 hardware; reference solutions run far faster
# today. The effective limit gives modern submissions the same headroom.
TIME_SCALE = 4.0
TIME_FLOOR = 1.0

_TIME_RE = re.compile(r"time\s*limit\s*per\s*test:\s*([\d.]+)\s*sec", re.I)
_MEM_RE = re.compile(r"memory\s*limit\s*per\s*test:\s*(\d+)\s*KB", re.I)


@dataclass
class ProblemMeta:
    problem_id: str
    title: str
    source_time_limit: float
    memory_kb: int

    @property
    def time_limit(self) -> float:
        return max(TIME_FLOOR, self.source_time_limit * TIME_SCALE)


def parse_statement(problem_id: str, statement_path: Path) -> ProblemMeta:
    text = statement_path.read_text(errors="replace")
    first_line = text.lstrip().splitlines()[0].strip()
    title = re.sub(r"^\d+\.\s*", "", first_line)

    time_match = _TIME_RE.search(text)
    mem_match = _MEM_RE.search(text)
    return ProblemMeta(
        problem_id=problem_id,
        title=title,
        source_time_limit=(
            float(time_match.group(1)) if time_match else DEFAULT_TIME_LIMIT
        ),
        memory_kb=int(mem_match.group(1)) if mem_match else DEFAULT_MEMORY_KB,
    )
