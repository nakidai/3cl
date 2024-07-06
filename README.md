3CL
--
CCCL is an implementation of [cool\_char\_lang](https://github.com/holy-8/cool_char_lang),
which is a brainfuck-inspired language

Documentation
--
To write code for it, follow [this documentation](https://github.com/holy-8/cool_char_lang/blob/main/reference/documentation.md)

Also there're [examples](https://github.com/holy-8/cool_char_lang/tree/main/programs)

How to build
--
If you don't know how to build cmake projects, then you should google a little
about it, but in short you need to install (obviously) cmake, some working c
compiler and run these commands in the root of the project:
```
cmake -B build
cmake --build build
```

And `3cl` (or `3cl.exe` ig if you're on windows) will appear in the `build`
directory

Also I think you can use some wrapper of the cmake with gui (or iirc cmake has
its own gui app)

Dependencies
--
- [cvector](https://github.com/eteran/c-vector) (header only, so included in the source)
