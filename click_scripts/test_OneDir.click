define($SERVER_SIDE enp0s9);

fd1::FromDevice($SERVER_SIDE, SNIFFER false, PROMISC true );

fd1 -> Print("Reverse") -> Queue(100) -> ToDevice($SERVER_SIDE);


FromHost() -> 