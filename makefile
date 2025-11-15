all: server run

server:
	@echo Building server...
	@g++ .\server.cpp .\HTTPparser.cpp .\HTTPReader.cpp .\Utils.cpp .\Socket.cpp .\ClientHandler.cpp -lws2_32 -o ./build/server.exe

run:
	@echo Running server...
	@./build/server.exe

clean:
	@echo Cleaning build directory...
	@rmdir /s /q build
	@mkdir build