"""Functions used to generate source files during build time"""

from pathlib import Path

import methods
import sys


def export_icon_builder(args):
    src_path = Path(str(args[1]))
    src_name = src_path.stem
    platform = src_path.parent.parent.stem

    with open(str(args[1]), "r") as file:
        svg = file.read()

    with methods.generated_wrapper(str(args[0])) as file:
        file.write(
            f"""\
inline constexpr const char *_{platform}_{src_name}_svg = {methods.to_raw_cstring(svg)};
"""
        )


def register_platform_apis_builder(args):
    platforms = args[1].read()
    api_inc = "\n".join([f'#include "{p}/api/api.h"' for p in platforms])
    api_reg = "\n\t".join([f"register_{p}_api();" for p in platforms])
    api_unreg = "\n\t".join([f"unregister_{p}_api();" for p in platforms])
    with methods.generated_wrapper(str(args[0])) as file:
        file.write(
            f"""\
#include "register_platform_apis.h"

{api_inc}

void register_platform_apis() {{
	{api_reg}
}}

void unregister_platform_apis() {{
	{api_unreg}
}}
"""
        )


# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "export_icon_builder":
        args.pop(0)
        export_icon_builder(args)
    if args[0] == "register_platform_apis_builder":
        args.pop(0)
        register_platform_apis_builder(args)
