
# Convert Windows path to Unix path (sử dụng cygpath nếu có)
if command -v cygpath &> /dev/null; then
    FILE_PATH=$(cygpath "$1")
else
    # Fallback: tự chuyển C:\ → /c/ (thô sơ, không chính xác 100%)
    FILE_PATH="/$(echo "$1" | sed 's|\\|/|g' | sed 's|:||')"
fi

# Lấy thư mục chứa file
TARGET_DIR=$(dirname "$FILE_PATH")

# In debug ra cho chắc
echo "→ Jumping to: $TARGET_DIR"

# Nhảy vào thư mục đó
cd "$TARGET_DIR" || exit

# Mở shell để người dùng tiếp tục làm việc
exec bash