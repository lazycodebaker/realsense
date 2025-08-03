
```bash
# Full clean rebuild
rm -rf build
mkdir build && cd build
cmake ..
cmake --build . --parallel

# Clean (optional, inside build/)
cmake --build . --target clean

# Run the app (if 'run' target is defined)
cmake --build . --target run

# Conan
conan install .. --build=missing

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .

```
