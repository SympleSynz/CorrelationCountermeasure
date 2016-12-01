define($SERVER_SIDE enp0s3);

//fd1::FromDevice($SERVER_SIDE, SNIFFER false, PROMISC true );

clientCor::Correlator(client);
serverCor::Correlator(server);

FromDevice($SERVER_SIDE) -> serverCor -> clientCor -> Discard;
