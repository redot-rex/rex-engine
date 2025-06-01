"""Functions used to generate source files during build time"""

import os

import methods


def make_fonts_header(args):
    arg = args.split(" ")
    print(arg[0])
    with methods.generated_wrapper(str(arg[0])) as file:
        arg.pop(0)
        for src in map(str, arg):
            # Saving uncompressed, since FreeType will reference from memory pointer.
            buffer = methods.get_buffer(src)
            name = os.path.splitext(os.path.basename(src))[0]

            file.write(f"""\
inline constexpr int _font_{name}_size = {len(buffer)};
inline constexpr unsigned char _font_{name}[] = {{
	{methods.format_buffer(buffer, 1)}
}};

""")

# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "make_fonts_header":
        args.pop(0)
        build_raw_headers(args)
