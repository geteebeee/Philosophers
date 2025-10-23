# philo

## 🧩 Overview

Each philosopher alternates between eating, sleeping, and thinking. To eat, a philosopher must take two forks — one on the left and one on the right. Proper synchronization ensures no deadlocks or data races occur.

## ⚙️ Build

```bash
make
```

## 🚀 Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]
```

### Example

```bash
./philo 5 800 200 200
```

### Arguments

| Argument                       | Description                                                       |
| ------------------------------ | ----------------------------------------------------------------- |
| `number_of_philosophers`       | Number of philosophers and forks                                  |
| `time_to_die`                  | Time (ms) before a philosopher dies if not eating                 |
| `time_to_eat`                  | Time (ms) a philosopher takes to eat                              |
| `time_to_sleep`                | Time (ms) a philosopher sleeps before thinking again              |
| `number_of_meals` *(optional)* | Simulation stops when all philosophers have eaten this many times |

