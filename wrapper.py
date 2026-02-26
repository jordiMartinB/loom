import os
import importlib
import inspect
import json
import sys
from typing import Any, Callable, Dict, Tuple

__all__ = ["run_topo", "run_octi", "run_transitmap", "run_loom"]

_backend_cache: Dict[Tuple[str, str], Callable] = {}  # key: (backend_name, func_name)


def _backend_name() -> str:
    """Read LOOM_BACKEND_MODULE at call time (allows tests to change env)."""
    return os.environ.get("LOOM_BACKEND_MODULE", "loom")


def _find_in_module(module, name: str) -> Callable:
    """Return attribute 'name' from module or its submodules, or raise ImportError."""
    if hasattr(module, name):
        return getattr(module, name)
    for attr in dir(module):
        try:
            item = getattr(module, attr)
        except Exception:
            continue
        if inspect.ismodule(item) and hasattr(item, name):
            return getattr(item, name)
    raise ImportError(f"function {name!r} not found in module {module.__name__!r} or its attributes")


def _resolve(name: str) -> Callable:
    """Resolve and cache backend callable by name and backend."""
    backend = _backend_name()
    cache_key = (backend, name)
    if cache_key in _backend_cache:
        return _backend_cache[cache_key]
    try:
        mod = importlib.import_module(backend)
    except Exception as e:
        # If import fails, try adding project lib/ to sys.path and retry
        lib_dir = os.path.join(os.path.dirname(__file__), "lib")
        if os.path.isdir(lib_dir) and lib_dir not in sys.path:
            sys.path.insert(0, lib_dir)
            try:
                mod = importlib.import_module(backend)
            except Exception as e2:
                raise ImportError(f"could not import backend module {backend!r}: {e2}") from e2
        else:
            raise ImportError(f"could not import backend module {backend!r}: {e}") from e
    fn = _find_in_module(mod, name)
    _backend_cache[cache_key] = fn
    return fn


def _call(name: str, *args, **kwargs) -> Any:
    fn = _resolve(name)
    return fn(*args, **kwargs)


def run_topo(*args, **kwargs):
    return _call("run_topo", *args, **kwargs)


def run_octi(*args, **kwargs):
    return _call("run_octi", *args, **kwargs)


def run_transitmap(*args, **kwargs):
    return _call("run_transitmap", *args, **kwargs)


def run_loom(*args, **kwargs):
    return _call("run_loom", *args, **kwargs)