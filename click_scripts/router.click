define($SERVER_SIDE enp0s3, $CLIENT_SIDE enp0s8);

fd0::FromDevice($CLIENT_SIDE, SNIFFER false, PROMISC true );
fd1::FromDevice($SERVER_SIDE, SNIFFER false, PROMISC true );

fd0 -> Queue(1000) -> ToDevice($SERVER_SIDE);
fd1 -> Queue(1000) -> ToDevice($CLIENT_SIDE);