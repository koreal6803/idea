@NETLIST = <../../bench/*>;

# generate dir

system "mkdir log" unless(-e "log");

foreach(@NETLIST){
    $_ =~ /\.\.\/\.\.\/bench\/(.*)/;
    $block = $1;
	print $1;
	system "./bin/release/circuit_simulator_test.out ../../bench/$block/${block}_routed.v ../../bench/$block/${block}_routed.sdf  ../../bench/$block/${block}_routed.pat > log/${block}.log";
	print "\n";
}
