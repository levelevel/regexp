CFLAGS=-std=c11 -g -Wall

SRCS=regexp.c
OBJS=$(SRCS:.c=.o)
HEADS=regexp.h

TEST_SRC=test/test.c
TEST=$(TEST_SRC:.c=)

$(OBJS): $(SRCS) $(HEADS)

$(TEST): $(TEST_SRC) $(OBJS)
	$(CC) $(CFLAGS) -I. -o $@ $^ $(LDFLAGS)

test: $(TEST)
	$(TEST)

clean:
	rm -f $(OBJS)

.PHONY: clean test
