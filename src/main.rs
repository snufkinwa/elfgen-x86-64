mod elf;
mod instruct;

use elf::{make_elf_header, make_elf_identifier, make_program_header, write_elf, Placement};
use instruct::{mov_r32_imm32, syscall};

fn main() {
    let data = Placement {
        offset: 0xf0,
        bytes: b"Hello, world!\n\0\0".to_vec(),
    };

    let code = Placement {
        offset: 0xc0,
        bytes: [
            mov_r32_imm32("rax", 0x01),                         
            mov_r32_imm32("rdi", 0x01),                         
            mov_r32_imm32("rsi", 0x600000 + data.offset as u32), 
            mov_r32_imm32("rdx", data.size() as u32),           
            syscall(),                                          
            mov_r32_imm32("rdi", 0x00),                         
            mov_r32_imm32("rax", 0x3C),                         
            syscall(),
        ]
        .into_iter()  
        .flatten()   
        .collect(),   
    };
    
    let elf_file = vec![
        Placement {
            offset: 0x00,
            bytes: make_elf_identifier(),
        },
        Placement {
            offset: 0x10,
            bytes: make_elf_header(
                0x400000 + code.offset as u64,
                0x40,
                2,
            ),
        },
        Placement {
            offset: 0x40,
            bytes: make_program_header(
                0x05, // Read and execute
                0,
                0x400000,
                code.extent() as u64,
                code.extent() as u64,
            ),
        },
        Placement {
            offset: 0x78,
            bytes: make_program_header(
                0x04, // Read
                0,
                0x600000,
                data.extent() as u64,
                data.extent() as u64,
            ),
        },
        code,
        data,
    ];

    write_elf("hello_world.elf", &elf_file).expect("Failed to write ELF file");
}