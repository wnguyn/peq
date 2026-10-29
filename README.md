# FPGA parametric EQ — work in progress

An early attempt at implementing a parametric equalizer on an FPGA.
The current Verilog boilerplate passes audio through unchanged; EQ processing
is still to be implemented.

## Current status

- [x] Signed 16-bit PCM input/output with valid signals and synchronous reset.
- [x] One-clock bypass datapath and a C++ Verilator simulation driver.
- [ ] Fixed-point biquad filter and coefficient representation.
- [ ] Frequency, gain, and Q controls for multiple EQ bands.
- [ ] Rounding, saturation, and comparison against a software reference.
- [ ] FPGA audio interface, clocking, and board integration.

## Layout

- `rtl/peq.v`: Verilog hardware entry point.
- `cpp/main.cpp`: simulation driver reading `input.pcm` and writing `output.pcm`.
- `Makefile`: Verilator build and simulation commands.

GitHub language statistics focus on the Verilog hardware. The supporting C++
driver is excluded with `.gitattributes`.

## Try the bypass

Requires Verilator, Make, a C++17 compiler, and FFmpeg for audio conversion.
From the repository root:

```sh
ffmpeg -i input.flac -ar 48000 -ac 1 -c:a pcm_s16le -f s16le input.pcm
make run
cmp input.pcm output.pcm
ffmpeg -f s16le -ar 48000 -ac 1 -i output.pcm output.wav
```

The bypass should produce byte-identical PCM output. The driver currently
expects a result after each input clock; update it when filter latency changes.
