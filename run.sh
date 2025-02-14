#!bin/bash
taskset --cpu-list 2 ./bs4 512 $1
taskset --cpu-list 2 ./bs4 1024 $1
taskset --cpu-list 2 ./bs4 2048 $1
taskset --cpu-list 2 ./bs4 4096 $1
taskset --cpu-list 2 ./bs4 8192 $1
taskset --cpu-list 2 ./bs4 16384 $1
taskset --cpu-list 2 ./bs4 32768 $1
taskset --cpu-list 2 ./bs4 65536 $1
taskset --cpu-list 2 ./bs4 131072 $1
taskset --cpu-list 2 ./bs4 262144 $1
taskset --cpu-list 2 ./bs4 524288 $1
taskset --cpu-list 2 ./bs4 1048576 $1
taskset --cpu-list 2 ./bs4 2097152 $1
taskset --cpu-list 2 ./bs4 4194304 $1
taskset --cpu-list 2 ./bs4 8388608 $1
taskset --cpu-list 2 ./bs4 16777216 $1