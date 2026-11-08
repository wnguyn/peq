use std::f64::consts::PI;
use std::fs::read_to_string;

struct Band {
    b0: f64,
    b1: f64,
    b2: f64,
    a1: f64,
    a2: f64,
}

struct FixedBand {
    b0: i64,
    b1: i64,
    b2: i64,
    a1: i64,
    a2: i64,
}
impl FixedBand {
    fn new(freq: f64, gain: f64, q: f64, sample_rate: f64) -> Self {
        let a = 10.0_f64.powf(gain / 40.0);
        let w = 2.0 * PI * freq / sample_rate;
        let alpha = w.sin() / (2.0 * q);
        let a0 = 1.0 + alpha / a;

        Self {
            b0: Self::fixed((1.0 + alpha * a) / a0),
            b1: Self::fixed(-2.0 * w.cos() / a0),
            b2: Self::fixed((1.0 - alpha * a) / a0),
            a1: Self::fixed(-2.0 * w.cos() / a0),
            a2: Self::fixed((1.0 - alpha / a) / a0),
        }
    }
    fn fixed(value: f64) -> i64 {
        (value * (1_u64 << 30) as f64).round() as i64
    }
}
impl Band {
    fn new(freq: f64, gain: f64, q: f64, sample_rate: f64) -> Self {
        let a = 10.0_f64.powf(gain / 40.0);
        let w = 2.0 * PI * freq / sample_rate;
        let alpha = w.sin() / (2.0 * q);
        let a0 = 1.0 + alpha / a;

        Self {
            b0: (1.0 + alpha * a) / a0,
            b1: -2.0 * w.cos() / a0,
            b2: (1.0 - alpha * a) / a0,
            a1: -2.0 * w.cos() / a0,
            a2: (1.0 - alpha / a) / a0,
        }
    }
}

fn main() {
    let contents = read_to_string("foo.txt").unwrap();
    let mut bands: Vec<FixedBand> = Vec::new();

    for line in contents.lines() {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.first() != Some(&"Filter") {
            continue;
        }
        if parts[2] != "ON" || parts[3] != "PK" {
            continue;
        }

        let freq = parts[5].parse().unwrap();
        let gain = parts[8].parse().unwrap();
        let q = parts[11].parse().unwrap();
        bands.push(FixedBand::new(freq, gain, q, 48000.0));
    }
    for i in &bands {
        println!("{}", i.b0);
        println!("{}", i.b1);
        println!("{}", i.b2);
        println!("{}", i.a2);
        println!("{}", i.a1);
    }
}
