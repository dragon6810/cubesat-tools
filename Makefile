CC = clang
LDFLAGS = -fsanitize=address -g
CFLAGS = -Wall -Werror -fsanitize=address -g

BIN_DIR = bin
OBJ_DIR = obj

COM_DIR = common
COM_SRC = $(wildcard $(COM_DIR)/*.c)
COM_OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(COM_SRC)))

SUNVEC_DIR = sunvec
SUNVEC_SRC = $(wildcard $(SUNVEC_DIR)/*.c)
SUNVEC_OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SUNVEC_SRC)))
SUNVEC_BIN = $(BIN_DIR)/sunvec

.PHONY: all common sunvec clean mkdirs

all: common sunvec

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

common: $(COM_OBJ)

sunvec: common $(SUNVEC_BIN)

$(OBJ_DIR)/$(COM_DIR)/%.c.o: $(COM_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(COM_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(SUNVEC_DIR)/%.c.o: $(SUNVEC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(SUNVEC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SUNVEC_BIN): $(SUNVEC_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(SUNVEC_OBJ) -o $(SUNVEC_BIN)