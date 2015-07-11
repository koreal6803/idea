cmd="./bin/release/circuit_simulator_test.out ../../bench_nan45/lib/NangateOpenCellLibrary_typical_conditional_ccs.lib ../../bench_compression/"$1"/"$1"_routed.v ../../bench_compression/"$1"/"$1"_routed.sdf ../../bench_compression/"$1"/"$1"_basicScan.pat" 
echo $cmd;
eval $cmd;
