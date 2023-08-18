from robot.api import SuiteVisitor
from robot.utils import Matcher
import copy

class ForTest(SuiteVisitor):

    def __init__(self, pattern,n:int):
        self.matcher = Matcher(pattern)
        self.n = n

    def start_suite(self, suite:list):
        """Remove tests that match the given pattern."""
        print("Start")
        suite_tmp = copy.deepcopy(suite)
        suite.tests.clear()
        for t in suite_tmp.tests:
            print("name=",t.longname)
            if self._is_excluded(t):
                for i in range(self.n):
                    print("Not name=", t.longname)
                    suite.tests.append(t)
                #break
        #for t in suite_tmp.tests:
         #   print("QueRen:name=", t.longname)
        #suite.tests = [t for t in suite.tests if not self._is_excluded(t)]
        #suite.tests.append(suite.tests)

    def _is_excluded(self, test):
        return self.matcher.match(test.name) or self.matcher.match(test.longname)

    def end_suite(self, suite):
        print("End")
        """Remove suites that are empty after removing tests."""
        suite.suites = [s for s in suite.suites if s.test_count > 0]

    def visit_test(self, test):
        """Avoid visiting tests and their keywords to save a little time."""
        pass