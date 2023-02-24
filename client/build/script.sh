cmake ..
make
./client

return_code=$?
status="exit code: ${return_code}" 
echo $status