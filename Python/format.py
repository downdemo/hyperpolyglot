#!/usr/bin/python3
import argparse
import datetime
import pangu
import pathlib
import re
import shutil
import tempfile
import os

class Formatter:
    def __init__(self):
        self.tmp_file: str = tempfile.mkstemp(suffix=f"_{datetime.date.today().strftime('%Y%m%d')}.cpp")[1]
        print(f'create {self.tmp_file}')

    def __del__(self):
        os.remove(self.tmp_file)
        print(f'delete {self.tmp_file}')

    def __format_cpp(self, contents: list[str]) -> list[str]:
        with open(self.tmp_file, 'w') as f:
            f.writelines([f'{_}\n' for _ in contents])
        os.system(f'clang-format -style=Google -i {self.tmp_file}')
        contents.clear()
        res: list[str] = []
        with open(self.tmp_file) as f:
            for line in f.readlines():
                line = line.rstrip('\n')
                res.append(line)
        return res

    def format_cpp(self, filename: str):
        if '.' not in os.path.basename(filename):
            return
        suffix = filename.rsplit('.', 1)[1]
        if suffix in ['h', 'hpp', 'cpp']:
            os.system(f'clang-format -style=Google -i {fullname}')
        elif suffix == 'md':
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
                        res += self.__format_cpp(cpp_content)
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

    def format_comment(self, filename: str):
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
    formatter = Formatter()
    if os.path.isdir(root):
        for dirpath, dirnames, filenames in os.walk(root):
            for filename in filenames:
                fullname: str = os.path.join(dirpath, filename)
                formatter.format_cpp(fullname)
    elif os.path.isfile(root):
        formatter.format_comment(root)
