#! /bin/bash 
function usage() {
  cat << EOT
  create_my_stream.sh - Create My Stream 

Usage:

  createmystream.sh -P {parent stream tag} -C {the stream name which will be created}
  createmyview.sh [-h]

Where:
  -C - the stream name which will be created
  -P - parent stream tag
EOT
  exit 0
}

MY_STREAM_NAME=
MY_PARENT_STREAM_NAME=

while getopts P:C: opt
do
   case $opt in
       P) MY_PARENT_STREAM_NAME="$OPTARG"
       ;;
       C) MY_STREAM_NAME="$OPTARG"
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
    if [ "$MY_PARENT_STREAM_NAME" = "" ]
    then
      usage
    fi
fi

/usr/atria/bin/cleartool mkstream -in $MY_PARENT_STREAM_NAME"@/enba_pvob" $MY_STREAM_NAME"@/enba_pvob"

