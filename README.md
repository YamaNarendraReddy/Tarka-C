# Tarka-C: The Logic-Oriented Diagnostic Compiler
**Educational Engineering | Security Research | Systems Programming**

## 🚀 Overview
**Tarka-C** is a specialized C compiler designed for deterministic memory safety and advanced diagnostics. Named after **Tarka-shastra** (the Indian science of reasoning), it treats memory access as a logical proposition that must be proven "Safe" before execution.

Built on top of the [chibicc](https://github.com/rui314/chibicc) architecture, Tarka-C extends the compiler with a logical safety runtime that prevents common vulnerabilities like buffer overflows and use-after-free before they can manifest as crashes or exploits.

## ✨ Key Features
| Feature | Tarka-C (The Teacher) | Standard C (gcc/clang) |
| :--- | :--- | :--- |
| **Safety Philosophy** | **Prevention**: Proves logic before access. | **Detection**: Opportunistic crash handling. |
| **Buffer Overflows** | Traps exactly at the illegal index. | Silent corruption or "Segmentation Fault". |
| **Use-After-Free** | Logically identifies "dead" memory regions. | Exploitable Undefined Behavior. |
| **Integer Overflows** | Hardware-level traps for signed math. | Wrap-around (Undefined Behavior). |
| **Diagnostics** | Explanatory "Talking Traces". | Opaque core dumps. |

## 🏗️ Technical Architecture
Tarka-C achieves its safety guarantees through **Compiler Instrumentation**:
1.  **Memory Registry**: The runtime maintains a high-performance registry of all active memory regions (stack, heap, and globals).
2.  **Safety Gateways**: Standard library functions like `memcpy`, `strcpy`, and `malloc` are intercepted and replaced with safe, instrumented versions.
3.  **Check-Before-Store**: The compiler injects validation hooks before every pointer dereference and array access.
4.  **Neighborhood Search**: In the event of a violation, the runtime scans memory to provide context-aware diagnostics (e.g., "You are 2 bytes past buffer 'x'").

## 🛠️ Getting Started

### Prerequisites
- Linux (x86-64)
- GCC/Clang
- Make

### Building the Project
You can build the compiler and the safety runtime using the top-level Makefile:
```bash
make
```

### Compiling Your First "Safe" Program
Tarka-C supports standard `.c` files and specialized `.tc` files.
```bash
# 1. Compile to instrumented assembly
./chibicc/chibicc your_code.c -o your_code.s

# 2. Link with the Tarka runtime
cc your_code.s tarka_runtime.o -o your_binary

# 3. Run with safety logic enabled
./your_binary
```

## 🧪 Testing the "Torture" Suite
To see Tarka-C in action against common vulnerabilities, run the test script (it uses `tests/test.c`):
```bash
./test_tarka 1  # Stack Overflow via strcpy
./test_tarka 5  # Use-After-Free logic
./test_tarka 8  # Out-of-Bounds array access
```

## 📂 Repository Structure
- `chibicc/`: The instrumented C compiler.
- `tarka_runtime.c`: The logical safety runtime.
- `tests/`: A collection of `.c` and `.tc` torture tests and benchmarks.
- `docs/`: Technical philosophy and brainstorming notes.
- `Makefile`: Top-level build system.
- `test_tarka`: Automation script for running diagnostics.

## 🎯 Project Goals
Tarka-C was developed by **Indra (B.Tech CSE-IoT)** as part of a 12-month master plan to master Multimodal AI and Systems Engineering for the Japan Robotics/AI market. It serves as a foundation for understanding:
- Low-level system security.
- Compiler-level instrumentation.
- Hardware/Software co-design for safety.

---
**"Logic is the shield of memory."**
🚀 Japan Market Ready | Systems & Robotics Engineering.
