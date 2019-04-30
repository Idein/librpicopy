# `librpicopy`

Fast implementations of `memcpy` and `memset` especially for Raspberry Pi.


## Requirements

You need [librpimemmgr](https://github.com/Idein/librpimemmgr) and
[libvc4regmap](https://github.com/Terminus-IMRC/libvc4regmap).


## Installation

```
$ git clone https://github.com/Idein/librpicopy.git
$ cd librpicopy/
$ cmake .
$ make package
$ sudo dpkg -i librpicopy-x.y.z-Linux.deb
```


## Benchmark results

Before running `examples/bench`, we did:
- `echo performance | sudo tee /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`
- `tvservice -o`

### Raspberry Pi 1

```
# DMA memcpy
num_dma=1 burst=1: 2.339081e+00 [s], 7.309628e-02 [s], 2.295222e+08 [B/s]
num_dma=1 burst=2: 1.589757e+00 [s], 4.967991e-02 [s], 3.377063e+08 [B/s]
num_dma=1 burst=3: 1.258924e+00 [s], 3.934137e-02 [s], 4.264522e+08 [B/s]
num_dma=2 burst=1: 1.431585e+00 [s], 4.473703e-02 [s], 3.750185e+08 [B/s]
num_dma=3 burst=1: 8.344490e-01 [s], 2.607653e-02 [s], 6.433837e+08 [B/s]
num_dma=4 burst=1: 6.766760e-01 [s], 2.114612e-02 [s], 7.933943e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.521782e+00 [s], 7.880569e-02 [s], 2.128935e+08 [B/s]
num_dma=1 burst=2: 1.710109e+00 [s], 5.344091e-02 [s], 3.139396e+08 [B/s]
num_dma=1 burst=3: 1.350544e+00 [s], 4.220450e-02 [s], 3.975220e+08 [B/s]
num_dma=2 burst=1: 1.587266e+00 [s], 4.960206e-02 [s], 3.382363e+08 [B/s]
num_dma=3 burst=1: 9.617560e-01 [s], 3.005487e-02 [s], 5.582195e+08 [B/s]
num_dma=4 burst=1: 1.242465e+00 [s], 3.882703e-02 [s], 4.321014e+08 [B/s]
```

### Raspberry Pi 2

```
# DMA memcpy
num_dma=1 burst=1: 3.039408e+00 [s], 9.498149e-02 [s], 1.766367e+08 [B/s]
num_dma=1 burst=2: 2.060051e+00 [s], 6.437659e-02 [s], 2.606105e+08 [B/s]
num_dma=1 burst=3: 1.603792e+00 [s], 5.011850e-02 [s], 3.347509e+08 [B/s]
num_dma=2 burst=1: 1.669256e+00 [s], 5.216424e-02 [s], 3.216229e+08 [B/s]
num_dma=3 burst=1: 1.012221e+00 [s], 3.163190e-02 [s], 5.303892e+08 [B/s]
num_dma=4 burst=1: 8.205342e-01 [s], 2.564169e-02 [s], 6.542944e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.506492e+00 [s], 7.832787e-02 [s], 2.141922e+08 [B/s]
num_dma=1 burst=2: 1.702948e+00 [s], 5.321714e-02 [s], 3.152596e+08 [B/s]
num_dma=1 burst=3: 1.339751e+00 [s], 4.186722e-02 [s], 4.007244e+08 [B/s]
num_dma=2 burst=1: 1.655901e+00 [s], 5.174690e-02 [s], 3.242168e+08 [B/s]
num_dma=3 burst=1: 9.431399e-01 [s], 2.947312e-02 [s], 5.692378e+08 [B/s]
num_dma=4 burst=1: 1.281902e+00 [s], 4.005945e-02 [s], 4.188079e+08 [B/s]
```

### Raspberry Pi 3

```
# DMA memcpy
num_dma=1 burst=1: 2.771375e+00 [s], 8.660547e-02 [s], 1.937200e+08 [B/s]
num_dma=1 burst=2: 1.829025e+00 [s], 5.715704e-02 [s], 2.935284e+08 [B/s]
num_dma=1 burst=3: 1.374241e+00 [s], 4.294503e-02 [s], 3.906673e+08 [B/s]
num_dma=2 burst=1: 1.671737e+00 [s], 5.224177e-02 [s], 3.211456e+08 [B/s]
num_dma=3 burst=1: 9.630981e-01 [s], 3.009682e-02 [s], 5.574416e+08 [B/s]
num_dma=4 burst=1: 7.646185e-01 [s], 2.389433e-02 [s], 7.021422e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.229066e+00 [s], 6.965831e-02 [s], 2.408502e+08 [B/s]
num_dma=1 burst=2: 1.474977e+00 [s], 4.609302e-02 [s], 3.639861e+08 [B/s]
num_dma=1 burst=3: 1.114009e+00 [s], 3.481279e-02 [s], 4.819267e+08 [B/s]
num_dma=2 burst=1: 1.379173e+00 [s], 4.309917e-02 [s], 3.892701e+08 [B/s]
num_dma=3 burst=1: 7.411345e-01 [s], 2.316045e-02 [s], 7.243907e+08 [B/s]
num_dma=4 burst=1: 1.351974e+00 [s], 4.224920e-02 [s], 3.971014e+08 [B/s]
```

### Raspberry Pi 3+

```
# DMA memcpy
num_dma=1 burst=1: 2.767517e+00 [s], 8.648489e-02 [s], 1.939901e+08 [B/s]
num_dma=1 burst=2: 1.797841e+00 [s], 5.618254e-02 [s], 2.986197e+08 [B/s]
num_dma=1 burst=3: 1.367642e+00 [s], 4.273882e-02 [s], 3.925522e+08 [B/s]
num_dma=2 burst=1: 1.542834e+00 [s], 4.821356e-02 [s], 3.479772e+08 [B/s]
num_dma=3 burst=1: 9.691148e-01 [s], 3.028484e-02 [s], 5.539807e+08 [B/s]
num_dma=4 burst=1: 7.792779e-01 [s], 2.435243e-02 [s], 6.889339e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.227929e+00 [s], 6.962277e-02 [s], 2.409731e+08 [B/s]
num_dma=1 burst=2: 1.461197e+00 [s], 4.566242e-02 [s], 3.674185e+08 [B/s]
num_dma=1 burst=3: 1.112422e+00 [s], 3.476320e-02 [s], 4.826143e+08 [B/s]
num_dma=2 burst=1: 1.379296e+00 [s], 4.310299e-02 [s], 3.892356e+08 [B/s]
num_dma=3 burst=1: 7.307837e-01 [s], 2.283699e-02 [s], 7.346509e+08 [B/s]
num_dma=4 burst=1: 1.359560e+00 [s], 4.248626e-02 [s], 3.948857e+08 [B/s]
```
