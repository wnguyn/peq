use std::fs::read_to_string;

struct Band {
    q: f32,
}
impl Band {
    fn new(contents: String) -> Vec<Band> {
        let mut bands = Vec::new();

        for line in contents.lines() {
            let mut words = line.split_whitespace();
            if words.next() != Some("Filter") {
                continue;
            }

            let q = words
                .skip_while(|word| *word != "Q")
                .nth(1)
                .unwrap()
                .parse::<f32>()
                .unwrap();

            bands.push(Band { q });
        }

        bands
    }
}

fn main() {
    let contents = read_to_string("foo.txt").unwrap();
    let bands: Vec<Band> = Band::new(contents);
    for band in bands {
        println!("{}", band.q);
    }
}
