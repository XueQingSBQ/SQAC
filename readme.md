## CQAC: Core Query on Attributed Co-bipartite Graph

A C++ implementation of **VC**, **AC**, and **MAC** indexing methods for efficient subgraph queries on co-bipartite graphs with attributes. This project supports full index construction, query, and dynamic maintenance (edge add/delete), accompanied by various experiments.

---

### 🗂️ Project Structure

```
CQAC/
├── Index/                  # Index construction & maintenance
│   ├── VC_index.cpp/h
│   ├── AC_index.cpp/h
│   ├── MAC_index_build_maintenance.cpp/h
│
├── Utils/                  # Graph tools
│   ├── graph_utils.cpp/h
│
├── Graph.cpp/h             # Graph data structure
├── casestudy.cpp/h         # Experiment scripts
├── dyn.rebuild.cpp/h       # Dynamic rebuild functions
├── compute_index_size.h    # Index size estimation
├── node.h                  # Index data structure
├── Data/                   # Datasets & I/O (input/output folders)
│   ├── Input/
│   ├── Output/
```

---

### ⚙️ Build Instructions

#### Prerequisites

* C++17 compiler (e.g. `g++`, `clang++`)
* CMake (optional but recommended)

#### Quick Compile

```bash
g++ -std=c++17 -O2 -o main \
    Graph.cpp \
    VC_index.cpp \
    AC_index.cpp \
    MAC_index_build_maintenance.cpp \
    graph_utils.cpp \
    dyn.rebuild.cpp \
    casestudy.cpp \
    main.cpp
```

Or use your favorite build system like CMake or Makefile.

---

### 🚀 How to Run

#### Command Format

```bash
./your_program <index_path> <isCreate: yes|no> <experiment_id>
```

#### Supported Experiment Commands

| ID | Description                          |
| -- | ------------------------------------ |
| 0  | Create VC Index                      |
| 1  | Create AC Index                      |
| 2  | Create MAC Index                     |
| 3  | Run Accuracy Query Experiment        |
| 4  | Run Label Ratio Query Test           |
| 5  | Run MAC Edge Add Maintenance Test    |
| 6  | Run MAC Edge Delete Maintenance Test |
| 7  | Run Case Study 2 (Edge Maintenance)  |

#### Example

```bash
./main dblp yes 0       # Create VC index on dblp dataset
./main dblp no  4       # Run label ratio query test
```

---


### 📊 Datasets Overview

| Dataset   | U Node Count | V Node Count | Edge Count | U Max Degree | V Max Degree |
| --------- | ------------ | ------------ | ---------- | ------------ | ------------ |
| IMDB-5M   | 1,053,307    | 2,127,745    | 5,463,743  | 359          | 27           |
| IMDB-7M   | 1,024,124    | 2,335,658    | 7,665,681  | 341          | 18           |
| IMDB-10M  | 1,053,307    | 2,565,632    | 10,529,521 | 611          | 45           |
| Tmall-3M  | 1,066,938    | 310,188      | 3,560,108  | 128          | 158          |
| Tmall-10M | 2,056,286    | 685,309      | 10,474,521 | 101          | 68           |
| DBLP      | 1,953,085    | 5,624,219    | 12,282,059 | 1386         | 287          |


---



### 🧪 Experiments

All experiments (accuracy, performance, edge dynamics) are defined in `*.cpp` and triggered by passing the right `command_id`.

---

### 📝 License

This project is **academic/public use only**. For commercial use, please contact the author.

---

### ✍️ Author

* Code author: \[Sun Baoqing]
* Affiliation: \[Hunan University]
* Email: \[xueqing@hnu.edu.cn]

---


### 📌 TODO (Optional)

* [ ] Add Makefile / CMakeLists.txt
* [ ] Add unit tests
* [ ] Add result visualizations

---

## Acknowledgments

Many thanks to seniors like **Luo Wensheng**, **qiaoyuanyang**, and **Boge Liu** for their support to the author.

---

