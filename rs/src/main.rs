use clap::*;
use std::{fs, io};
// make stupid cli later???

struct Band {
    b0: f32,
    b1: f32,
    b2: f32,
    a1: f32,
    a2: f32,
}
impl Band {
    fn new(str: String) -> Vec<Band> {
        // do something???
    }
}

fn main() -> io::Result<()> {
    let contents = std::fs::read_to_string("foo.txt")?;
    let foo: Vec<Band> = Band::new(contents);
}
