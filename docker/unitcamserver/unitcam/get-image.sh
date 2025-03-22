#!/bin/bash -f

# ---------------------------------
TIMER_WAIT_SECONDS=100
#TIMER_WAIT_SECONDS=5

while true
do
    # ---------------------------------------
    DATETIMESTR=`date +'%Y%m%d%H%M%S'`
    DATESTR=`date +'%Y%m%d'`
    mkdir -p /images/$DATESTR

    # ---------------------------------------
    TARGET_HOST=192.168.19.95
    HOSTNAME=cam1
    curl http://$TARGET_HOST/capture --silent --output /images/$DATESTR/pic$DATETIMESTR-$HOSTNAME.jpg

    # ---------------------------------------
    TARGET_HOST=192.168.19.94
    HOSTNAME=cam2
    curl http://$TARGET_HOST/capture --silent --output /images/$DATESTR/pic$DATETIMESTR-$HOSTNAME.jpg

    # ---------------------------------------
    TARGET_HOST=192.168.19.93
    HOSTNAME=cam3
    curl http://$TARGET_HOST/capture --silent --output /images/$DATESTR/pic$DATETIMESTR-$HOSTNAME.jpg

    # ---------------------------------------
    sleep $TIMER_WAIT_SECONDS
    # ---------------------------------------

done

