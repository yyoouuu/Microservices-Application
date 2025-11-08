#!/bin/bash
# ============================================
# Script: fetch_lib.sh
# 功能：克隆三方库到临时目录，复制include和src到file目录，删除临时目录
# ============================================
#$0  $1    $2  $3  将命令行参数赋给遍历
# 退出时遇到错误立即停止
set -e

# GitHub仓库地址（示例：可以替换成你自己的库）
REPO_URL="https://github.com/user/thirdparty-lib.git"

# 目标文件夹
TARGET_DIR="file"
TEMP_DIR="file_temp"

echo ">>> 创建目录..."
mkdir -p "$TARGET_DIR"
mkdir -p "$TEMP_DIR"

echo ">>> 从GitHub克隆仓库: $REPO_URL"
git clone "$REPO_URL" "$TEMP_DIR"

echo ">>> 复制 include/ 和 src/ 到 $TARGET_DIR"
# 检查并复制 include
if [ -d "$TEMP_DIR/include" ]; then
    cp -r "$TEMP_DIR/include" "$TARGET_DIR/"
else
    echo "⚠️  未找到 include 目录"
fi

# 检查并复制 src
if [ -d "$TEMP_DIR/src" ]; then
    cp -r "$TEMP_DIR/src" "$TARGET_DIR/"
else
    echo "⚠️  未找到 src 目录"
fi

echo ">>> 删除临时目录 $TEMP_DIR"
rm -rf "$TEMP_DIR"

echo "✅ 完成：include 和 src 已复制到 $TARGET_DIR/"