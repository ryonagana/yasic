.PHONY: clean All

All:
	@echo "----------Building project:[ invaders_better - Debug ]----------"
	@cd "invaders_better" && "$(MAKE)" -f  "invaders_better.mk"
clean:
	@echo "----------Cleaning project:[ invaders_better - Debug ]----------"
	@cd "invaders_better" && "$(MAKE)" -f  "invaders_better.mk" clean
