
INC = -I..
LIB =

DEBUG=

FLG= -Wno-deprecated -Wno-non-template-friend -g -O0

CC = g++ ${DEBUG} -Wall -DLINGER


BIN = example
OBJ = main.o time_wheel.o

all: $(BIN)

$(BIN): ${OBJ}
	$(CC) $(FLG) -o $@ $(INC) $^ $(LIB)

time_wheel.o: ../time_wheel.cpp
	$(CC) $(FLG) $(INC) -c -o $@ $<

%.o: %.cpp 
	$(CC) $(FLG) $(INC) -c -o $@ $<
	
clean:
	rm *.o example
	
