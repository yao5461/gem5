#!/usr/bin/env python

import re
import sys

def main(argv):
    if len(argv) != 3:
        print 'Error parameters!'
        return 

    in_file_name = argv[1]
    out_file_name = argv[2]

    # read content
    with open(in_file_name, 'r') as f:
        raw_lines = f.readlines()

    # pattern
    pattern1 = re.compile(r'-{10}.')
    pattern2 = re.compile(r'system\.cpu\d\.\Scache\.overall.*::total.')
    pattern3 = re.compile(r'system\.l2\.overall.*::total.*')

    # result content
    content = ''

    for line in raw_lines:
        if pattern1.match(line) or pattern2.match(line) or pattern3.match(line):
            content = content + line
    
    # write file
    with open(out_file_name, 'w') as f:
        f.writelines(content)

if __name__ == '__main__':
    main(sys.argv)
