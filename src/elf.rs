use std::fs::File;
use std::io::{Seek, Write};
use std::io::{self};

pub struct Placement {
    pub offset: usize,
    pub bytes: Vec<u8>,
}

impl Placement {
    pub fn size(&self) -> usize {
        self.bytes.len()
    }

    pub fn extent(&self) -> usize {
        self.offset + self.size()
    }
}

// ELF helper functions to write headers
pub fn le16(n: u16) -> [u8; 2] {
    n.to_le_bytes()
}

pub fn le32(n: u32) -> [u8; 4] {
    n.to_le_bytes()
}

pub fn le64(n: u64) -> [u8; 8] {
    n.to_le_bytes()
}

pub fn make_elf_identifier() -> Vec<u8> {
    vec![
        0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
}

pub fn make_elf_header(entrypoint: u64, program_header_offset: u64, program_header_num: u16) -> Vec<u8> {
    let mut header = vec![];
    header.extend(&le16(0x02)); 
    header.extend(&le16(0x3E)); 
    header.extend(&le32(0x01)); 
    header.extend(&le64(entrypoint)); 
    header.extend(&le64(program_header_offset)); 
    header.extend(&le64(0x00)); 
    header.extend(&le32(0x00)); 
    header.extend(&le16(0x40)); 
    header.extend(&le16(0x38)); 
    header.extend(&le16(program_header_num)); 
    header.extend(&le16(0x00)); 
    header.extend(&le16(0x00)); 
    header.extend(&le16(0x00)); 
    header
}

pub fn make_program_header(flags: u32, offset: u64, vaddr: u64, filesz: u64, memsz: u64) -> Vec<u8> {
    let mut header = vec![];
    header.extend(&le32(0x01)); 
    header.extend(&le32(flags));
    header.extend(&le64(offset));
    header.extend(&le64(vaddr));
    header.extend(&le64(0x00)); 
    header.extend(&le64(filesz));
    header.extend(&le64(memsz));
    header.extend(&le64(0x1000)); 
    header
}

// Function to write ELF to file
pub fn write_elf(filename: &str, placements: &[Placement]) -> io::Result<()> {
    let mut file = File::create(filename)?;
    for placement in placements {
        file.seek(io::SeekFrom::Start(placement.offset as u64))?;
        file.write_all(&placement.bytes)?;
    }
    Ok(())
}
