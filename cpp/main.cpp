#include "Vpeq.h"
#include "verilated.h"

#include <cstdint>
#include <fstream>
#include <iostream>

static void tick(Vpeq& circuit) {
    circuit.clk = 0;
    circuit.eval();
    circuit.clk = 1;
    circuit.eval();
}

int main(int argc, char** argv) {
    VerilatedContext context;
    context.commandArgs(argc, argv);
    Vpeq dut{&context};
    std::ifstream input("input.pcm", std::ios::binary);
    std::ofstream output("output.pcm", std::ios::binary);
    if (!input || !output) {
        std::cerr << "Could not open input.pcm or output.pcm\n";
        return 1;
    }

    dut.clk = 0;
    dut.rst = 1;
    dut.sample_valid = 0;
    dut.sample_in = 0;
    tick(dut);
    dut.rst = 0;

    // Decode and encode explicitly: PCM files use little-endian samples.
    char bytes[2];
    while (input.read(bytes, sizeof(bytes))) {
        dut.sample_in = static_cast<std::uint8_t>(bytes[0]) |
                        (static_cast<std::uint8_t>(bytes[1]) << 8);
        dut.sample_valid = 1;
        tick(dut);
        if (!dut.sample_out_valid) {
            std::cerr << "Expected a bypass sample after one clock\n";
            return 1;
        }
        const std::uint16_t sample = dut.sample_out;
        bytes[0] = static_cast<char>(sample & 0xff);
        bytes[1] = static_cast<char>(sample >> 8);
        if (!output.write(bytes, sizeof(bytes))) {
            std::cerr << "Could not write output.pcm\n";
            return 1;
        }
    }
    if (!input.eof() || input.gcount() != 0) {
        std::cerr << "Could not read a complete 16-bit PCM sample\n";
        return 1;
    }
    output.flush();
    if (!output) {
        std::cerr << "Could not flush output.pcm\n";
        return 1;
    }
    dut.sample_valid = 0;
    tick(dut);
    dut.final();
    return 0;
}
