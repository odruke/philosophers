
###VARIABLES###

# name of the executable
NAME = philo

# compilator and compilation flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
LFLAGS = -pthread

# directories
SRC_DIR = ./src/
OBJ_DIR = $(SRC_DIR)target/
INC_DIR = ./inc/

# files
FILES = main.c error_handle.c parsing.c utils.c simulation.c safe_functions.c init.c setters_getters.c print_functions.c
INC_FILES = philo.h

SRC = $(addprefix $(SRC_DIR), $(FILES))
OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))
INC = $(addprefix $(INC_DIR)%.h, $(INC_FILES))



#####RULES####

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(LFLAGS) -o $@
	@$(MAKE) compilation_success

# create .o file
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "Compiling: %s                                    \r" $<
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	printf "Starting compilation..."
	@mkdir -p $(OBJ_DIR)

# delete just file OBJ_DIR and o file inside
clean:
	@rm -rf $(OBJ_DIR)

# executes clean and deletes the executable
fclean: clean
	@rm -f $(NAME)

re: fclean all

compilation_success:
	@echo "	┌─────────────────────────────────────────────────────────────────────────────────────────────┐"
	@echo "	│██████╗ ██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ██████╗ ██╗  ██╗███████╗██████╗ ███████╗│"
	@echo "	│██╔══██╗██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝│"
	@echo "	│██████╔╝███████║██║██║     ██║   ██║███████╗██║   ██║██████╔╝███████║█████╗  ██████╔╝███████╗│"
	@echo "	│██╔═══╝ ██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗╚════██║│"
	@echo "	│██║     ██║  ██║██║███████╗╚██████╔╝███████║╚██████╔╝██║     ██║  ██║███████╗██║  ██║███████║│"
	@echo "	│╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝│"
	@echo "	└─────────────────────────────────────────────────────────────────────────────────────────────┘"

.PHONY: clean fclean re all compilation_success
