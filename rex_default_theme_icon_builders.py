"""Functions used to generate source files during build time"""

import os
import sys
import methods


# See also `editor/icons/editor_icons_builders.py`.
def make_default_theme_icons_action(args):
    icons_names = []
    icons_raw = []
    target = [args.pop(0)]
    srcs = args[0].split(" ")

    for src in map(str, srcs):
        with open(src, encoding="utf-8", newline="\n") as file:
            icons_raw.append(methods.to_raw_cstring(file.read()))

        name = os.path.splitext(os.path.basename(src))[0]
        icons_names.append(f'"{name}"')

    icons_names_str = ",\n\t".join(icons_names)
    icons_raw_str = ",\n\t".join(icons_raw)

    with methods.generated_wrapper(str(target[0])) as file:
        file.write(f"""\
#include "modules/modules_enabled.gen.h"

inline constexpr int default_theme_icons_count = {len(icons_names)};
inline constexpr const char *default_theme_icons_sources[] = {{
	{icons_raw_str}
}};

inline constexpr const char *default_theme_icons_names[] = {{
	{icons_names_str}
}};
""")

# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "make_default_theme_icons_action":
        args.pop(0)
        make_default_theme_icons_action(args)
