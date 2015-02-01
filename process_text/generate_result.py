#!/usr/bin/env python
#
#

import sys
import re


def generate_label(file_name):
    front, file_type = file_name.split('.')
    front, p_name, m_name = front.split('_')

    return p_name, m_name


def main(argv):
    if len(argv) != 4:
        print 'Error parameters'
        return

    # read parameters
    base_path = argv[1]
    file_list = argv[2]
    dest_file = argv[3]

    # read list
    with open(file_list, 'r') as f:
        lines = f.readlines()

    # regular expression
    pattern = re.compile(r'.*l2\.overall_miss_rate::total *')
    pat_res = re.compile(r'0\.\d*')

    # result content
    res_content = ''

    # process each file
    for line in lines:
        file_name, tail = line.split('\n')
        p_name, m_name = generate_label(file_name)

        with open(base_path + file_name, 'r') as f:
            content = f.readlines()

        # find the result lines
        for line_content in content:
            m = pattern.match(line_content)

            if m:
                print line_content
                
                temp_str = pattern.split(line_content)
                print temp_str[1]
                
                res_m = pat_res.match(temp_str[1])
                res_vaule = res_m.group()
                print res_vaule

                res_content = res_content + p_name + '-' + m_name + ': ' + res_vaule + '\n'
        
        res_content = res_content + '\n'

    # write result
    with open(dest_file, 'w') as f:
        f.writelines(res_content)


if __name__ == '__main__':
    main(sys.argv)
