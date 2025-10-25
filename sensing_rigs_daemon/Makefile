# Define project's directories
BDIR := ./build
IDIR := ./include
SDIR := ./src
RDIR := ./run

# Define used compiler and its options
CC := gcc
IFLAGS := -I$(IDIR)
CFLAGS := -Wall -Wextra

#Define project executable
EXEC := sensing_rigs_daemon
# Define the installation path
IPATH := /usr/local/bin

# Automatically find source files & header files
SRCS := $(wildcard $(SDIR)/*.c)
DEPS := $(wildcard $(IDIR)/*.h)
# Automatically define object files
OBJS := $(patsubst $(SDIR)/%.c,$(BDIR)/%.o,$(SRCS))

.DELETE_ON_ERROR:

# Define the executable as default target
all: $(EXEC)

# Compile every source file inside ./src placing the object files in ./build
$(BDIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(BDIR)
	@mkdir -p $(RDIR)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Define the rule for linking object files into the executable
$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ $(OBJS)

# Specify which targets are not rules
.PHONY: all clean install uninstall

# Define a rule to clean up the build files
clean:
	@rm -rf $(BDIR) ./$(EXEC)
	@echo "Cleaned build files!"

# Define a rule to install the daemon
install:
	@mkdir -p $(IPATH)
	@sudo cp $(EXEC) $(IPATH)
	@sudo chmod 755 $(IPATH)/$(EXEC)
	@echo "Daemon successfully installed to $(IPATH)!"

# Define a rule to uninstall the daemon
uninstall:
	@sudo rm -f $(IPATH)/$(EXEC)
	@echo "Daemon successfully uninstalled!"
