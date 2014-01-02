CCOPT=-DLINUX
CC=gcc -Wall
#CC=/opt/gnu/bin/gcc

all: msga msgb sema semb rpca rpcb forki

clean: 
	rm msg[ab] sem[ab] rpc[ab] forki

msg: msga msgb

msga: msga.c params.h
	$(CC) $(CCOPT) msga.c -o msga

msgb: msgb.c params.h
	$(CC) $(CCOPT) msgb.c -o msgb

sem: sema semb

sema: sema.c params.h
	$(CC) $(CCOPT) sema.c -o sema

semb: semb.c params.h
	$(CC) $(CCOPT) semb.c -o semb

rpc: rpca rpcb

rpca: rpca.c rpcparams.h
	$(CC) $(CCOPT) rpca.c -o rpca -ltirpc

rpcb: rpcb.c rpcparams.h 
	$(CC) $(CCOPT) rpcb.c -o rpcb -ltirpc

forki: forki.c
	$(CC) $(CCOPT) forki.c -o forki

