CFLAGS=-std=c11 -g -Wall -Wunused-parameter

SRCS=$(wildcard *.c)
HEADS=$(wildcard *.h)
OBJS=$(SRCS:.c=.o)

TEST_SRC=test/test.c
TEST_HEAD=test/test_dat.h
TEST_OBJS=$(TEST_SRC:.c=.o)
TEST=$(TEST_SRC:.c=)

$(OBJS): $(SRCS) $(HEADS)
$(TEST_OBJS): $(TEST_SRC) $(TEST_HEAD) $(HEADS)

$(TEST): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -I. -o $@ $^ $(LDFLAGS)

test: $(TEST)
	$(TEST)

clean:
	rm -f $(OBJS) $(TEST) a.out

.PHONY: clean test
