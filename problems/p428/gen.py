"""Test for p428 (Rebus).

The input file is empty (the task is pure output: print 1000 proper
aligned addition rebuses), so there is exactly one test and the usual
minimal/maximal seed plan does not apply.
"""

import sys

seed = int(sys.argv[1])
assert seed == 1
