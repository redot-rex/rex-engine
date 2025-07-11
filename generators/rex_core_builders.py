"""Functions used to generate source files during build time"""

from collections import OrderedDict
from io import TextIOWrapper

import os
import sys
import methods


def version_hash_builder(args):
    git = methods.get_git_info()
    with methods.generated_wrapper(str(args[0])) as file:
        file.write(
            f"""\
#include "core/version.h"

const char *const REDOT_VERSION_HASH = "{git['git_hash']}";
const uint64_t REDOT_VERSION_TIMESTAMP = {git['git_timestamp']};
""")


def encryption_key_builder(args):
    source = os.environ.get("SCRIPT_AES256_ENCRYPTION_KEY")
    if source:
        src = source
    else:
        src = "0" * 64
    try:
        buffer = bytes.fromhex(src)
        if len(buffer) != 32:
            raise ValueError
    except ValueError:
        methods.print_error(
            f'Invalid AES256 encryption key, not 64 hexadecimal characters: "{src}".\n'
            "Unset `SCRIPT_AES256_ENCRYPTION_KEY` in your environment "
            "or make sure that it contains exactly 64 hexadecimal characters."
        )
        raise

    with methods.generated_wrapper(str(args[0])) as file:
        file.write(
            f"""\
#include "core/config/project_settings.h"

uint8_t script_encryption_key[32] = {{
	{methods.format_buffer(buffer, 1)}
}};"""
        )


def make_certs_header(args):
    print(args)
    buffer = methods.get_buffer(str(args[1]))
    decomp_size = len(buffer)
    buffer = methods.compress_buffer(buffer)

    with methods.generated_wrapper(str(args[0])) as file:
        # System certs path. Editor will use them if defined. (for package maintainers)
        if len(args) > 3:
            file.write(f'#define _SYSTEM_CERTS_PATH "{args[3]}"\n')
        else:
            file.write(f'#define _SYSTEM_CERTS_PATH ""\n')
        if args[2] == 'ON':
            # Defined here and not in env so changing it does not trigger a full rebuild.
            file.write(f"""\
#define BUILTIN_CERTS_ENABLED

inline constexpr int _certs_compressed_size = {len(buffer)};
inline constexpr int _certs_uncompressed_size = {decomp_size};
inline constexpr unsigned char _certs_compressed[] = {{
	{methods.format_buffer(buffer, 1)}
}};
""")


def make_redot_authors_header(args):
    SECTIONS = {
        "Project Founders": "REDOT_AUTHORS_FOUNDERS",
        "Lead Developer": "REDOT_AUTHORS_LEAD_DEVELOPERS",
        "Project Manager": "REDOT_AUTHORS_PROJECT_MANAGERS",
        "Developers": "REDOT_AUTHORS_DEVELOPERS",
    }
    buffer = methods.get_buffer(str(args[1]))
    reading = False

    with methods.generated_wrapper(str(args[0])) as file:

        def close_section():
            file.write("\tnullptr,\n};\n\n")

        for line in buffer.decode().splitlines():
            if line.startswith("    ") and reading:
                file.write(f'\t"{methods.to_escaped_cstring(line).strip()}",\n')
            elif line.startswith("## "):
                if reading:
                    close_section()
                    reading = False
                section = SECTIONS[line[3:].strip()]
                if section:
                    file.write(f"inline constexpr const char *{section}[] = {{\n")
                    reading = True

        if reading:
            close_section()


def make_authors_header(args):
    SECTIONS = {
        "Project Founders": "AUTHORS_FOUNDERS",
        "Lead Developer": "AUTHORS_LEAD_DEVELOPERS",
        "Project Manager": "AUTHORS_PROJECT_MANAGERS",
        "Developers": "AUTHORS_DEVELOPERS",
    }
    buffer = methods.get_buffer(str(args[1]))
    reading = False

    with methods.generated_wrapper(str(args[0])) as file:

        def close_section():
            file.write("\tnullptr,\n};\n\n")

        for line in buffer.decode().splitlines():
            if line.startswith("    ") and reading:
                file.write(f'\t"{methods.to_escaped_cstring(line).strip()}",\n')
            elif line.startswith("## "):
                if reading:
                    close_section()
                    reading = False
                section = SECTIONS[line[3:].strip()]
                if section:
                    file.write(f"inline constexpr const char *{section}[] = {{\n")
                    reading = True

        if reading:
            close_section()


def make_donors_header(args):
    SECTIONS = {
        "Patrons": "DONORS_PATRONS",
        "Platinum sponsors": "DONORS_SPONSORS_PLATINUM",
        "Gold sponsors": "DONORS_SPONSORS_GOLD",
        "Silver sponsors": "DONORS_SPONSORS_SILVER",
        "Diamond members": "DONORS_MEMBERS_DIAMOND",
        "Titanium members": "DONORS_MEMBERS_TITANIUM",
        "Platinum members": "DONORS_MEMBERS_PLATINUM",
        "Gold members": "DONORS_MEMBERS_GOLD",
    }
    buffer = methods.get_buffer(str(args[1]))
    reading = False

    with methods.generated_wrapper(str(args[0])) as file:

        def close_section():
            file.write("\tnullptr,\n};\n\n")

        for line in buffer.decode().splitlines():
            if line.startswith("    ") and reading:
                file.write(f'\t"{methods.to_escaped_cstring(line).strip()}",\n')
            elif line.startswith("## "):
                if reading:
                    close_section()
                    reading = False
                section = SECTIONS.get(line[3:].strip())
                if section:
                    file.write(f"inline constexpr const char *{section}[] = {{\n")
                    reading = True

        if reading:
            close_section()


