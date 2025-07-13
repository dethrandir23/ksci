# ksci

**ksci** is an interpreter for **Kartascript**, a simple command-based scripting language designed for embedded systems like Raspberry Pi Pico. Written in C++, it allows basic GPIO and control operations in an intuitive way.

## Features

- Kartascript interpreter
- GPIO, variable, and delay operations
- Control flow with GOTO, LABEL, IF, CALL, RETURN
- Easily extensible in C++
- Command-line interface using Cliopatra.

## Compiling

```bash
git clone https://github.com/kullaniciadi/ksci.git
cd ksci
cmake -B build
cmake --build build
```

## USAGE

```kartascript
./ksci run <file_path.ks>
```

Example:
```kartascript
./ksci run ../test/deneme.ks
```

## Kartascript File Format

`.ks` files contain human-readable instructions interpreted at runtime.

Example:
```
SET A 0
LABEL START
    PRINT A
    ADD A A 1
    IF A == 5 GOTO END
    GOTO START
LABEL END
    PRINT "DONE"
```

## SUPPORTED COMMANDS

- `SET <var> <value>`
- `ADD <target> <a> <b>`
- `SUB <target> <a> <b>`
- `GPIO <pin> ON/OFF`
- `DELAY <ms>`
- `PRINT <expr1> <expr2> ... ENDL`
- `LABEL <name>`
- `GOTO <label>`
- `IF <a> <op> <b> GOTO <label>`
- `CALL <label>`, `RETURN`

## LICENSE

MIT License — see the `LICENSE` file for details.
