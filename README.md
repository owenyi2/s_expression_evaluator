# S expression Evaluator

Work in Progress: Right now it is just a calculator with lisp-like (prefix) notation and variables 

# Usage Examples

```
./main
> (+ 1 1)
2
> (setq r 0.05)
0.050000
> (setq P 10000)
10000.000000
> (setq months 24)
24.000000
> (* P (^ (+ 1 (/ r 12)) months))
11049.413356
```

# TODO

Memory Leak because `snode_free_recursive` does not properly free the contents of `Eval_Atom` in particular `eval_atom->identifier`.

```valgrind
==65295== 8 bytes in 1 blocks are definitely lost in loss record 3 of 9
==65295==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==65295==    by 0x10B0DB: ea_from_string (atom.h:101)
==65295==    by 0x10B3BF: _type_check_snode_recursive (atom.h:184)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
==65295==    by 0x10B581: _type_check_snode_recursive (atom.h:224)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
==65295==    by 0x10B581: _type_check_snode_recursive (atom.h:224)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
==65295==    by 0x10B581: _type_check_snode_recursive (atom.h:224)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
==65295==    by 0x10B5A7: _type_check_snode_recursive (atom.h:228)
```

Make SNode have a `(void* free_atom_fptr)(void*)` to call during `snode_free_recursive`
