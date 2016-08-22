#-*-coding:utf8;-*-

"""
中文
"""

import urllib2
import httplib
import time
import re
import os
import platform
import thread

if platform.system() == "Windows":
    savePath = u"D:\\downloaded\\"
else:
    savePath = u"\\scard\\downloaded\\"

mainurl = "http://18h.mm-cg.com/"
num = 444

def dosth(n = 400):
    print "num:" + str(n)
    url = mainurl + "18H_" + str(n) + ".html"
    print "url:" + url
    headers = {
        'Connection':'keep-alive',
                    'Cache-Control':'max-age=0',
                    'Accept': 'text/html, */*; q=0.01',
                    'X-Requested-With': 'XMLHttpRequest',
                    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.89 Safari/537.36',
                    'DNT':'1',
                    'Referer': 'http://18h.mm-cg.com/',
        }
    try:
        request = urllib2.Request(url, headers = headers)
        response = urllib2.urlopen(request)
        if response:
            content = response.read()
            title = getTitle(content)
            if title == None:
                print "error"
                return
            sPath = savePath + _cn(title) + u"\\"
            if not os.path.exists(sPath):
                os.makedirs(sPath)
            pl = getIMGlist(content)
            for p in pl:
                time.sleep(0.1)
                print "download:" + p
                pname = os.path.split(p)
                pPath = sPath + _cn(pname[1])
                pd = urllib2.Request(p, headers = headers)
                pdd = urllib2.urlopen(pd)
                if(pdd):
                    if os.path.exists(pPath) and os.path.getsize(pPath) == 0:
                        os.remove(pPath)
                    if not os.path.exists(pPath):
                        f = open(pPath, "w")
                        f.write(pdd.read())
                        f.close()
            
                print "download complate:" + p
    except Exception,e:
        print Exception,":",e

def getTitle(data):
    p = re.compile(r'<title>([^<]*)</title>')
    m = p.findall(data)
    if m:
        return m[0]
    return None

def getIMGlist(data):
    p = re.compile(r'Large_cgurl\[[1-9]*\] = "([^"]*)";')
    m = p.findall(data)
    if m:
        return m

def _cn(s):
    return s.decode("utf8")

def _nc(s):
    return s.encode("utf8")

def start():
    for i in range(400,510):
        thread.start_new_thread(dosth,(i,))

def test(n):
    for i in range(0,10):
        print str(n) + ":" + str(i)

def main():
    start()

main()