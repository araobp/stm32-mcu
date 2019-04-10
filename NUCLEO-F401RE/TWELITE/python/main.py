'''
 "twelite" module test utility.

 Example usage:
 $ python main.py COM9 2 1 -q

'''
import argparse
import sys
import traceback

import twelite as tw
import util

### Serial port setting in bps
BAUDRATE = 115200

INT8_T = ord('i')
INT16_T = ord('l')
ASCII = ord('a')

### Argument parser
DESCRIPTION = '''
TWELITE module test utility.
'''
parser = argparse.ArgumentParser(description=DESCRIPTION)
parser.add_argument("dst", type=int, help="destination ID")
parser.add_argument("port", help="Serial port identifier")
parser.add_argument("-t", "--type", default="int8_t", help="Type")

args = parser.parse_args()

if __name__ == '__main__':

    with tw.MasterNode(args.port, BAUDRATE) as mn:
        try:
            if args.type == 'int8_t':
                data = mn.fetch(dst=args.dst, cmd=INT8_T, quality_data=False)
                print(util.to_int8_array(data))
            elif args.type == 'int16_t':
                data = mn.fetch(dst=args.dst, cmd=INT16_T, quality_data=False)
                print(util.to_int16_array(data))
            elif args.type == 'ascii':
                data = mn.fetch(dst=args.dst, cmd=ASCII, quality_data=False)
                print(data.decode('ascii'))
        except tw.TweliteException as e:
            print(e)
            #traceback.print_exc()
            #pass

