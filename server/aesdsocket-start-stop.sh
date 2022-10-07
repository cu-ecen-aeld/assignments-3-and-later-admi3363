#!/bin/sh

#start
#located in /etc/init.d/rcs
#S = startup runlevel

#stop
#/etc/init.d/rcK
#K = kill or shutdown level

#/etc/init.d/Saesdsocket start
#/etc/init.d/Saesdsocket stop

case "$1" in
    start)
        echo "Starting aesdsocket"
        start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket
        ;;
    stop)
        echo "Stoping aesdsocket"
        start-stop-daemon -K -n aesdsocket
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac

exit 0