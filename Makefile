CC = clang
LDFLAGS = -fsanitize=address -g
CFLAGS = -std=c89 -Wall -fsanitize=address -g

BIN_DIR = bin
OBJ_DIR = obj

COM_DIR = common
COM_SRC = $(wildcard $(COM_DIR)/*.c)
COM_OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(COM_SRC)))

CSPICE_DIR = cspice
CSPICE_SRC_DIR = $(CSPICE_DIR)/src
CSPICE_INC_DIR = $(CSPICE_DIR)/include
CSPICE_SRC = $(wildcard $(CSPICE_SRC_DIR)/*.c)
CSPICE_OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(CSPICE_SRC)))

SUNVEC_DIR = sunvec
SUNVEC_SRC = $(wildcard $(SUNVEC_DIR)/*.c)
SUNVEC_OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SUNVEC_SRC)))
SUNVEC_BIN = $(BIN_DIR)/sunvec

.PHONY: all common sunvec cspice clean mkdirs

all: common sunvec

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

common: $(COM_OBJ)

cspice: $(CSPICE_OBJ)

sunvec: common cspice $(SUNVEC_BIN)

$(OBJ_DIR)/$(COM_DIR)/%.c.o: $(COM_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(COM_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(CSPICE_SRC_DIR)/%.c.o: $(CSPICE_SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(CSPICE_DIR)
	@mkdir -p $(OBJ_DIR)/$(CSPICE_SRC_DIR)
	$(CC) $(CFLAGS) -I$(CSPICE_INC_DIR) -c $< -o $@

$(OBJ_DIR)/$(SUNVEC_DIR)/%.c.o: $(SUNVEC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(SUNVEC_DIR)
	$(CC) $(CFLAGS) -I$(CSPICE_INC_DIR) -c $< -o $@

$(SUNVEC_BIN): $(SUNVEC_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(CSPICE_OBJ) $(SUNVEC_OBJ) -o $(SUNVEC_BIN)