def make_license_header(args):
    src_copyright = str(args[1])
    src_license = str(args[2])

    class LicenseReader:
        def __init__(self, license_file: TextIOWrapper):
            self._license_file = license_file
            self.line_num = 0
            self.current = self.next_line()

        def next_line(self):
            line = self._license_file.readline()
            self.line_num += 1
            while line.startswith("#"):
                line = self._license_file.readline()
                self.line_num += 1
            self.current = line
            return line

        def next_tag(self):
            if ":" not in self.current:
                return ("", [])
            tag, line = self.current.split(":", 1)
            lines = [line.strip()]
            while self.next_line() and self.current.startswith(" "):
                lines.append(self.current.strip())
            return (tag, lines)

    projects = OrderedDict()
    license_list = []

    with open(src_copyright, "r", encoding="utf-8") as copyright_file:
        reader = LicenseReader(copyright_file)
        part = {}
        while reader.current:
            tag, content = reader.next_tag()
            if tag in ("Files", "Copyright", "License"):
                part[tag] = content[:]
            elif tag == "Comment" and part:
                # attach non-empty part to named project
                projects[content[0]] = projects.get(content[0], []) + [part]

            if not tag or not reader.current:
                # end of a paragraph start a new part
                if "License" in part and "Files" not in part:
                    # no Files tag in this one, so assume standalone license
                    license_list.append(part["License"])
                part = {}
                reader.next_line()

    data_list = []
    for project in iter(projects.values()):
        for part in project:
            part["file_index"] = len(data_list)
            data_list += part["Files"]
            part["copyright_index"] = len(data_list)
            data_list += part["Copyright"]

    with open(src_license, "r", encoding="utf-8") as file:
        license_text = file.read()

    with methods.generated_wrapper(str(args[0])) as file:
        file.write(f"""\
inline constexpr const char *GODOT_LICENSE_TEXT = {{
{methods.to_raw_cstring(license_text)}
}};

struct ComponentCopyrightPart {{
	const char *license;
	const char *const *files;
	const char *const *copyright_statements;
	int file_count;
	int copyright_count;
}};

struct ComponentCopyright {{
	const char *name;
	const ComponentCopyrightPart *parts;
	int part_count;
}};

""")

        file.write("inline constexpr const char *COPYRIGHT_INFO_DATA[] = {\n")
        for line in data_list:
            file.write(f'\t"{methods.to_escaped_cstring(line)}",\n')
        file.write("};\n\n")

        file.write("inline constexpr ComponentCopyrightPart COPYRIGHT_PROJECT_PARTS[] = {\n")
        part_index = 0
        part_indexes = {}
        for project_name, project in iter(projects.items()):
            part_indexes[project_name] = part_index
            for part in project:
                file.write(
                    f'\t{{ "{methods.to_escaped_cstring(part["License"][0])}", '
                    + f"&COPYRIGHT_INFO_DATA[{part['file_index']}], "
                    + f"&COPYRIGHT_INFO_DATA[{part['copyright_index']}], "
                    + f"{len(part['Files'])}, {len(part['Copyright'])} }},\n"
                )
                part_index += 1
        file.write("};\n\n")

        file.write(f"inline constexpr int COPYRIGHT_INFO_COUNT = {len(projects)};\n")

        file.write("inline constexpr ComponentCopyright COPYRIGHT_INFO[] = {\n")
        for project_name, project in iter(projects.items()):
            file.write(
                f'\t{{ "{methods.to_escaped_cstring(project_name)}", '
                + f"&COPYRIGHT_PROJECT_PARTS[{part_indexes[project_name]}], "
                + f"{len(project)} }},\n"
            )
        file.write("};\n\n")

        file.write(f"inline constexpr int LICENSE_COUNT = {len(license_list)};\n")

        file.write("inline constexpr const char *LICENSE_NAMES[] = {\n")
        for license in license_list:
            file.write(f'\t"{methods.to_escaped_cstring(license[0])}",\n')
        file.write("};\n\n")

        file.write("inline constexpr const char *LICENSE_BODIES[] = {\n\n")
        for license in license_list:
            to_raw = []
            for line in license[1:]:
                if line == ".":
                    to_raw += [""]
                else:
                    to_raw += [line]
            file.write(f"{methods.to_raw_cstring(to_raw)},\n\n")
        file.write("};\n\n")


# Allows CMake to call these functions using args
if __name__ == "__main__":
    args = sys.argv
    args.pop(0)
    if args[0] == "version_hash_builder":
        args.pop(0)
        version_hash_builder(args)
    if args[0] == "encryption_key_builder":
        args.pop(0)
        encryption_key_builder(args)
    if args[0] == "make_certs_header":
        args.pop(0)
        make_certs_header(args)
    if args[0] == "make_redot_authors_header":
        args.pop(0)
        make_redot_authors_header(args)
    if args[0] == "make_authors_header":
        args.pop(0)
        make_authors_header(args)
    if args[0] == "make_donors_header":
        args.pop(0)
        make_donors_header(args)
    if args[0] == "make_license_header":
        args.pop(0)
        make_license_header(args)

