#!/usr/bin/env python3

import os
import glob
import argparse

parser = argparse.ArgumentParser(description="Enumerate AMDGPU IPs")
parser.add_argument('--gpu', dest='card_index', nargs='?', type=int, default=0, help="Card index, first by default")
parser.add_argument('--enumerate', action='store_true', help="Enumerate IPs")
parser.add_argument('-u', dest='unique_ips', action='store_true', help="Print only unique IPs (based on name and version)")
args = parser.parse_args()

def detect_card():
    global args
    cards = glob.glob("/sys/class/drm/card[0-9]")
    if len(cards) == 0 or len(cards) - 1 < args.card_index:
        raise SystemExit("No card found!")
    return cards[args.card_index]

def enumerate_ips():
    global args
    card = detect_card();
    ip_discovery = os.path.join(card, "device/ip_discovery/die")
    if not os.path.exists(ip_discovery):
        raise SystemExit("GPU IP discovery not supported! Old kernel?")

    root, dirs, _ = next(os.walk(os.path.join(ip_discovery, "0")))

    ips = []
    for ip_name in dirs:
        ip_path = os.path.join(root, ip_name)
        if(os.path.islink(ip_path)):
            _, ip_instances, _ = next(os.walk(ip_path))
            for current_instance in ip_instances:
                ver_major = [x.strip() for x in open(os.path.join(ip_path, current_instance, "major"), "r").readlines()][0]
                ver_minor = [x.strip() for x in open(os.path.join(ip_path, current_instance, "minor"), "r").readlines()][0]
                ver_revision = [x.strip() for x in open(os.path.join(ip_path, current_instance, "revision"), "r").readlines()][0]
                version = "{}.{}.{}".format(ver_major, ver_minor, ver_revision)
                base_addrs = [x.strip() for x in open(os.path.join(ip_path, current_instance, "base_addr"), "r").readlines()]
                
                for base_addr in base_addrs:
                    current_ip = {'name': ip_name, 'version': version, 'base_addr': base_addr}
                    if not any(ip['name'] == ip_name and ip['version'] == version for ip in ips) or not args.unique_ips: ips.append(current_ip)

    name_maxlen = max(len(x['name']) for x in ips)
    ver_maxlen = max(len(x['version']) for x in ips)

    for ip in sorted(ips, key=lambda d: d['base_addr']):
        print(f"{ip['name']:>{name_maxlen}} {ip['version']:<{ver_maxlen}} {ip['base_addr']}")

# Default action
if args.enumerate or len(args._get_args()) < 1:
    enumerate_ips()
