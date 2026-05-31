# The Plazza

A multi-process, multi-threaded pizzeria simulation in C++20.

---

## Build

### With Make (recommended)
```bash
make        # build
make re     # clean rebuild
make clean  # remove object files
make fclean # remove build artefacts and binary
```

### With CMake directly
```bash
cmake -S . -B build
cmake --build build
```

### Generate Doxygen documentation
```bash
cmake --build build --target doc
# HTML output → docs/
```

---

## Usage

```
./plazza <multiplier> <cooks> <time> [--ticket]
```

| Parameter | Description |
|---|---|
| `multiplier` | Cooking time multiplier (positive float — `< 1` speeds up, `> 1` slows down) |
| `cooks` | Number of cook threads per kitchen |
| `time` | Ingredient restock interval in milliseconds |
| `--ticket` | *(optional)* Print a receipt with timestamps for each order |

**Example:**
```bash
./plazza 2 5 2000           # standard run
./plazza 0.5 3 500 --ticket # half-speed with receipts
```

---

## Shell commands

| Command | Description |
|---|---|
| `TYPE SIZE xQTY [; ...]` | Place one or more pizza orders |
| `status` | Display all active kitchens (cook workload + ingredient stocks) |
| `menu` | List all available pizzas with prices and ingredients |
| `exit` | Shut down all kitchens and exit |

**Order syntax:**
```
TYPE  := [a-zA-Z]+           (pizza name, case-insensitive)
SIZE  := S | M | L | XL | XXL
QTY   := x[1-9][0-9]*
```

**Examples:**
```
$> regina XXL x2; fantasia M x3; margarita S x1
$> americana L x1;margarita S x2
```

---

## Pizza types

The available pizzas are loaded from `config/menu.csv` at startup (fallback to built-in defaults if the file is absent).

| Name | Ingredients | Base cook time |
|---|---|---|
| Margarita | dough, tomato, gruyere | 1 s |
| Regina | dough, tomato, gruyere, ham, mushrooms | 2 s |
| Americana | dough, tomato, gruyere, steak | 2 s |
| Fantasia | dough, tomato, eggplant, goat cheese, chief love | 4 s |

> **Adding a pizza:** edit `config/menu.csv` — no recompilation needed.

CSV format:
```
PizzaType(string);BakingTime(second);Ingredients(comma-separated);Price
margarita;1;dough,tomato,gruyere;6
```

---

## Architecture

```
User
 │
 ▼
Reception  (main process — interactive shell)
 │  IPC via socketpair  <<  /  >>
 ├──▶ Kitchen 1  (child process)
 │       ├── Cook #0  (thread)
 │       ├── Cook #1  (thread)   ← thread pool (N cooks)
 │       ├── ...
 │       ├── Restock thread      ← +1 unit / ingredient every T ms
 │       └── Ingredient stock    (std::map + Mutex)
 ├──▶ Kitchen 2  (child process)
 └──▶ ...        (spawned on demand)
```

### Key components

| Component | Role |
|---|---|
| `Reception` | Main process — shell, load balancer, kitchen lifecycle |
| `Kitchen` | Child process — thread pool, stock management, IPC handler |
| `SafeQueue<Pizza>` | Thread-safe blocking queue between IPC loop and cook threads |
| `IPC` | `socketpair` encapsulation with `<<` / `>>` operators |
| `Thread` / `Mutex` / `ConditionVariable` | RAII wrappers over `std::thread` primitives |
| `MenuLoader` | Loads pizza recipes from CSV (or built-in defaults) |
| `PizzaSerializer` | `pack()` / `unpack()` for serializing pizza orders over IPC |
| `OrderParser` | Validates and tokenises raw shell input into pizza objects |

### Load balancing

Pizzas are dispatched one by one to the kitchen with the lowest current load. A kitchen accepts at most `2 × N` pizzas (queued + cooking). A new kitchen is forked only when all existing ones are at capacity.

### Kitchen lifecycle

A kitchen shuts itself down after **5 seconds of complete inactivity** (empty queue + all cooks idle). The reception reaps the process with `waitpid`.

---

## Log file

Every completed pizza is timestamped and appended to `plazza.log` in the working directory:

```
[2026-05-30 17:42:01] Regina (M) is ready!
[2026-05-30 17:42:03] Margarita (S) is ready!
```
