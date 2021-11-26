.PHONY: clean All

All:
	@echo "----------Building project:[ invaders - Release ]----------"
	@cd "invaders" && "$(MAKE)" -f  "invaders.mk"
clean:
	@echo "----------Cleaning project:[ invaders - Release ]----------"
	@cd "invaders" && "$(MAKE)" -f  "invaders.mk" clean
