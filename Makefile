.PHONY: clean All

All:
	@echo "----------Building project:[ invaders_win - Debug ]----------"
	@cd "invaders_better" && "$(MAKE)" -f  "invaders_win.mk"
clean:
	@echo "----------Cleaning project:[ invaders_win - Debug ]----------"
	@cd "invaders_better" && "$(MAKE)" -f  "invaders_win.mk" clean
