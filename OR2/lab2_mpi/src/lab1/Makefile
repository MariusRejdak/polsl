CC		= mpicc
CLINKER   = $(CC)
OPTFLAGS  = -O
HFILES	= 
SOURCE	= systest.c
OBJS	  = $(SOURCE:.c=.o)
NAME	  = systest.out

CFLAGS	  = $(OPTFLAGS) -std=c99

$(NAME): $(OBJS) $(HFILES)
	$(CLINKER) $(OPTFLAGS) -o$(NAME) $(OBJS) -lm 

.c.o:
	$(CC) $(CFLAGS) -c $*.c
run:
	mpirun -n 8 $(NAME)
