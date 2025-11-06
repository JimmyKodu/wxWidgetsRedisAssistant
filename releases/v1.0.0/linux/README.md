# Redis Assistant v1.0.0 - Linux Release

This directory will contain the Linux x86_64 release package.

After building, you will find:
- `RedisAssistant-1.0.0-linux-x86_64.tar.gz` - Compressed release package
- `RedisAssistant-1.0.0-linux-x86_64/` - Extracted package contents

## Contents
- `RedisAssistant` - Main executable
- `run.sh` - Launcher script that sets up library paths
- `README.txt` - User instructions
- `lib*.so` - Required shared libraries (wxWidgets, hiredis)

## Running
```bash
tar -xzf RedisAssistant-1.0.0-linux-x86_64.tar.gz
cd RedisAssistant-1.0.0-linux-x86_64
./run.sh
```
