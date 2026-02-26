import os
import sys
import importlib
import unittest

class WrapperIntegrationTest(unittest.TestCase):
    def setUp(self):
        self.test_dir = os.path.abspath(os.path.dirname(__file__))
        self.proj_root = os.path.abspath(os.path.join(self.test_dir, ".."))
        self._orig_sys_path = list(sys.path)

        # Ensure the test dir (contains backend_real.py) is searched first
        if self.test_dir in sys.path:
            sys.path.remove(self.test_dir)
        sys.path.insert(0, self.test_dir)

        # Also ensure project root is available for wrapper.py
        if self.proj_root in sys.path:
            sys.path.remove(self.proj_root)
        sys.path.insert(1, self.proj_root)

        # set backend before importing wrapper
        self._orig_env = os.environ.get("LOOM_BACKEND_MODULE")
        os.environ["LOOM_BACKEND_MODULE"] = "backend_real"

        # remove cached modules so import picks up env/sys.path changes
        for mod in ("wrapper", "backend_real"):
            sys.modules.pop(mod, None)

        self.wrapper = importlib.import_module("wrapper")
        importlib.reload(self.wrapper)

    def tearDown(self):
        # restore env
        if self._orig_env is None:
            os.environ.pop("LOOM_BACKEND_MODULE", None)
        else:
            os.environ["LOOM_BACKEND_MODULE"] = self._orig_env

        # remove modules we added so tests don't leak state
        for mod in ("wrapper", "backend_real"):
            sys.modules.pop(mod, None)

        sys.path[:] = self._orig_sys_path

    def test_proxy_functions(self):
        self.assertEqual(self.wrapper.run_topo(1, y=2), ("topo", 1, 2))
        self.assertEqual(self.wrapper.run_octi(), "octi")
        self.assertEqual(self.wrapper.run_transitmap(3, 4), 7)
        self.assertEqual(self.wrapper.run_loom(a=5), {"a": 5})

if __name__ == "__main__":
    unittest.main()