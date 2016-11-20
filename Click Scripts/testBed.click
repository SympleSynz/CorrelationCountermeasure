define($CLIENT_SIDE enp0s8,$SERVER_SIDE enp0s9);


fd0::FromDevice($CLIENT_SIDE, SNIFFER false, PROMISC true );
fd1::FromDevice($SERVER_SIDE, SNIFFER false, PROMISC true );

clientSideAttacker::AnonAttacker(1);
serverSideAttacker::AnonAttacker(0);

torNetwork::BlackBox();

countermeasure::CountermeasureBox();

pipeline::{serverSideAttacker -> torNetwork ->


fd0 -> q0 -> ToDevice(enp0s9);

fd1 -> serverSideAttacker -> torNetwork -> [0]countermeasure -> clientSideAttacker -> [1]countermeasure -> ToDevice(enp0s8);

clientSideAttacker[0] -> 


/* IP traffic out [0], all other packets out [1] */
ipClassify::Classifier(12/0800, -); 

/* TCP traffic out [0], all other IP traffic out [1] */
tcpClassify::IPClassifier(tcp, -);


q0::Queue(1000);
q1::Queue(1000);
q2::Queue(1000);
q3::Queue(1000);

fd1 -> revq1::Queue; 

fd0->c0;

c0[0]->CheckIPHeader(14) -> c1; /* IP */
c0[1]-> q0; /* Not IP */

/* for dest 9501, with 85% prob, let through unmodified */
c1[0]->  IPPrint("IP-TCP9501") -> tcprst::TCPReseter(85);
tcprst[0]->q1;
tcprst[1]->IPPrint("RST")->revq2::Queue; /* Send RST back */

rrsched::RoundRobinSched;
rrsched -> td0::ToDevice(enp0s8);
revq1->[0]rrsched;
revq2->[1]rrsched;

c1[1]->  q2;
c1[2]->  q3;
c1[3]->  q0;

sched::RoundRobinSched;
q0->[0]sched;
q1->[1]sched;
q2->[2]sched;
q3->[3]sched;

sched -> BandwidthShaper(1000000) -> ToDevice(enp0s9);