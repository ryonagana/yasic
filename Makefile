.PHONY: clean All

All:
	@echo "----------Building project:[ invaders_linux - Debug ]----------"
	@cd "invaders_linux" && "$(MAKE)" -f  "invaders_linux.mk"
clean:
	@echo "----------Cleaning project:[ invaders_linux - Debug ]----------"
	@cd "invaders_linux" && "$(MAKE)" -f  "invaders_linux.mk" clean
