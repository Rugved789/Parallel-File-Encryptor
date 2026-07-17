CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/file_handling -Isrc/app/processes
RM = cmd /c del /q /f

MAIN_TARGET = encrypt_decrypt.exe
CRYPTION_TARGET = cryption.exe

MAIN_SRC = main.cpp \
           src/app/processes/ProcessManagement.cpp \
           src/app/file_handling/IO.cpp \
           src/app/file_handling/ReadEnv.cpp \
           src/app/encryptDecrypt/Cryption.cpp

CRYPTION_SRC = src/app/encryptDecrypt/CryptionMain.cpp \
               src/app/encryptDecrypt/Cryption.cpp \
               src/app/file_handling/IO.cpp \
               src/app/file_handling/ReadEnv.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)

all: $(MAIN_TARGET) $(CRYPTION_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET)

.PHONY: clean all