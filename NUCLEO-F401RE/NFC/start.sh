# Web app server
sudo node ./webapp/webapp.js &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" > ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh
chmod +x ./stop.sh

# RasPi as a digital signage controller
sudo node ./webapp/signage/webapp.js &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" >> ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh

localhost=$(hostname -I)
ip=($localhost)
echo "Open http://"$ip with your browser to start the signage web app
echo "and run signage_controller.py to remote-control the signage!"
