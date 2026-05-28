##
## EPITECH PROJECT, 2026
## Makefile Type
## File description:
## Makefile
##

################################ UTILS ################################

BUILD_DIR	=	build

################################ NAMES ################################

NAME	=	plazza

############################### COMPILE ###############################

all:	$(NAME)

$(NAME):
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@cmake --build $(BUILD_DIR)

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		find $(BUILD_DIR) -name "*.o" -delete 2>/dev/null || true; \
	fi

fclean:	clean
	@rm -rf $(BUILD_DIR)
	@rm -f $(NAME)

re:	fclean	all

################################# END #################################

.PHONY: all clean fclean re
