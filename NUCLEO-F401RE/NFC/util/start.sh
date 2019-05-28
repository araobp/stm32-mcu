echo "Starting BLE gateway..."
sudo python3 ./gateway.py -i 0 -d BLE2 -s 11223344-5566-7788-9900-aabbccddeeff -t 01020304-0506-0708-0900-0a0b0c0d0e0f -r 01020304-0506-0708-0900-0a0b0c0deeff &
sleep 1
child_pid=$(ps --ppid $! -o pid=)
echo "sudo kill -9 $!" > ./stop.sh
echo "sudo kill -9 $child_pid" >> ./stop.sh
chmod +x ./stop.sh

