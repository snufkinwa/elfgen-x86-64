#include <cstdint>   
#include <vector>    
#include <string>     
#include <stdexcept>  
#include "instruct.h" 

// Get register code based on the register name
uint8_t get_register_code(const std::string& reg_name) {
    if (reg_name == "rax") return 0;
    if (reg_name == "rdi") return 7;
    if (reg_name == "rsi") return 6;
    if (reg_name == "rdx") return 2;
    throw std::invalid_argument("Unsupported register");
}

// Create machine code for mov instruction (mov r32, imm32)
std::vector<uint8_t> mov_r32_imm32(const std::string& reg_name, uint32_t imm32) {
    uint8_t reg = get_register_code(reg_name);
    return {
        0xC7, static_cast<uint8_t>(0xC0 | reg), 
        static_cast<uint8_t>(imm32), 
        static_cast<uint8_t>(imm32 >> 8), 
        static_cast<uint8_t>(imm32 >> 16), 
        static_cast<uint8_t>(imm32 >> 24)
    };
}

// Create syscall instruction
std::vector<uint8_t> syscall() {
    return {0x0F, 0x05};
}
