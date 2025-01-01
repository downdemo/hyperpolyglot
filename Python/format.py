#!/usr/bin/python3
import argparse
import pangu
import pathlib
import re
import os


def format_cpp(contents: list[str]) -> list[str]:
    tmp_file: str = '/tmp/tmp.cpp'
    os.system(f'touch {tmp_file}')
    with open(tmp_file, 'w') as f:
        f.writelines([f'{_}\n' for _ in contents])
    os.system(f'clang-format -style=Google -i {tmp_file}')
    contents.clear()
    res: list[str] = []
    with open(tmp_file) as f:
        for line in f.readlines():
            line = line.rstrip('\n')
            res.append(line)
    return res


def overwrite_cpp(filename: str):
    res: list[str] = []
    is_code_block: bool = False
    with open(filename, encoding='utf-8') as f:
        cpp_content: list[str] = []
        for line in f.readlines():
            line = line.rstrip('\n')
            if line.startswith('```cpp'):
                is_code_block = True
                res.append(line)
                continue
            if is_code_block and line.startswith('```'):
                is_code_block = False
                res += format_cpp(cpp_content)
                res.append(line)
                continue
            if is_code_block:
                cpp_content.append(line)
            else:
                res.append(line)
    res = [f'{_}\n' for _ in res]
    with open(filename, 'w', encoding='utf-8') as f:
        f.writelines(res)
    print(filename)


def overwrite(filename: str):
    res: list[str] = []
    is_code_block: bool = False
    with open(filename, encoding='utf-8') as f:
        for line in f.readlines():
            line = line.rstrip('\n')
            if line.startswith('```'):
                is_code_block = not is_code_block
            if is_code_block:
                group = re.search(r'(.*?);\s*//\s*(.*)', line)
                if group is not None:
                    line = f'{group.group(1)};  // {pangu.spacing_text(group.group(2))}'
                if line.startswith('//'):
                    line = f'// {pangu.spacing_text(line[2:].lstrip())}'
                res.append(line)
            else:
                """
                e.g [space][space]*
                """
                space_connt: int = len(line) - len(line.lstrip())
                res.append(line[0:space_connt] + pangu.spacing_text(line[space_connt:]))
    res = [f'{_}\n' for _ in res]
    with open(filename, 'w', encoding='utf-8') as f:
        f.writelines(res)
    print(filename)


if __name__ == '__main__':
    os.chdir(pathlib.Path(__file__).parents[1])
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-f',
        required=False,
        type=str,
        default=os.path.join(pathlib.Path(__file__).parents[1], 'Cpp'),
        help='format dir or file',
    )
    args = parser.parse_args()
    root: str = args.f
    if os.path.isdir(root):
        for dirpath, dirnames, filenames in os.walk(root):
            for filename in filenames:
                if '.' not in filename:
                    continue
                suffix = filename.rsplit('.', 1)[1]
                fullname: str = os.path.join(dirpath, filename)
                if suffix == 'md':
                    overwrite_cpp(fullname)
                elif suffix in ['h', 'hpp', 'cpp']:
                    os.system(f'clang-format -style=Google -i {fullname}')
                    print(fullname)
    elif os.path.isfile(root):
        overwrite_cpp(root)
