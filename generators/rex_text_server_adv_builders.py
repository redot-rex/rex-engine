"""Functions used to generate source files during build time"""

import methods
import sys

def make_icu_data(args):
    buffer = methods.get_buffer(str(args[1]))
    with methods.generated_wrapper(str(args[0])) as file:
        file.write(f"""\
/* (C) 2016 and later: Unicode, Inc. and others. */
/* License & terms of use: https://www.unicode.org/copyright.html */

#include <unicode/utypes.h>
#include <unicode/udata.h>
#include <unicode/uversion.h>

extern "C" U_EXPORT const size_t U_ICUDATA_SIZE = {len(buffer)};
extern "C" U_EXPORT const unsigned char U_ICUDATA_ENTRY_POINT[] = {{
	{methods.format_buffer(buffer, 1)}
}};
""")

# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "make_icu_data":
        args.pop(0)
        make_icu_data(args)
