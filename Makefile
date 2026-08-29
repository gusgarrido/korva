SRCDIR := src
TOOLSDIR := tools
TESTDIR := tests
BUILDDIR := build

CC := cc
INCLUDES := $(addprefix -I,$(SRCDIR))
CFLAGS := -std=c99 -O2 -Wall -g $(INCLUDES)

SRCS := $(shell find $(SRCDIR) -type f -name '*.c')
OBJS := $(patsubst %,$(BUILDDIR)/%,$(SRCS:.c=.o))

REPL_TARGET := build/target/repl
REPL_SRCS := $(shell find $(TOOLSDIR)/repl -type f -name '*.c')
REPL_OBJS := $(patsubst %,$(BUILDDIR)/%,$(REPL_SRCS:.c=.o))

TEST_SRCS := $(shell find $(TESTDIR) -type f -name 'test_*.c')
TEST_BINS := $(patsubst $(TESTDIR)/%.c,$(BUILDDIR)/tests/%,$(TEST_SRCS))
TEST_LIBS := $(shell find $(TESTDIR) -type f -name '*.c' ! -name 'test_*.c' )
TEST_OBJS := $(patsubst %,$(BUILDDIR)/%,$(TEST_LIBS:.c=.o))
TEST_INCLUDES := $(addprefix -I,$(TESTDIR)/unity)
TEST_CFLAGS := $(CFLAGS) $(TEST_INCLUDES)

build: $(OBJS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

repl: $(REPL_TARGET)

$(REPL_TARGET): $(OBJS) $(REPL_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

tests: $(TEST_BINS)

$(BUILDDIR)/tests/%: $(TESTDIR)/%.c $(OBJS) $(TEST_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -o $@ $< $(OBJS) $(TEST_OBJS) 

$(BUILDDIR)/tests/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR)

.PHONY: build repl tests clean
