# DSP Homework 1: Discrete Hidden Markov Model

override CFLAGS +=
override CXXFLAGS +=
override LDFLAGS +=

all: train_hmm.c test_hmm.c
	gcc train_hmm.c -o train_hmm
	gcc test_hmm.c -o test_hmm

clean:
	$(RM) -f *.o
	$(RM) -rf *.dSYM
	$(RM) -f train_hmm test_hmm

.PHONY: all clean
