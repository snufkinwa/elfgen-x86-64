#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> mov_r32_imm32(const std::string& reg_name, uint32_t imm32);
std::vector<uint8_t> syscall();
