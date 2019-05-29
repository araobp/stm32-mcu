# Web app server
sudo node ./webapp/webapp.js &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" > ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh
chmod +x ./stop.sh

# RasPi as a digital signage controller
sudo node ./signage/webapp.js --mode=https &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" >> ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh

# RasPi as a digital signage controller
sudo node ./signage/webapp.js --mode=http &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" >> ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh

# BLE gateway for http-version of video player
sudo python3 ./signage/video_player/ble_gateway/gateway.py -i 0 -d BLE2 -s 11223344-5566-7788-9900-aabbccddeeff -t 01020304-0506-0708-0900-0a0b0c0d0e0f -r 01020304-0506-0708-0900-0a0b0c0deeff &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" >> ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh

localhost=$(hostname -I)
ip=($localhost)
echo "---"
echo "Open http://"$ip" or https://"$ip":444 with your browser"
echo "to start the signage web app, and run signage_controller.py to"
echo "remote-control the signage!"
echo ""
echo "Open https://"$ip"/some_service?loc=debug&dat=shiny&sno=24"
echo "for debugging the web app server"
echo ""
echo "Open https://"$ip"/some_service?loc=kawasaki,todoroki&dat=civic&sno=12"
echo "for testing a menu page"
echo ""

