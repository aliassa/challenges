CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude -I. -Ishared -Ientities
BUILD   = build
LIB     = $(BUILD)/libdynarray.a

# Collect all sources explicitly per directory
SRCS    = ships_game.c \
          cargo.c      \
          combat.c     \
          debug.c      \
          entities/ship.c     \
          entities/soldier.c  \
          entities/weapon.c   \
          shared/utils.c

# Mirror .c -> .o but place all objects under build/
OBJS    = $(patsubst %.c, $(BUILD)/%.o, $(SRCS))

# ─── Main target ────────────────────────────────────────────────
all: $(BUILD)/game

$(BUILD)/game: $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $^ -L$(BUILD) -ldynarray -lm -o $@

# ─── Static library ─────────────────────────────────────────────
$(BUILD)/shared/dynamic_array.o: shared/dynamic_array.c shared/dynamic_array.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(BUILD)/shared/dynamic_array.o
	ar rcs $@ $^

# ─── Generic rule — builds any .o into build/ mirroring src path ─
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ─── Utilities ──────────────────────────────────────────────────
clean:
	rm -rf $(BUILD)

rebuild: clean all

.PHONY: all clean rebuild