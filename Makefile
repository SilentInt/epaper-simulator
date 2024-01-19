# Makefile

# 编译器
CC = clang
CXX = clang++
# CC = gcc
# CXX = g++

# 源文件夹
SRC_DIRS = . HAL epd_driver epd_driver/chips fonts icons epd_views

# 目标文件夹
BUILD_DIR = build
# 创建目标文件夹及其子目录
BUILD_DIRS = $(foreach dir,$(SRC_DIRS),$(BUILD_DIR)/$(dir))

# 逐个创建目标文件夹及其子目录
$(foreach dir,$(BUILD_DIRS),$(shell mkdir -p $(dir)))

# 查找所有的C和C++文件
C_SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
CXX_SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# 目标文件
OBJS = $(C_SRCS:%.c=$(BUILD_DIR)/%.o) $(CXX_SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 头文件
INC_DIRS = $(SRC_DIRS)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# SDL2库
SDL2_LIB = -lSDL2 -lSDL2main
# 链接选项
LDFLAGS =  $(SDL2_LIB) -fsanitize=address -fno-stack-protector -fno-var-tracking -fno-omit-frame-pointer

# 可执行文件
TARGET = $(BUILD_DIR)/main

FLAGS = -Wall -Werror -O2 -g
# 编译选项
CFLAGS =  $(FLAGS) -std=c99
CXXFLAGS = $(FLAGS) -std=c++11
# -std=c++11

run: $(TARGET)
	./$(TARGET)


# 链接所有的目标文件和SDL2库
$(TARGET): $(OBJS)
	$(CXX)  $^ $(INC_FLAGS) $(LDFLAGS) -o $@

# 编译C文件
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@ 

# 编译C++文件
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@ 

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 默认目标
all: $(TARGET)


.PHONY: all clean

