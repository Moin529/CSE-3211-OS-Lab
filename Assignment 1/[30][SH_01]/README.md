# Custom Shell Implementation [SH_01]

## Assignment Information
- **Course:** Operating Systems Lab
- **Assignment:** Building a Custom Shell with Basic Commands

## Features Implemented

### I. Basic Commands (Manual Implementation)
The following commands are handled via dedicated functions within the code:
* `pwd`: Displays the current working directory.
* `ls`: Lists directory contents (supports `-a` for hidden files).
* `cd`: Changes the working directory (Built-in).
* `mkdir`: Creates one or more directories.
* `touch`: Creates one or more empty files.
* `rm`: Removes specified files.
* `cp`: Copies a source file to a destination.
* `mv`: Renames or moves a file.
* `cat`: Displays contents of one or more files.
* `echo`: Prints text to the terminal (Built-in).

### II. Advanced Features & Enhancements
* **Process Management:** Uses `fork()`, `execvp()`, and `waitpid()` to execute external system commands not manually defined.
* **Command Arguments:** All commands support multiple arguments (e.g., `mkdir folder1 folder2 folder3`).
* **I/O Redirection (Bonus):**
    * Output Redirection (`>`): Redirects command output to a file (e.g., `ls > files.txt`).
    * Input Redirection (`<`): Reads command input from a file (e.g., `cat < test.txt`).
* **Background Execution (Bonus):** Append `&` to run commands in the background without blocking the shell (e.g., `sleep 10 &`).
* **Error Handling:** Provides descriptive error messages using `perror` for invalid paths, missing files, or permission issues.

## File Structure
* `shell.c`: The main C source code containing command logic and the execution loop.
* `run.sh`: A bash script to automate compilation and execution.
* `README.md`: Assignment Task documentation.

## How to Compile and Run

1.  **Ensure you have GCC installed** on your Linux/Unix environment.
2.  **Grant execution permission** to the shell script:
    ```bash
    chmod +x run.sh
    ```
3.  **Run the script**:
    ```bash
    ./run.sh
    ```

## Usage Examples
- **Manual Command:** `CSEDU_Shell> ls -a`
- **Redirection:** `CSEDU_Shell> echo Hello World > message.txt`
- **Background:** `CSEDU_Shell> cat message.txt &`
- **Exit:** `CSEDU_Shell> exit`
