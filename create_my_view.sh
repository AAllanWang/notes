#! /bin/bash 
function usage() {
  cat << EOT
  createmyview.sh - Create My View

Usage:

  createmyview.sh -v {view tag} -s {the stream name where the view will be created}
  createmyview.sh [-h]

Where:
  -s - the stream name where the view will be created
  -v - view tag
EOT
  exit 0
}

MY_STREAM_NAME=
MY_VIEW_TAG=

while getopts s:v: opt
do
   case $opt in
       s) MY_STREAM_NAME="$OPTARG"
       ;;
       v) MY_VIEW_TAG="$OPTARG"
       ;;
       '?') usage;;
   esac
done

# remove options from arg list (if any)
shift $(($OPTIND - 1))

if [ $# -gt 0 ]
then
    echo "Error: Wrong number of arguments"
    usage
fi

if [ "$MY_STREAM_NAME" = "" ]
then
  usage
  else
    if [ "$MY_VIEW_TAG" = "" ]
    then
      usage
    fi
fi

if [ ! -d /ucm/view_store_LTE/$USER ]; then
  mkdir /ucm/view_store_LTE/$USER
fi

/usr/atria/bin/cleartool mkview -tag $MY_VIEW_TAG -stream $MY_STREAM_NAME@/enba_pvob -host bjltelncs38 -hpath /ucm/view_store_LTE/$USER/$MY_VIEW_TAG.vws -gpath /ucm/view_store_LTE/$USER/$MY_VIEW_TAG.vws /ucm/view_store_LTE/$USER/$MY_VIEW_TAG.vws

