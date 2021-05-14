run: 
	@echo ":: Compiling $(FILE)"
	@$(CLANG) clang $(FILE) -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo ":: Running $(FILE) with args: $(ARGS)"
	@./temp $(ARGS)

pset1/mario:
	@mkdir build || echo "Build already exists, using it\n"
	@echo ":: Compiling pset1/mario_less.c"
	@$(CLANG) clang pset1/mario_less.c -ggdb -o build/mario_less -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo ":: Compiling pset1/mario_less_half.c"
	@$(CLANG) clang pset1/mario_less_half.c -ggdb -o build/mario_less_half -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo ":: Compiling mario_more"
	@$(CLANG) clang pset1/mario_more.c -ggdb -o build/mario_more -Wall -Wextra -pedantic -std=c99 -ledit -lm

	@echo "\n:: Running mario less half"
	@./build/mario_less_half
	@echo ":: Running mario less"
	@./build/mario_less
	@echo ":: Running mario more"
	@./build/mario_more

pset1: pset1/mario
	@echo "\n:: Compiling Cash"
	@$(CLANG) clang pset1/cash.c -ggdb -o build/cash -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo ":: Compiling Credit"
	@$(CLANG) clang pset1/credit.c -ggdb -o build/credit -Wall -Wextra -pedantic -std=c99 -ledit -lm

	@echo ":: Cash"
	@./build/cash
	@echo ":: Credit"
	@./build/credit
