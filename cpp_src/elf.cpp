#include "elf.h"
#include <fstream>
#include <iostream>

std::vector<uint8_t> le16(uint16_t n) {
    return {static_cast<uint8_t>(n), static_cast<uint8_t>(n >> 8)};
}

std::vector<uint8_t> le32(uint32_t n) {
    return {static_cast<uint8_t>(n), static_cast<uint8_t>(n >> 8), static_cast<uint8_t>(n >> 16), static_cast<uint8_t>(n >> 24)};
}

std::vector<uint8_t> le64(uint64_t n) {
    return {static_cast<uint8_t>(n), static_cast<uint8_t>(n >> 8), static_cast<uint8_t>(n >> 16), static_cast<uint8_t>(n >> 24),
            static_cast<uint8_t>(n >> 32), static_cast<uint8_t>(n >> 40), static_cast<uint8_t>(n >> 48), static_cast<uint8_t>(n >> 56)};
}

std::vector<uint8_t> make_elf_identifier() {
    return {0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
}

std::vector<uint8_t> make_elf_header(uint64_t entrypoint, uint64_t program_header_offset, uint16_t program_header_num) {
    std::vector<uint8_t> header;

    std::cout << "Creating ELF header..." << std::endl;

    auto le16_1 = le16(0x02);
    std::cout << "Adding le16(0x02): " << le16_1.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_1.begin(), le16_1.end());

    auto le16_2 = le16(0x3E);
    std::cout << "Adding le16(0x3E): " << le16_2.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_2.begin(), le16_2.end());

    auto le32_1 = le32(0x01);
    std::cout << "Adding le32(0x01): " << le32_1.size() << " bytes." << std::endl;
    header.insert(header.end(), le32_1.begin(), le32_1.end());

    auto le64_1 = le64(entrypoint);
    std::cout << "Adding le64(entrypoint): " << le64_1.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_1.begin(), le64_1.end());

    auto le64_2 = le64(program_header_offset);
    std::cout << "Adding le64(program_header_offset): " << le64_2.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_2.begin(), le64_2.end());

    auto le64_3 = le64(0x00);
    std::cout << "Adding le64(0x00): " << le64_3.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_3.begin(), le64_3.end());

    auto le32_2 = le32(0x00);
    std::cout << "Adding le32(0x00): " << le32_2.size() << " bytes." << std::endl;
    header.insert(header.end(), le32_2.begin(), le32_2.end());

    auto le16_3 = le16(0x40);
    std::cout << "Adding le16(0x40): " << le16_3.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_3.begin(), le16_3.end());

    auto le16_4 = le16(0x38);
    std::cout << "Adding le16(0x38): " << le16_4.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_4.begin(), le16_4.end());

    auto le16_5 = le16(program_header_num);
    std::cout << "Adding le16(program_header_num): " << le16_5.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_5.begin(), le16_5.end());

    auto le16_6 = le16(0x00);
    std::cout << "Adding le16(0x00): " << le16_6.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_6.begin(), le16_6.end());

    auto le16_7 = le16(0x00);
    std::cout << "Adding le16(0x00): " << le16_7.size() << " bytes." << std::endl;
    header.insert(header.end(), le16_7.begin(), le16_7.end());

    std::cout << "ELF header completed, total size: " << header.size() << " bytes." << std::endl;
    return header;
}

std::vector<uint8_t> make_program_header(uint32_t flags, uint64_t offset, uint64_t vaddr, uint64_t filesz, uint64_t memsz) {
    std::vector<uint8_t> header;

    std::cout << "Creating Program Header..." << std::endl;

    auto le32_1 = le32(0x01);
    std::cout << "Adding le32(0x01): " << le32_1.size() << " bytes." << std::endl;
    header.insert(header.end(), le32_1.begin(), le32_1.end());

    auto le32_2 = le32(flags);
    std::cout << "Adding le32(flags): " << le32_2.size() << " bytes." << std::endl;
    header.insert(header.end(), le32_2.begin(), le32_2.end());

    auto le64_1 = le64(offset);
    std::cout << "Adding le64(offset): " << le64_1.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_1.begin(), le64_1.end());

    auto le64_2 = le64(vaddr);
    std::cout << "Adding le64(vaddr): " << le64_2.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_2.begin(), le64_2.end());

    auto le64_3 = le64(0x00);
    std::cout << "Adding le64(0x00): " << le64_3.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_3.begin(), le64_3.end());

    auto le64_4 = le64(filesz);
    std::cout << "Adding le64(filesz): " << le64_4.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_4.begin(), le64_4.end());

    auto le64_5 = le64(memsz);
    std::cout << "Adding le64(memsz): " << le64_5.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_5.begin(), le64_5.end());

    auto le64_6 = le64(0x1000);
    std::cout << "Adding le64(0x1000): " << le64_6.size() << " bytes." << std::endl;
    header.insert(header.end(), le64_6.begin(), le64_6.end());

    std::cout << "Program Header completed, total size: " << header.size() << " bytes." << std::endl;
    return header;
}

bool write_elf(const std::string& filename, const std::vector<Placement>& placements) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return false;
    }
    for (const auto& placement : placements) {
        file.seekp(placement.offset);
        file.write(reinterpret_cast<const char*>(placement.bytes.data()), placement.bytes.size());
    }
    return true;
}
