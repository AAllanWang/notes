#! /bin/bash

ITEM_INFO=""
ITEM_INFO=`tar -tzf $1 | awk -F "#" '{ print $4; }' | awk -F "_" '{ print $2;}' | grep -E "[A-Z][A-Z0-9]+"`
echo
echo
echo $ITEM_INFO
echo
echo
tar -tzf $1 | sed -n 's/#BI_\([0-9A-F]\{4\}\)#FT_OMsw#FI_\([A-Z][0-9A-Z]*\)#FV_[0-9A-F]\{8\}/\2 hwref=0x\1/p' 
