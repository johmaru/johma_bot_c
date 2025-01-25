APP = ./johma_bot

build:
	gcc johma_bot.c -o johma_bot -pthread -ldiscord -lcurl

run:
	./johma_bot	