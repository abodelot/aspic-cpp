TARGET  := aspic
SRCDIR  := src
SRC     := $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJDIR  := obj
OBJ     := $(SRC:%.cpp=$(OBJDIR)/%.o)
DEP     := $(SRC:%.cpp=$(OBJDIR)/%.d)

CC      := g++
CFLAGS  := -MMD -MP -I$(SRCDIR) -std=c++11 -pedantic -Wall -Wextra -Wwrite-strings -Wuseless-cast -O2
LDFLAGS := -lreadline

C_GREEN  := \033[1;32m
C_YELLOW := \033[1;33m
C_NONE   := \033[0m

$(TARGET): $(OBJ)
	@echo "$(C_GREEN)linking$(C_NONE) $@"
	@$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@echo "$(C_GREEN)compiling\033[0m $<"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@echo "$(C_YELLOW)removing$(C_NONE) $(OBJDIR)/"
	-@rm -r $(OBJDIR)

mrproper: clean
	@echo "$(C_YELLOW)removing$(C_NONE) $(TARGET)"
	-@rm $(TARGET)

all: mrproper $(TARGET)
