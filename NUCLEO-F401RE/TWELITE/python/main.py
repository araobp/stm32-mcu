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

### Argument parser
DESCRIPTION = '''
TWELITE module test utility.
'''
parser = argparse.ArgumentParser(description=DESCRIPTION)
parser.add_argument("port", help="Serial port identifier")

args = parser.parse_args()

if __name__ == '__main__':

    with tw.MasterNode(args.port, BAUDRATE) as mn:
        while True:
            try:
                data = mn.read(quality_data=False)
                #print(data, data.decode('utf-8'))
                print(util.to_int8_array(data))
            except tw.TweliteException as e:
                print(e)
                #traceback.print_exc()
                #pass

