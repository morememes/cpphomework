#!/bin/bash

#
# Params for tester: message(char*), max_clients(int), iters(int), shift(int), block(bool).
#
# message - сообщение, которое клиенты спамят серверу.
# max_clients - максимальное количество клиентов, которое будет достигнуто в цикле.
# iters - количество сообщений, отправленных каждым клиентом.
# shift - количество клиентов, которое будет добавленно в следующей итерации теста.
# block - тип сервер. Нужен для остановки сервера с блокируемым вводом/выводом
#

echo "Starting..."
./build/src/myproject/unblock_server/unblockserver &
./build/src/myproject/client_tester/tester "Hello, there!" 400 25 100 0
wait
./build/src/myproject/block_server/blockserver &
./build/src/myproject/client_tester/tester "Hello, there!" 40 25 10 1
echo "Finished"