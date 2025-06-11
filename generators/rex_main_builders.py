"""Functions used to generate source files during build time"""

import methods
import sys


def make_splash(args):
    buffer = methods.get_buffer(str(args[1]))

    with methods.generated_wrapper(str(args[0])) as file:
        # Use a neutral gray color to better fit various kinds of projects.
        file.write(f"""\
static const Color boot_splash_bg_color = Color(0.14, 0.14, 0.14);
inline constexpr const unsigned char boot_splash_png[] = {{
	{methods.format_buffer(buffer, 1)}
}};
""")


def make_splash_editor(args):
    buffer = methods.get_buffer(str(args[1]))

    with methods.generated_wrapper(str(args[0])) as file:
        # The editor splash background color is taken from the default editor theme's background color.
        # This helps achieve a visually "smoother" transition between the splash screen and the editor.
        file.write(f"""\
static const Color boot_splash_editor_bg_color = Color(0.125, 0.145, 0.192);
inline constexpr const unsigned char boot_splash_editor_png[] = {{
	{methods.format_buffer(buffer, 1)}
}};
""")


def make_app_icon(args):
    buffer = methods.get_buffer(str(args[1]))

    with methods.generated_wrapper(str(args[0])) as file:
        # Use a neutral gray color to better fit various kinds of projects.
        file.write(f"""\
inline constexpr const unsigned char app_icon_png[] = {{
	{methods.format_buffer(buffer, 1)}
}};
""")



# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "make_splash":
        args.pop(0)
        make_splash(args)
    if args[0] == "make_splash_editor":
        args.pop(0)
        make_splash_editor(args)
    if args[0] == "make_app_icon":
        args.pop(0)
        make_app_icon(args)
