#include <iostream>
#include <vector>
#include <iomanip>
#include "elf.h"
#include "instruct.h"

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[41m"
#define GREEN "\033[42m"
#define YELLOW "\033[43m"
#define BLUE "\033[44m"
#define MAGENTA "\033[45m"
#define CYAN "\033[46m"

// Function to display a hex dump of the ELF file with colored output
void hex_dump(const std::vector<Placement>& placements) {
    if (placements.empty()) {
        std::cout << "Error: No placements found to display." << std::endl;
        return;
    }

    for (const auto& placement : placements) {
        if (placement.bytes.empty()) {
            std::cout << "Warning: Placement at offset 0x" << std::hex << placement.offset << " has no bytes." << std::endl;
            continue;
        }

        const char* color;
        if (placement.offset == 0x00) {
            color = RED; // ELF identifier
        } else if (placement.offset == 0x10) {
            color = GREEN; // ELF header
        } else if (placement.offset == 0x40 ) {
            color = BLUE; // Program headers
        } else if (placement.offset == 0x78) {
            color = MAGENTA; // Program headers
        } else if (placement.offset == 0xc0) {
            color = YELLOW; // Code section
        } else {
            color = CYAN; // Data section
        }

        std::cout << color << "Offset: 0x" << std::hex << std::setw(8) << std::setfill('0') << placement.offset << RESET << std::endl;
        for (size_t i = 0; i < placement.bytes.size(); ++i) {
            if (i % 16 == 0) {
                std::cout << color << "\n0x" << std::setw(8) << std::setfill('0') << placement.offset + i << ": " << RESET;
            }
            std::cout << color << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(placement.bytes[i]) << " " << RESET;
        }
        std::cout << std::endl << std::endl;
    }
}

std::vector<uint8_t> concatenate_instructions() {
    std::vector<uint8_t> result;

    // Concatenate the vectors returned by mov_r32_imm32 and syscall into result
    auto append = [&](const std::vector<uint8_t>& vec, const std::string& instruction) {
        if (vec.empty()) {
            std::cerr << "Error: The vector for " << instruction << " is empty!" << std::endl;
            return;
        }

        std::cout << "Appending " << vec.size() << " bytes for " << instruction << std::endl;
        result.insert(result.end(), vec.begin(), vec.end());
    };

    append(mov_r32_imm32("rax", 0x01), "mov rax, 1");
    append(mov_r32_imm32("rdi", 0x01), "mov rdi, 1");
    append(mov_r32_imm32("rsi", 0x600000 + 0xf0), "mov rsi, 0x600000 + data offset");
    append(mov_r32_imm32("rdx", 14), "mov rdx, data size");
    append(syscall(), "syscall (write)");
    append(mov_r32_imm32("rdi", 0x00), "mov rdi, 0");
    append(mov_r32_imm32("rax", 0x3C), "mov rax, 60 (exit)");
    append(syscall(), "syscall (exit)");

    return result;
}

int main() {
    std::cout << "Starting program..." << std::endl;

    Placement data {
        0xf0,
        std::vector<uint8_t>({'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', '\0', '\0'})
    };

    std::cout << "Data initialized." << std::endl;

    // Create the code section by concatenating the instructions
    Placement code {
        0xc0,
        concatenate_instructions()
    };

    std::cout << "Code initialized. Size of code: " << code.bytes.size() << " bytes." << std::endl;

    // Debug sizes of vectors returned by ELF header functions
    auto elf_identifier = make_elf_identifier();
    std::cout << "ELF identifier size: " << elf_identifier.size() << " bytes." << std::endl;

    auto elf_header = make_elf_header(0x400000 + code.offset, 0x40, 2);
    std::cout << "ELF header size: " << elf_header.size() << " bytes." << std::endl;

    auto program_header1 = make_program_header(0x05, 0, 0x400000, code.extent(), code.extent());
    std::cout << "Program header 1 size: " << program_header1.size() << " bytes." << std::endl;

    auto program_header2 = make_program_header(0x04, 0, 0x600000, data.extent(), data.extent());
    std::cout << "Program header 2 size: " << program_header2.size() << " bytes." << std::endl;

    std::vector<Placement> elf_file = {
        {0x00, elf_identifier},
        {0x10, elf_header},
        {0x40, program_header1},
        {0x78, program_header2},
        code,
        data
    };

    std::cout << "ELF file initialized." << std::endl;

    // Display the ELF file contents as a hex dump with colors
    hex_dump(elf_file);

    std::cout << "Program finished." << std::endl;

    return 0;
}