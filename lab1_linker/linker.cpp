/**********************************
 * Anthor: Zihao Wang             *
 * Date: 02-21-2017               *
 * Email: zw1074@nyu.edu          *
 **********************************/
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <map>

typedef std::map<std::string, int> sym_map;

struct instruction {
    char type;
    int address;
    int error_type = 0; // 0 means OK, c++11 new feature for initialization.
};

struct Warning {
    int warncode;
    int Module_n;
    std::string name;
    int rela_addr = 999;
    int max_addr = 999;
};

// Define some global variables.
std::vector<int> module_address; // Store the module address
std::vector<std::vector<instruction> > instr_list; // Store instruction
std::vector<std::vector<std::string> > use_list; // Store the use 
sym_map sym_table; // Store symbol table
sym_map sym_module; // Store symbol module
int linenum, lineoffset; // Store parse error information

// This split function is copied from one solution of split by delim question in StackOverflow
std::vector<std::string> split(const std::string& text, const std::string& delims)
{
    std::vector<std::string> tokens;
    std::size_t start = text.find_first_not_of(delims), end = 0;

    while((end = text.find_first_of(delims, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(delims, end);
    }
    if(start != std::string::npos)
        tokens.push_back(text.substr(start));

    return tokens;
}

inline bool isInteger(const std::string& s)
{
   // check if input can be converted to integer.
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char* p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

void __parseerror(int errcode) {
    static std::string errstr[] = {
        "NUM_EXPECTED", // Number expect
        "SYM_EXPECTED", // Symbol Expected
        "ADDR_EXPECTED", // Addresssing Expected which is A/E/I/R
        "SYM_TOO_LONG", // Symbol Name is too long
        "TO_MANY_DEF_IN_MODULE", // > 16
        "TO_MANY_USE_IN_MODULE", // > 16
        "TO_MANY_INSTR", // total num_instr exceeds memory size (512)
    };
    std::cout << "Parse Error line " << linenum << " offset " << lineoffset << ": " << errstr[errcode] << std::endl;
}

void __Error(int errcode, std::string name = "None") {
    switch(errcode) {
        case 1:
            std::cout << " Error: Absolute address exceeds machine size; zero used" << std::endl;
            break;
        case 2:
            std::cout << " Error: Relative address exceeds module size; zero used" << std::endl;
            break;
        case 3:
            std::cout << " Error: External address exceeds length of uselist; treated as immediate" << std::endl;
            break;
        case 4:
            std::cout << " Error: " << name << " is not defined; zero used" << std::endl;
            break;
        case 5:
            std::cout << " Error: This variable is multiple times defined; first value used" << std::endl;
            break;
        case 6:
            std::cout << " Error: Illegal immediate value; treated as 9999" << std::endl;
            break;
        case 7:
            std::cout << " Error: Illegal opcode; treated as 9999" << std::endl;
    }
}

void __Warning(int warncode, int Module_n, std::string name, int rela_addr = 999, int max_addr = 999) {
    switch(warncode) {
        case 1:
            std::cout << "Warning: Module " << Module_n << ": " << name << " too big " << rela_addr << " (max=" << max_addr << ") assume zero relative\n";
            break;
        case 2:
            std::cout << "Warning: Module " << Module_n << ": " << name << " appeared in the uselist but was not actually used\n";
            break;
        case 3:
            std::cout << "Warning: Module " << Module_n << ": " << name << " was defined but never used\n";
            break;
    }
}

std::string expand_4_digit(int num) {
    std::string tmp = std::to_string(10000+num);
    tmp.erase(0,1);
    return tmp;
}

void Pass_One(const char* filename) {
    /*
     * Pass first value to preloaded address.
     */ 
    std::ifstream file;
    std::vector<Warning> warning_list; // Only focus on warning 1
    int count = 10000;
    std::string name;
    std::string line;
    int address;
    int current_address = 0;
    int current_count;
    int Module_n = 1;
    std::vector<std::string> tmp_use;
    std::vector<instruction> tmp_instr;
    std::vector<std::string> current_def_sym;
    sym_map current_sym_table;
    sym_map error_sym;
    int stage = 1; // 1 means definition stage, 2 means uselist stage, 3 means program stage.
    int flag_expect = 1; // 0 means symbol, 1 means number, 2 means address
    file.open(filename, std::ios::in);
    linenum = 1;
    int old_linenum;
    int old_offset;
    int offset = 0;
    int inspace = 1;
    char ch;
    char type;
    int stage_over = 0;
    std::string tmp;
    Warning tmp_warning;
    instruction tmp_instruction;
    if (file.is_open()) {
        while (file.good())
        {
            ch = file.get();
            offset += 1;
            if (!file.good()) {
                if (!stage_over) {
                    lineoffset = old_offset;
                    linenum = old_linenum;
                    switch(flag_expect) {
                        case 0:
                            __parseerror(1);
                            exit(1);
                        case 1:
                            __parseerror(0);
                            exit(1);
                        case 2:
                            __parseerror(2);
                            exit(1);
                    }
                }
            }
            // Many condition
            if (ch != '\n' && ch != ' ' && ch != '\t') tmp.push_back(ch);
            else {
                lineoffset = offset - tmp.size();
                if (tmp.size() != 0) {
                    // std::cout << stage << std::endl;
                    switch(stage) {
                        case 1: // definition stage
                            if (count == 10000) { // New Module
                                module_address.push_back(current_address);
                                if (!isInteger(tmp)) { __parseerror(0); exit(1); }
                                count = std::stoi(tmp);
                                if (count > 16) { __parseerror(4); exit(1); }
                                current_count = 0;
                                flag_expect = 0;
                                stage_over = 0;
                                if (current_count == count) {
                                    count = 10000; flag_expect = 1; stage = 2;
                                }
                            }
                            else {
                                if (current_count < count) {
                                    if (isInteger(tmp)) {
                                        if (flag_expect == 1) {
                                            address = std::stoi(tmp);
                                            current_def_sym.push_back(name);
                                            current_sym_table.insert({{name, address}});
                                            sym_module.insert({{name, Module_n}});
                                            current_count += 1;
                                            flag_expect = 0;
                                            if (current_count == count) {
                                                count = 10000; flag_expect = 1; stage = 2;
                                            }
                                        }
                                        else { __parseerror(1); exit(1); }
                                    }
                                    else { 
                                        if (flag_expect == 0)
                                            if (tmp.size() > 16) { __parseerror(3); exit(1); }
                                            else { name = tmp; flag_expect = 1; }
                                        else { 
                                            __parseerror(0); 
                                            exit(1); 
                                        }
                                    }
                                }
                            }
                            break;
                        case 2: // use list
                            if (count == 10000) { 
                                if (!isInteger(tmp)) { __parseerror(0); exit(1); }
                                count = std::stoi(tmp); 
                                if (count > 16) { __parseerror(5); exit(1); }
                                current_count = 0; 
                                if (current_count == count) { use_list.push_back(tmp_use); tmp_use.clear(); count = 10000; stage = 3; }
                            }
                            else {
                                if (current_count < count) {
                                    if (isInteger(tmp)) { __parseerror(1); exit(1); }
                                    tmp_use.push_back(tmp);
                                    current_count += 1;
                                    if (current_count == count)
                                        { use_list.push_back(tmp_use); tmp_use.clear(); count = 10000; stage = 3; }
                                }
                            }
                            break;
                        case 3: // Instruction list
                            if (count == 10000) { 
                                if (!isInteger(tmp)) { __parseerror(0); exit(1); }
                                count = std::stoi(tmp); 
                                if (current_address + count > 512) { __parseerror(6); exit(1); }
                                for (int i = 0; i < current_def_sym.size(); i++) {
                                    if (current_sym_table[current_def_sym[i]] > count - 1) {
                                        tmp_warning.warncode = 1;
                                        tmp_warning.Module_n = Module_n;
                                        tmp_warning.name = current_def_sym[i];
                                        tmp_warning.rela_addr = current_sym_table[current_def_sym[i]];
                                        tmp_warning.max_addr = count - 1;
                                        warning_list.push_back(tmp_warning);
                                        current_sym_table[current_def_sym[i]] = 0;
                                    }
                                    if (sym_table.find(current_def_sym[i]) == sym_table.end()) sym_table.insert({{current_def_sym[i], current_sym_table[current_def_sym[i]] + current_address}});
                                    else {
                                        if (sym_table[current_def_sym[i]] != 999) {
                                            if (error_sym.find(current_def_sym[i]) == error_sym.end()) error_sym.insert({{current_def_sym[i], 5}});
                                        else sym_table[current_def_sym[i]] = current_sym_table[current_def_sym[i]] + current_address;
                                        } 
                                    }
                                }
                                current_count = 0; 
                                if (current_count == count) { current_def_sym.clear(); instr_list.push_back(tmp_instr); tmp_instr.clear(); Module_n += 1; count = 10000; stage = 1; flag_expect = 1; }
                                flag_expect = 2;
                            }
                            else {
                                if (current_count < count) {
                                    if (isInteger(tmp)) {
                                        if (flag_expect == 1) {
                                            address = std::stoi(tmp);
                                            tmp_instruction.type = type;
                                            tmp_instruction.address = address;
                                            tmp_instr.push_back(tmp_instruction);
                                            current_count += 1;
                                            flag_expect = 2;
                                            if (current_count == count) { 
                                                current_def_sym.clear(); 
                                                instr_list.push_back(tmp_instr); 
                                                tmp_instr.clear(); 
                                                current_address += count; 
                                                count = 10000; 
                                                stage = 1; 
                                                stage_over = 1;
                                                Module_n += 1;
                                            }
                                        }
                                        else { __parseerror(2); exit(1); }
                                    }
                                    else {
                                        if (flag_expect == 2)
                                            if (tmp != "A"
                                             && tmp != "E"
                                             && tmp != "I"
                                             && tmp != "R") { __parseerror(2); exit(1); }
                                            else { type = tmp[0]; flag_expect = 1; }
                                        else { __parseerror(0); exit(1); }
                                        
                                    }
                                }
                            }
                            break;
                    }
                    tmp.clear();
                }
                if (ch == '\n') { 
                    old_linenum = linenum;
                    old_offset = offset;
                    linenum += 1; 
                    offset = 0; 
                }
            }
        }
    }
    file.close();
    // Print first Warning and symbol table
    for (int i = 0; i < warning_list.size(); i++) {
        __Warning(warning_list[i].warncode, warning_list[i].Module_n, warning_list[i].name, warning_list[i].rela_addr, warning_list[i].max_addr);
    }
    std::cout << "Symbol Table" << std::endl;
    for (auto it = sym_table.begin(); it != sym_table.end(); ++it) {
        std::cout << it->first << "=" << it->second;
        if (error_sym.find(it->first) != error_sym.end()) __Error(5);
        else std::cout << std::endl;
    }
}

void Pass_Two() {
    int big_number = 1000;
    std::string absolute_value;
    instruction tmp_instr;
    sym_map sym_useage;
    sym_map uselist_useage;
    for (auto it = sym_table.begin(); it != sym_table.end(); ++it) {
        sym_useage.insert({{it->first, 0}});
    }
    std::cout << "Memory Map" << std::endl;
    int current_address = 0;
    for (int Module_n = 0; Module_n < module_address.size(); Module_n++) {
        for (int j = 0; j < use_list[Module_n].size(); j++)
            uselist_useage.insert({{use_list[Module_n][j], 0}});
        for (int instr_n = 0; instr_n < instr_list[Module_n].size(); instr_n++) {
            tmp_instr = instr_list[Module_n][instr_n];
            absolute_value = std::to_string(big_number);
            big_number += 1;
            absolute_value.erase(0,1);
            if (tmp_instr.type == "I"[0]) {
                if (tmp_instr.address > 9999) {
                    std::cout << absolute_value << ": " << 9999;
                    __Error(6);
                }
                else std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address) << '\n';
            }
            else if (tmp_instr.type == "A"[0]) {
                if (tmp_instr.address > 9999) {
                    std::cout << absolute_value << ": " << 9999;
                    __Error(7);
                }
                else if (tmp_instr.address % 1000 > 512) {
                    std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address/1000*1000);
                    __Error(1);
                }
                else std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address) << '\n';
            }
            else if (tmp_instr.type == "R"[0]) {
                if (tmp_instr.address > 9999) {
                    std::cout << absolute_value << ": " << 9999;
                    __Error(7);
                }
                else if (tmp_instr.address % 1000 > instr_list[Module_n].size()) {
                    std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address/1000*1000 + current_address);
                    __Error(2);
                }
                else std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address + current_address) << '\n';
            }
            else if (tmp_instr.type == "E"[0]) {
                if (tmp_instr.address > 9999) {
                    std::cout << absolute_value << ": " << 9999;
                    __Error(7);
                }
                else if (tmp_instr.address % 1000 > use_list[Module_n].size()-1) {
                    std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address);
                    __Error(3);
                }
                else if (sym_table.find(use_list[Module_n][tmp_instr.address % 1000]) == sym_table.end()) {
                    uselist_useage[use_list[Module_n][tmp_instr.address % 1000]] = 1;
                    std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address/1000*1000);
                    __Error(4, use_list[Module_n][tmp_instr.address % 1000]);
                }
                else {
                    uselist_useage[use_list[Module_n][tmp_instr.address % 1000]] = 1;
                    sym_useage[use_list[Module_n][tmp_instr.address % 1000]] = 1;
                    std::cout << absolute_value << ": " << expand_4_digit(tmp_instr.address/1000*1000 + sym_table[use_list[Module_n][tmp_instr.address % 1000]]) << '\n';
                }
            }
        }
        for (auto it = uselist_useage.begin(); it != uselist_useage.end(); ++it) {
            if (it->second == 0)
                __Warning(2, Module_n + 1, it->first);
        }
        current_address += instr_list[Module_n].size();
        uselist_useage.clear();
    }
    std::cout << std::endl;
    for (auto it = sym_useage.begin(); it != sym_useage.end(); ++it) {
        if (it->second == 0)
            __Warning(3, sym_module[it->first], it->first);
    }
}

int main( int argc, char *argv[] ) {
    Pass_One(argv[1]);
    std::cout << std::endl;
    Pass_Two();
    return 0;
}
