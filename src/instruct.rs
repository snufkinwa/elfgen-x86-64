use crate::elf::le32;

pub fn mov_r32_imm32(reg_name: &str, imm32: u32) -> Vec<u8> {
    let reg = match reg_name {
        "rax" => 0,
        "rdi" => 7,
        "rsi" => 6,
        "rdx" => 2,
        _ => panic!("Unsupported register"),
    };
    vec![0xC7, 0xC0 | reg, le32(imm32)[0], le32(imm32)[1], le32(imm32)[2], le32(imm32)[3]]
}

pub fn syscall() -> Vec<u8> {
    vec![0x0F, 0x05]
}
