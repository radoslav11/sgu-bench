"""Where the benchmark data lives.

By default the problem packages sit in ``problems/`` next to the installed
package (the repo root). Override with the ``SGU_BENCH_DATA`` environment
variable to point at a checkout / cache elsewhere.
"""

import os
from pathlib import Path

_PACKAGE_DIR = Path(__file__).resolve().parent


def data_root() -> Path:
    env = os.environ.get("SGU_BENCH_DATA")
    return Path(env).expanduser().resolve() if env else _PACKAGE_DIR.parent


def problems_dir() -> Path:
    return data_root() / "problems"


def tests_present() -> bool:
    """True once the test data (a Release download) has been unpacked."""
    sample = problems_dir() / "p100" / "tests"
    return sample.is_dir() and any(sample.glob("*.in"))
