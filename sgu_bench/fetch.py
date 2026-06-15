"""Download and unpack the test data.

The ~1.1 GB of generated tests (~300 MB gzipped) are not stored in git; they
ship as a GitHub Release asset and are unpacked into ``problems/*/tests/`` on
demand. Override the source with ``SGU_BENCH_TESTS_URL``.
"""

import os
import sys
import tarfile
import tempfile
import urllib.request
from pathlib import Path

from .paths import data_root, tests_present

DEFAULT_TESTS_URL = (
    "https://github.com/radoslav11/sgu-bench/releases/download/"
    "tests-v1/sgu-bench-tests-v1.tar.gz"
)


def tests_url() -> str:
    return os.environ.get("SGU_BENCH_TESTS_URL", DEFAULT_TESTS_URL)


def _report(done, total):
    if total > 0:
        pct = done * 100 // total
        mb = done / 1048576
        sys.stderr.write(f"\r  downloading tests: {pct}% ({mb:.0f} MB)")
        sys.stderr.flush()


def fetch_tests(force: bool = False, url: str | None = None) -> Path:
    """Download the test tarball and extract it into the data root."""
    root = data_root()
    if tests_present() and not force:
        return root / "problems"

    url = url or tests_url()
    print(f"Fetching test data from {url}", file=sys.stderr)
    with tempfile.NamedTemporaryFile(suffix=".tar.gz", delete=False) as tmp:
        tmp_path = Path(tmp.name)

    def hook(block, block_size, total):
        _report(block * block_size, total)

    try:
        urllib.request.urlretrieve(url, tmp_path, reporthook=hook)
        sys.stderr.write("\n")
        print("Extracting...", file=sys.stderr)
        with tarfile.open(tmp_path, "r:gz") as tar:
            tar.extractall(root)
    finally:
        tmp_path.unlink(missing_ok=True)

    if not tests_present():
        raise RuntimeError(
            "test data did not unpack as expected; check the tarball layout "
            "(it must contain problems/pNNN/tests/*.in)"
        )
    print(f"Test data ready under {root / 'problems'}", file=sys.stderr)
    return root / "problems"
