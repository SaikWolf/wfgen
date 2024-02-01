#!/usr/bin/env python3

import psutil,argparse

def is_proc_running(name):
    for p in psutil.process_iter(['name']):
        try:
            if p.name() == name:
                return True
        except (psutil.ZombieProcess,psutil.NoSuchProcess):
            pass
    return False

def proc_port(name,addr="127.0.10.10"):
    if not is_proc_running(name):
        return []
    pids = []
    for p in psutil.process_iter(['name','pid']):
        if p.name() == name:
            pids.append(p.pid)

    connections = psutil.net_connections()
    ports = []
    for c in connections:
        if c.status == 'LISTEN':
            if c.pid in pids and c.laddr.ip == addr:
                ports.append(c.laddr.port)

    return ports


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument('-type',default='running',type=str,help=('"running" or "port"'))
    p.add_argument("name",default="wav_process.py",type=str,help=("Name of the process to search on"))
    p.add_argument('-addr',default='127.0.10.10',type=str,help=('Address expected to be bound to'))
    return p.parse_args()

if __name__ == "__main__":
    args = parse_args()
    if args.type == 'port':
        for port in proc_port(args.name,args.addr):
            print(port)
    else:
        print(is_proc_running(args.name))


