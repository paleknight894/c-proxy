objects=domain_getfd.o  ipv6_getfd.o     socks.o   ipv4_getfd.o  method_exists.o
socks5-server:$(objects) tcp_forward.o
	cc -o socks5-server $(objects) tcp_forward.o
$(objects):sockstypes.h
tcp_forward.o:
.PHONY:clean
clean:
	  rm socks5-server $(objects) tcp_forward.o
