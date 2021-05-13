pset1/mario_less: pset1/mario_less.c
	$(CLANG) clang pset1/mario_less.c -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm

pset1/mario_more: pset1/mario_more.c
	$(CLANG) clang pset1/mario_more.c -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm

pset1/mario_less_half: pset1/mario_less_half.c
	$(CLANG) clang pset1/mario_less_half.c -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm

pset1/mario:
	@mkdir build || echo "Build already exists, using it"
	@echo "\n\n :: Compiling mario_less"
	$(CLANG) clang pset1/mario_less.c -ggdb -o build/mario_less -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo "\n\n :: Compiling mario_less half"
	$(CLANG) clang pset1/mario_less_half.c -ggdb -o build/mario_less_half -Wall -Wextra -pedantic -std=c99 -ledit -lm
	@echo "\n\n :: Compiling mario_more"
	$(CLANG) clang pset1/mario_more.c -ggdb -o build/mario_more -Wall -Wextra -pedantic -std=c99 -ledit -lm

	@echo "\n\n :: Running mario less half"
	@./build/mario_less_half
	@echo "\n\n :: Running mario less"
	@./build/mario_less
	@echo "\n\n :: Running mario more"
	@./build/mario_more

pset1/cash: pset1/cash.c
	$(CLANG) clang pset1/cash.c -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm

pset1/credit: pset1/credit.c
	$(CLANG) clang pset1/credit.c -ggdb -o temp -Wall -Wextra -pedantic -std=c99 -ledit -lm

