# Kartascript 🧠⚡

Kartascript is a simple interpreted scripting language designed for embedded systems like the Raspberry Pi Pico.  
It focuses on clarity, low-level control (GPIO, delay, variables), and modularity — perfect for DIY hardware terminals and microcontroller automation.

---

## ✨ Features

- Minimal syntax, easy to read and write
- No compilation required — runs directly via the `ksci` interpreter
- Works with GPIO pins, delays, and simple control structures
- Designed with microcontrollers in mind
- Extendable with labels, functions (CALL/RETURN), and conditional jumps

---

## 📦 Example Script

```kartascript
SET LED 13
SET DELAY_TIME 500
SET COUNTER 0
SET MAX_COUNT 5

LABEL LOOP
    PRINT "BLINKING: " COUNTER "/" MAX_COUNT ENDL
    GPIO LED ON
    DELAY DELAY_TIME
    GPIO LED OFF
    DELAY DELAY_TIME
    ADD COUNTER COUNTER 1
    IF COUNTER == MAX_COUNT GOTO END
    GOTO LOOP

LABEL END
    PRINT "DONE!" ENDL
```

---

## 🧾 Supported Instructions

| Instruction | Description |
|------------|-------------|
| `SET var value` | Define a variable with an integer value |
| `ADD target val1 val2` | target = val1 + val2 |
| `SUB target val1 val2` | target = val1 - val2 |
| `GPIO pin ON/OFF` | Simulates turning a GPIO pin on or off |
| `DELAY ms` | Wait for given milliseconds |
| `LABEL name` | Define a label for jumps or calls |
| `GOTO label` | Jump to a label unconditionally |
| `IF val1 OP val2 GOTO label` | Conditional jump. Supported operators: `==`, `!=`, `>`, `<`, `>=`, `<=` |
| `CALL label` | Call a subroutine (pushes current line to stack) |
| `RETURN` | Return to previous location after `CALL` |
| `PRINT value...` | Print one or more values. Can be constants, variables, or `"strings"` |
| `READ pin` | (Simulated) read value from pin into variable |

> ✅ `ENDL` is a special keyword used in `PRINT` to add a newline (`\n`) and flush output.

---

## 📘 Syntax Tips

- All values are separated by spaces.
- Variables are **case-sensitive**.
- Labels are declared as: `LABEL name`
- No indentation or braces — structure is handled via labels and jumps.
- Strings must be in `"double quotes"` and cannot contain spaces.
- To prevent buffering issues in embedded terminals, always end prints with `ENDL`.

---

## 🧪 Sample Output

```text
BLINKING: 0 / 5
GPIO 13 is on now!
GPIO 13 is off now!
BLINKING: 1 / 5
GPIO 13 is on now!
GPIO 13 is off now!
...
DONE!
```

---

## 🚀 Roadmap Ideas

- GPIO read/write on real hardware (e.g., Pico SDK integration)
- File I/O from SD cards
- Built-in `nano`-like script editor for devices with screens
- Scripting REPL (interactive mode)
- String operations, arrays, and `WHILE` loops

---

## 📄 License

MIT License — free to use, modify, and embed in your toaster robot.

---

🧑‍💻 Written by **Bekir Efe Öztürk**
👾 Language Engine: [`ksci`](https://github.com/dethrandir23/ksci)
