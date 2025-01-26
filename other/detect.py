def detect_indentation(file_path):
    with open(file_path, 'r') as f:
        for line in f:
            stripped = line.lstrip()
            # 跳过空行
            if not stripped:
                continue
            leading = line[:len(line) - len(stripped)]
            if leading.startswith('\t'):
                print("Tab: " + line) 
            elif leading == ' ' * 4:
                print("4 spaces: " + line) 
            elif leading.strip():
                continue  # 忽略非缩进行
    return "Unknown or no indentation"

# 示例用法
file_path = "target.cpp"
detect_indentation(file_path)