#!/usr/bin/env python3

import subprocess 
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("port", help="Port")
parser.add_argument("loc", help="Location")
args = parser.parse_args()

local_ip_addr = subprocess.check_output(['hostname', '-I']).decode('utf-8').split(' ')[0].rstrip(' \n') 

base_url = '{}:{}/some_service?loc={}\n'.format(local_ip_addr, args.port, args.loc)

print(base_url, end='')

