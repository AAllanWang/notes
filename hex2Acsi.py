#!/usr/bin/python
import sys
if __name__ == '__main__':
    strResult = ""
    len = len(sys.argv);
    for i in range(1,len):
        strHex = sys.argv[i]
        strAsci = chr(int(strHex,16))
        strResult = strResult + strAsci
        print strHex,strAsci
    print "Their ASCII codes are"
    print strResult
