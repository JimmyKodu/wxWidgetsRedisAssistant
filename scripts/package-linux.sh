#!/bin/bash
# Linux packaging script for Redis Assistant

set -e

VERSION=${1:-1.0.0}
BUILD_DIR="build-linux"
PACKAGE_DIR="releases/v${VERSION}/linux"
PACKAGE_NAME="RedisAssistant-${VERSION}-linux-x86_64"

echo "Building Redis Assistant for Linux (version ${VERSION})..."

# Clean and create directories
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$PACKAGE_DIR/$PACKAGE_NAME"

# Build the application
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..

# Copy executable
cp "$BUILD_DIR/RedisAssistant" "$PACKAGE_DIR/$PACKAGE_NAME/"

# Copy dependencies
echo "Copying dependencies..."
ldd "$BUILD_DIR/RedisAssistant" | grep "=> /" | awk '{print $3}' | while read lib; do
    # Only copy non-system libraries
    if [[ "$lib" == *"libwx"* ]] || [[ "$lib" == *"hiredis"* ]]; then
        cp "$lib" "$PACKAGE_DIR/$PACKAGE_NAME/" 2>/dev/null || true
    fi
done

# Create launcher script
cat > "$PACKAGE_DIR/$PACKAGE_NAME/run.sh" << 'EOF'
#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="$DIR:$LD_LIBRARY_PATH"
"$DIR/RedisAssistant" "$@"
EOF
chmod +x "$PACKAGE_DIR/$PACKAGE_NAME/run.sh"

# Create README
cat > "$PACKAGE_DIR/$PACKAGE_NAME/README.txt" << EOF
Redis Assistant v${VERSION} - Linux Build

To run the application:
    ./run.sh

Requirements:
- This is a self-contained build with all required dependencies included.
- Linux x86_64 system

For more information, visit: https://github.com/JimmyKodu/wxWidgetsRedisAssistant
EOF

# Create archive
cd "$PACKAGE_DIR"
tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME"
cd ../../../

echo "Linux package created: $PACKAGE_DIR/${PACKAGE_NAME}.tar.gz"
