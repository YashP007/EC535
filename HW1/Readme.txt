# README — EC535 HW1 (Fall 2025)

## Overview

This assignment implements a small C program, **MyBitApp**, that:

1. Reads unsigned 32-bit *decimal* integers from an input file (one per line).
2. For each number, outputs two columns to an output file:

   * **Column 1:** the **binary mirror** of the input, expressed in decimal.
   * **Column 2:** the count of occurrences of the bit pattern **“010”** in the **original** input’s 32-bit representation.
3. Uses a **linked list** to keep items sorted **lexicographically by the ASCII (decimal string) of the mirrored value** before writing them out.

> Example: `19088743` → mirror `3869426816`, “010” count `5`.

---

## File structure

```
YourBUusername_HW1/
├─ main.c       // file I/O, builds/sorts list, drives processing
├─ bits.c       // BinaryMirror() and CountSequence() implementations
├─ bits.h
├─ mylist.c     // linked-list create/insert (lexicographic by ASCII of mirror)
├─ mylist.h
├─ Makefile     // builds MyBitApp; includes clean rule
└─ README.md    // this file
```

---

## Build

On the lab machines (or any Linux with gcc):

```bash
make
```

This produces the executable `MyBitApp`.
Clean build artifacts:

```bash
make clean
```

---

## Run

```
./MyBitApp <inputfile> <outputfile>
```

### Input format

* One **unsigned 32-bit decimal** integer per line, arbitrary whitespace allowed.
* Values must be in `[0, 4294967295]`.

### Output format

* Two tab-separated decimal columns:

  1. Mirrored value (decimal)
  2. Count of “010” occurrences in the original value
* **Rows are sorted** by the **ASCII (decimal string) of the mirrored value** in ascending order.

---

## Implementation notes

* **BinaryMirror(uint32\_t)**: shifts through 32 bits, builds reversed bit order in a `uint32_t`.
* **CountSequence(uint32\_t)**: slides a 3-bit window (`& 0b111`) over all 32 positions and counts matches to `0b010`.
* **Linked list**: `appendNode(&head, flip_val, count)` performs insertion so the list remains sorted **lexicographically by `ascii`**, where `ascii` is the decimal string for the mirrored value (e.g., `"3869426816"`).
* **Robustness**: `main.c` checks `argc`, `fopen`, and `malloc` failures.
* **Debugging**: set `debug_mode = 0/1` in `main.c` to silence/enable logs.

---

## Assumptions & limitations

* Input integers are decimal, not hex or binary.
* Only the mirrored value’s **decimal string** is used for sorting (as required: sorted by “ASCII representation”).
* Output prints **mirrored value** and **pattern count** only (the list may hold more fields internally).

---

## Quick test

**in.txt**

```
19088743
1
10
```

**Run**

```bash
./MyBitApp in.txt out.txt
```

**Expected (example)**

```
1	0
3869426816	5
1342177280	1
```

(Exact order may vary with more inputs; sorting is lexicographic on the **string** of the mirrored value.)