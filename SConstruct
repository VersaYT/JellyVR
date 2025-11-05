#!/usr/bin/env python
import os
import sys

env = SConscript("engine/godot-cpp/SConstruct")

sources = Glob("src/*.cpp")

if env["platform"] == "linux":
    # env.Append(LIBS=[
    #     'libmpv',
    #     "GL"
    #     ])
    # env.Append(LIBPATH=['../dependencies/mpv-build/mpv/build'])  # Set library path
    # env.Append(CPPPATH=['../dependencies/mpv-build/mpv/include'])  # Set header path

    library = env.SharedLibrary(
        "build/jellyvr{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )
else:
    # Ensure all necessary libraries are linked
    library = env.SharedLibrary(
        "build/jellyvr{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )


Default(library)