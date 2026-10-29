#include "Vpeq.h"
#include "verilated.h"

#include <cstdint>
#include <fstream>
#include <iostream>



// i don't know what this does lol
static void tick(Vpeq& circuit)
{
    circuit.clk = 0;
    circuit.eval();
    circuit.clk = 1;
    circuit.eval();
}

int main(int argc, char** argv)
{
    VerilatedContext context;
    context.commandArgs(argc, argv);
    Vpeq dut{&context};

    // random thing that i ffmpeg into raw pcm
    std::ifstream input("input.pcm", std::ios::binary);
    std::ofstream output("output.pcm", std::ios::binary);

    dut.clk = 0;
    dut.rst = 1;
    dut.sample_valid = 0;
    dut.sample_in = 0;
    tick(dut);
    dut.rst = 0;


    // this is little endian i think.. if only rust you can just do u16::from_le or whatever
    char bytes[2];
    while (input.read(bytes, sizeof(bytes)))
    {
        dut.sample_in = static_cast<std::uint8_t>(bytes[0]) |
                        (static_cast<std::uint8_t>(bytes[1]) << 8);
        dut.sample_valid = 1;
        tick(dut);
        if (!dut.sample_out_valid)
        {
            std::cerr << "Expected a bypass sample after one clock\n";
            return 1;
        }
        const std::uint16_t sample = dut.sample_out;
        bytes[0] = std::static_cast<char>(sample & 0xff);
        bytes[1] = std::static_cast<char>(sample >> 8);
        if (!output.write(bytes, sizeof(bytes)))
        {
            std::cerr << "Could not write output.pcm\n";
            return 1;
        }
    }
    output.flush();
    dut.sample_valid = 0;
    tick(dut);
    dut.final();
    return 0;
}
