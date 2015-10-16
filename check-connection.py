#!/usr/bin/env python


import requests, re
from subprocess import Popen, PIPE
from time import sleep
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool



###  record: (host, max_latancy, raindow color (from 0 to 5))
hosts =  [("provider-gateway", 3, 5), ("server-ip", 80, 4), ("foreign-public-server", 4, 1), ("public-server", 40, 2) ]

url="http://192.168.1.5/"

raindow = [ "8f00ff", "0000ff", "00ffff", "ffff00", "ff4a00", "ff0000"]

timeDelay = 15 # seconds between requests

############################################################

color = 0
prevColor = 0


def send(data, period):

    try:
        k = requests.get(url + 'set', params=data)
    except:
        pass

    sleep(period)
    

def checkHost(host):
    
    global color

    ret = Popen("""ping -c 10 -i 0.2 -q -W 4 {0}| tail -n 2""".format(host[0]), shell=True, stdout=PIPE, stderr=PIPE)
    
    # Get packets lost
    try:
        lost = re.search(r'(\d+)% packet loss', ret.stdout.readline().rstrip()).group(1)
        if int(lost) > 20:
          if color < host[2]:
             color = host[2]
             print host[0] + ' ' + lost
    except:
        pass
        
    # Get latency    
    try:    
        latency = ret.stdout.readline().rstrip().split('/')[4]
        if float(latency) > (2.0 * host[1]):
          if color < host[2]:
             color = host[2]   
             print host[0] + ' lat: ' + latency 
    except:
        pass
        


while True:

    pool = ThreadPool(10)    
    pool.map(checkHost, hosts)
    pool.close()
    pool.join()
    
    print color

    if color != prevColor:
        payloadOff = {'rgb': '000000'}
        send(payloadOff, 1.5)

    payload = {'rgb': raindow[color]}
    send(payload, 3)

    prevColor = color;
    color = 0
    print
                        
    sleep(timeDelay)
    
    