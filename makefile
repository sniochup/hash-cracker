NODES = 13
HASH = "aa7uckMQPpN46"
LETTER_NUM = 5
EXEC = cracker
TEXT = "github"

build:
	mpicc cracker.c -lcrypt -o $(EXEC)

encrypt:
	mpicc encrypter.c -lcrypt -o encrypter
	./encrypter $(TEXT)
	rm encrypter

run: build
	mpirun --oversubscribe -np $(NODES) --hostfile hosts --map-by node $(EXEC) $(LETTER_NUM) $(HASH)
	rm $(EXEC)