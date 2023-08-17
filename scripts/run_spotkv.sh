#/bin/bash

trap 'kill $(jobs -p)' SIGINT

# workloads="./workloads/workloada.spec ../workloads/workloadb.spec ../workloads/workloadd.spec ../workloads/workloadf.spec"
workloads="./workloads/workloadt.spec"
datapath="/mnt/nvmedisk/yiliu/spotkv/"

for workload in $workloads; do
  echo "Running spotkv with for $workload"
  sudo rm -r "$datapath"*
  sudo ./ycsbc -db spotkv -threads 3 -P $workload -path $datapath>>./ycsbc.output &
  wait
done
