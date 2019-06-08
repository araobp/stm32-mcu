# Web app server
node ./webapp/webapp.js &
sleep 1
echo "kill -9 $!" > ./stop.sh
chmod +x ./stop.sh

# Home appliances
node ./home_appliances/webapp.js &
sleep 1
echo "kill -9 $!" >> ./stop.sh

localhost=$(hostname -I)
ip=($localhost)
echo "---"
echo "Open https://"$ip":10444 with your browser for signage apps."
echo ""
echo "Open https://"$ip":10443/some_service?loc=debug&dat=shiny&sno=24"
echo "for debugging the web app server"
echo ""

