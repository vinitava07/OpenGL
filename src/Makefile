# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -I./Libraries/include -Wall

# Bibliotecas necessárias
LIBS = -ldl -lglfw -lassimp

# Arquivos-fonte
SRCS = main.cpp glad.c Libraries/lib/stb.cpp
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

# Nome do executável
TARGET = app

# Regras
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJS)
	$(CXX) $^ $(LIBS) -o $@

# Compilar arquivos .cpp e .c para .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJS) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)