define($CLIENT_SIDE enp0s8, $SERVER_SIDE enp0s3);

clientFD::FromDevice($CLIENT_SIDE, SNIFFER false, PROMISC true );
serverFD::FromDevice($SERVER_SIDE, SNIFFER false, PROMISC true );

clientCor::Correlator(client);
serverCor::Correlator(server);

preCounter::CoverTraffic(50);
postCounter::checkCoverTraffic();

clientQueue::Queue(100);

/* IP traffic out [0], all other packets out [1] */
ipClassify::Classifier(12/0800, -);

/* TCP traffic out [0], all other IP traffic out [1] */
tcpClassify::IPClassifier(tcp, -);

clientFD -> Queue(100) -> ToDevice($SERVER_SIDE);

serverFD -> ipClassify;

ipClassify[0] -> CheckIPHeader(14) -> tcpClassify;
ipClassify[1] -> clientQueue;

tcpClassify[0] -> serverCor;
tcpClassify[1] -> clientQueue;

serverCor -> preCounter -> clientCor -> postCounter -> clientQueue;

clientQueue -> ToDevice($CLIENT_SIDE);
