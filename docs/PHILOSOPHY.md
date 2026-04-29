# Tarka-C: The Logic-Oriented Diagnostic Compiler
**Educational Engineering Project | By Indra (B.Tech CSE-IoT)**

## 1. The Philosophy of Tarka (Logic)
Named after **Tarka-shastra** (the Indian science of reasoning), this compiler treats every memory access as a logical proposition that must be proven "Safe" before execution. Standard compilers use **Detection** (checking if a program crashed after the damage is done); Tarka-C uses **Prevention** (proving logic before the hand touches the memory).

## 2. Tarka-C vs. Standard C
| Feature | Standard C (gcc/clang) | Tarka-C (The Teacher) |
| :--- | :--- | :--- |
| **Safety Philosophy** | Opportunistic (Detects later) | Deterministic (Proves before) |
| **Buffer Overflows** | Corrupts data, then crashes | Stops exactly at the illegal index |
| **Use-After-Free** | Silent corruption / exploits | Logically identifies the "dead" variable |
| **Integer Overflows** | Wraps around (Undefined Behavior) | Traps at hardware level (Deterministic) |
| **Diagnostics** | "Segmentation Fault (Core Dumped)" | "Logic Error at line X: You are Y bytes past 'buffer'" |

## 3. Educational Pillars
Tarka-C is designed as a learning tool for students to understand **why** their code is dangerous:
*   **The Talking Trace:** Instead of crashing, the runtime explains the logical breach.
*   **Neighborhood Search:** If an access fails, the runtime scans memory to tell you which variable you *probably* meant to hit.
*   **Safe Gateways:** It automatically intercepts legacy functions like `strcpy` and `memcpy` to explain their internal failures.

## 4. How to Use Tarka-C

### A. Environment Setup
```bash
# 1. Build the Tarka-C Compiler
make -C chibicc

# 2. Build the Logical Safety Runtime
cc -c tarka_runtime.c -o tarka_runtime.o
```

### B. Compiling and Running
You can use Tarka-C on any standard `.c` file or use the specialized `.tc` extension.
```bash
# Translate to Instrumented Assembly
./chibicc/chibicc -S your_code.tc -o your_code.s

# Link with the Safety Runtime
cc your_code.s tarka_runtime.o -o your_binary

# Execute the Logical Binary
./your_binary
```

## 5. Procedure for "The Torture Test"
To see Tarka-C in action, run the provided `test.c` torture script:
1. `./test_tarka 1` (Stack Overflow via intercepted `strcpy`)
2. `./test_tarka 5` (Use-After-Free via `tarka_free` logic)
3. `./test_tarka 8` (Out-of-Bounds array access)

---
**"Logic is the shield of memory."**
🚀 **Japan Market Ready: Systems, Robotics, and Security Engineering.**
