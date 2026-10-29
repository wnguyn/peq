super kool verilog command to do that ore whatever:

a bad attempt at learning verilog for fun. i like using parametric eq so i would like to know how it actually works and it seems like a good first fpga project of something i actually use.
```sh
ffmpeg -i input.flac -ar 48000 -ac 1 -c:a pcm_s16le -f s16le input.pcm
make run
cmp input.pcm output.pcm
ffmpeg -f s16le -ar 48000 -ac 1 -i output.pcm output.wav
```
