CFLAGS=-std=c11 -g -Wall -Wunused-parameter
LDFLAGS+=-lpcre2-8

SRCS=$(wildcard *.c)
HEADS=$(wildcard *.h)
OBJS=$(SRCS:.c=.o)

TEST_SRCS=$(wildcard test/test*.c)
TEST_HEADS=$(wildcard test/test*.h) 
TEST_OBJS=$(TEST_SRCS:.c=.o)
TEST=test/test

$(OBJS): $(SRCS) $(HEADS)
$(TEST_OBJS): $(TEST_SRCS) $(TEST_HEADS) $(HEADS)

$(TEST): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -I. -o $@ $^ $(LDFLAGS)

test: $(TEST)
	$(TEST)

clean:
	rm -f $(OBJS) $(TEST) a.out

.PHONY: clean test
