#pragma once

#include <sstream>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

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

typedef std::string reg_t;

/**
 * @brief find variable in block
 *
 * @param block_id_  block index number
 * @param var_name_  variable in this block
 */
typedef struct variable {
    size_t block_id_;
    std::string var_name_;
    friend bool operator<(const variable &lhs,
                          const variable &rhs) {
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

/// function argument regs map <funcname, <(param0, reg0), (param1, reg1), ...
typedef std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>> > func_info_t;
