#/bin/bash

trap 'kill $(jobs -p)' SIGINT

# workloads="./workloads/workloada.spec ../workloads/workloadb.spec ../workloads/workloadd.spec ../workloads/workloadf.spec"
workloads="./workloads/workloadc.spec"
datapath="/mnt/nvmedisk/yiliu/rocksdb/"

for workload in $workloads; do
  echo "Running rocksdb with for $workload"
  sudo rm -r "$datapath"*
  sudo ./ycsbc -db rocksdb -threads 1 -P $workload -path $datapath>./ycsbc.output &
  wait
done

