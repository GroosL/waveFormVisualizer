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

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -o $@ $^ $(SDL_LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: RELEASEFLAGS = $(DEBUGFLAGS)
debug: clean $(TARGET)
	lldb ./$(TARGET)

clean:
	@rm -rf $(OBJDIR)/*.o $(BINDIR)/trabalho_comunicacao

.PHONY: all run debug clean help
