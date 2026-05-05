CXX = c++
CXXFLAGS = -std=c++20 -Wall -Wextra -I./include
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O3

SDL_LIBS = -lSDL3 -lSDL3_ttf

SRCDIR = src
OBJDIR = build
BINDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/trabalho_comunicacao

IS_NIXOS := $(shell grep -qi nixos /etc/os-release && echo 1 || echo 0)

ifeq ($(IS_NIXOS),1)
    RUN_CMD = ./$(TARGET)
else
    RUN_CMD = nixGL ./$(TARGET)
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -o $@ $^ $(SDL_LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN_CMD)

debug: RELEASEFLAGS = $(DEBUGFLAGS)
debug: clean $(TARGET)
	lldb ./$(TARGET)

clean:
	@rm -rf $(OBJDIR)/*.o $(BINDIR)/trabalho_comunicacao

.PHONY: all run debug clean help
