sudo node ./https_web.js &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" > ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh
chmod +x ./stop.sh

sudo node ./web.js &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" >> ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh

localhost=$(hostname -I)
ip=($localhost)
echo "Open http://"$ip with your browser
