#!/bin/bash

# 指定文件路径
file_path="target.cpp"

# 确保文件存在
if [ ! -f "$file_path" ]; then
    echo "File not found: $file_path"
    exit 1
fi

# 等待3秒
echo "Waiting for 3 seconds..."
sleep 3


# 使用 xdotool 模拟键盘输入内容
echo "Start"
echo "" > log.txt
# 逐行读取文件内容并输入
while IFS= read -r line || [ -n "$line" ]; do
    # 输入每一行内容
    echo "$line" >> log.txt
    xdotool type "$line"
    # 模拟回车键
    xdotool key Return
    xdotool key Home
done < "$file_path"