#ifndef ELF_H
#define ELF_H

#include <string>
#include <vector>
#include <cstdint>

// Define the Placement structure to hold offset and bytes
struct Placement {
    size_t offset;
    std::vector<uint8_t> bytes;

    // Method to get the size of the byte array
    size_t size() const {
        return bytes.size();
    }

    // Method to get the extent (offset + size)
    size_t extent() const {
        return offset + size();
    }
};

// Function declarations
std::vector<uint8_t> make_elf_identifier();
std::vector<uint8_t> make_elf_header(uint64_t entrypoint, uint64_t program_header_offset, uint16_t program_header_num);
std::vector<uint8_t> make_program_header(uint32_t flags, uint64_t offset, uint64_t vaddr, uint64_t filesz, uint64_t memsz);
bool write_elf(const std::string& filename, const std::vector<Placement>& placements);

#endif
