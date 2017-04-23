### Compiler Back End

The general idea of register allocation is straightforward. At any given point, the processor can only hold a finite number of values in its registers, and the values being used need to be in those registers. If there are not enough registers, we have register spill, and extra store and load instructions are needed. The way to handle register spill is to use memory.

In this Project, we implement both Naive and CFG intra-block allocation techniques.

When Parser did syntax checking, semantic checking and generated the corresponding IR code, `Generator` will receive
IR info and do register allocation, instruction selection and code generation.

```c++
/**
 * Convert IR to MIPS code.
 */
class Generator {
  public:
    Generator(std::string filename);
    Generator(std::vector<std::string>& ir, func_info_t& func_info);
    virtual void generate() = 0;
    void dump();
}

// naive register allocation
class GenNaive final : public Generator {...}

// CFG and intra-block register allocation
class GenCFG final : public Generator {...}

// EBB register allocation
class GenEBB final : public Generator {...}
```

In order to generate MIPS code, for convenience, we split IR code into data and text segments and utilize special
method to process each of these.

```c++
...
 private:
  void data_seg() override;
  void text_seg() override;
...
```

#### load and save variable

After live analysis and graph coloring, when registers need to be scheduled at any IR code, we have to allocated/select matched registers to load/save variables. The context is very complicate in here,
this operation maybe happened in function procedures, loops, return, general statments, etc. Therefore, we
defined two abstract operations `load` and `store` to achieve this goal.

```c++
std::string load(std::string token);
std::string load(std::string token, std::string res);
void store(std::string token, std::string reg);
```

#### integer and float

In modern MIPS, it has `t0-t9` temporary registers and `f0-f31` float registers. When each IR code is proccessing, generator must check each operand's type, then select appropriate instructions and registers. Thus, we defined an
abstract operation `check_type` to address this issue.

```c++
enum Type { INT = 0, FLOAT = 1 };
Type check_type(std::string name);
```

#### Block Register Allocation

To support block register allocation, we need some data structures to record variables in each block, variables' live range in each block, variables' registers, etc.

```c++
/**
 * @brief basic block in IR code.
 *
 * @param start_pos  start line in IR code
 * @param end_pos    end line in IR code
 */
typedef struct block {
  size_t start_pos_;
  size_t end_pos_;
} block_t;

typedef size_t reg_t;

/**
 * @brief find variable in block
 *
 * @param block_id_  block index number
 * @param var_name_  variable in this block
 */
typedef struct variable {
  size_t block_id_;
  std::string var_name_;
  friend bool operator<(const variable &lhs, const variable &rhs) {
    if (lhs.block_id_ < rhs.block_id_) {
      return true;
    } else {
      if (lhs.block_id_ == rhs.block_id_) {
        if (lhs.var_name_ < rhs.var_name_) {
          return true;
        }
      }
    }
    return false;
  }
} variable_t;

/**
 * @brief variable's live range
 *
 * @param block_id_  block index number
 * @param var_name_  variable in this block
 */
typedef struct live_range {
  size_t start_pos_;
  size_t end_pos_;
} live_range_t;
```

When these data structures are filled up, we can do live range analysis and graph coloring to allocate register for
variables in each block.

#### Function Call

To support function call, we need `c++ stack and unordered_map` data strcutures to record all functions and their function parameters and return types.


```c++
/// function argument regs map <funcname, <(param0, reg0), (param1, reg1), ...
typedef std::unordered_map<std::string,
                           std::vector<std::pair<std::string, std::string>>>
    func_info_t;
func_info_t func_map_;
std::stack<std::string> func_stack_;
```

Using these data structure, you can retrieve and answer the question, such as, whether inside a function? How many parameters in this function? What's this paramter's type?
