#!/bin/bash -f

# ---------------------------------
TIMER_WAIT_SECONDS=90
#TIMER_WAIT_SECONDS=5

while true
do
    # ---------------------------------------
    DATETIMESTR=`date +'%Y%m%d%H%M%S'`
    DATESTR=`date +'%Y%m%d'`
    mkdir -p /images/$DATESTR

    # ---------------------------------------
    TARGET_HOST=192.168.20.1
    HOSTNAME=cam0
    curl http://$TARGET_HOST/capture --silent --output /images/$DATESTR/pic$DATETIMESTR-$HOSTNAME.jpg

    # ---------------------------------------
    sleep $TIMER_WAIT_SECONDS
    # ---------------------------------------

done
