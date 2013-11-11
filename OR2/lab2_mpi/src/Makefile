CC        = /usr/lib/openmpi/bin/mpicc
CLINKER   = $(CC)
OPTFLAGS  = -O
HFILES    = 
SOURCE    = cpi.c
OBJS      = $(SOURCE:.c=.o)
NAME      = cpi.exe

CFLAGS	  = $(OPTFLAGS)

$(NAME): $(OBJS) $(HFILES)
	$(CLINKER) $(OPTFLAGS) -o$(NAME) $(OBJS) -lm 

.c.o:
	$(CC) $(CFLAGS) -c $*.c
run:
	/usr/lib/openmpi/bin/mpirun -n 8 $(NAME)

