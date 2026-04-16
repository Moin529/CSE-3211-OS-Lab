SOURCE="shell.c"
OUTPUT="shell"

# Checking source file existence
if [ ! -f "$SOURCE" ]; then
    echo "Error: $SOURCE not found!"
    exit 1
fi

# Compilation
echo "Compiling $SOURCE..."
gcc -Wall "$SOURCE" -o "$OUTPUT"

# Check the compilation
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    echo "Launching shell... (Type 'exit' to quit)"
    echo "------------------------------------------"
    # Run the shell
    ./"$OUTPUT"
else
    echo "Compilation failed. Please check for syntax errors."
    exit 1
fi