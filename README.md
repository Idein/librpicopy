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
num_dma=1 burst=1: 2.339082e+00 [s], 7.309631e-02 [s], 2.295221e+08 [B/s]
num_dma=1 burst=2: 1.590297e+00 [s], 4.969678e-02 [s], 3.375916e+08 [B/s]
num_dma=1 burst=3: 1.258913e+00 [s], 3.934103e-02 [s], 4.264559e+08 [B/s]
num_dma=2 burst=1: 1.451011e+00 [s], 4.534409e-02 [s], 3.699978e+08 [B/s]
num_dma=3 burst=1: 8.352830e-01 [s], 2.610259e-02 [s], 6.427413e+08 [B/s]
num_dma=4 burst=1: 6.734340e-01 [s], 2.104481e-02 [s], 7.972139e+08 [B/s]
num_dma=5 burst=1: 6.634940e-01 [s], 2.073419e-02 [s], 8.091571e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.521621e+00 [s], 7.880066e-02 [s], 2.129071e+08 [B/s]
num_dma=1 burst=2: 1.710269e+00 [s], 5.344591e-02 [s], 3.139102e+08 [B/s]
num_dma=1 burst=3: 1.350440e+00 [s], 4.220125e-02 [s], 3.975526e+08 [B/s]
num_dma=2 burst=1: 1.598383e+00 [s], 4.994947e-02 [s], 3.358838e+08 [B/s]
num_dma=3 burst=1: 9.613610e-01 [s], 3.004253e-02 [s], 5.584488e+08 [B/s]
num_dma=4 burst=1: 1.310814e+00 [s], 4.096294e-02 [s], 4.095706e+08 [B/s]
num_dma=5 burst=1: 6.970670e-01 [s], 2.178334e-02 [s], 7.701855e+08 [B/s]
```

### Raspberry Pi 2

```
# DMA memcpy
num_dma=1 burst=1: 3.039420e+00 [s], 9.498187e-02 [s], 1.766360e+08 [B/s]
num_dma=1 burst=2: 2.059771e+00 [s], 6.436784e-02 [s], 2.606459e+08 [B/s]
num_dma=1 burst=3: 1.603887e+00 [s], 5.012146e-02 [s], 3.347312e+08 [B/s]
num_dma=2 burst=1: 1.669284e+00 [s], 5.216512e-02 [s], 3.216175e+08 [B/s]
num_dma=3 burst=1: 1.010008e+00 [s], 3.156275e-02 [s], 5.315511e+08 [B/s]
num_dma=4 burst=1: 8.206183e-01 [s], 2.564432e-02 [s], 6.542273e+08 [B/s]
num_dma=5 burst=1: 6.900214e-01 [s], 2.156317e-02 [s], 7.780497e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.506677e+00 [s], 7.833367e-02 [s], 2.141763e+08 [B/s]
num_dma=1 burst=2: 1.703087e+00 [s], 5.322147e-02 [s], 3.152340e+08 [B/s]
num_dma=1 burst=3: 1.339909e+00 [s], 4.187217e-02 [s], 4.006770e+08 [B/s]
num_dma=2 burst=1: 1.656958e+00 [s], 5.177993e-02 [s], 3.240100e+08 [B/s]
num_dma=3 burst=1: 9.421253e-01 [s], 2.944142e-02 [s], 5.698509e+08 [B/s]
num_dma=4 burst=1: 1.280455e+00 [s], 4.001420e-02 [s], 4.192815e+08 [B/s]
num_dma=5 burst=1: 6.945343e-01 [s], 2.170420e-02 [s], 7.729941e+08 [B/s]
```

### Raspberry Pi 3

```
# DMA memcpy
num_dma=1 burst=1: 2.773088e+00 [s], 8.665901e-02 [s], 1.936004e+08 [B/s]
num_dma=1 burst=2: 1.832027e+00 [s], 5.725083e-02 [s], 2.930476e+08 [B/s]
num_dma=1 burst=3: 1.374076e+00 [s], 4.293988e-02 [s], 3.907141e+08 [B/s]
num_dma=2 burst=1: 1.657935e+00 [s], 5.181048e-02 [s], 3.238190e+08 [B/s]
num_dma=3 burst=1: 9.642743e-01 [s], 3.013357e-02 [s], 5.567616e+08 [B/s]
num_dma=4 burst=1: 7.669339e-01 [s], 2.396668e-02 [s], 7.000224e+08 [B/s]
num_dma=5 burst=1: 6.548098e-01 [s], 2.046281e-02 [s], 8.198883e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.229582e+00 [s], 6.967443e-02 [s], 2.407944e+08 [B/s]
num_dma=1 burst=2: 1.477440e+00 [s], 4.617001e-02 [s], 3.633791e+08 [B/s]
num_dma=1 burst=3: 1.114157e+00 [s], 3.481742e-02 [s], 4.818627e+08 [B/s]
num_dma=2 burst=1: 1.379145e+00 [s], 4.309829e-02 [s], 3.892780e+08 [B/s]
num_dma=3 burst=1: 7.406436e-01 [s], 2.314511e-02 [s], 7.248707e+08 [B/s]
num_dma=4 burst=1: 1.353069e+00 [s], 4.228340e-02 [s], 3.967802e+08 [B/s]
num_dma=5 burst=1: 5.689845e-01 [s], 1.778077e-02 [s], 9.435598e+08 [B/s]
```

### Raspberry Pi 3+

```
# DMA memcpy
num_dma=1 burst=1: 2.767498e+00 [s], 8.648431e-02 [s], 1.939914e+08 [B/s]
num_dma=1 burst=2: 1.800424e+00 [s], 5.626326e-02 [s], 2.981913e+08 [B/s]
num_dma=1 burst=3: 1.367743e+00 [s], 4.274196e-02 [s], 3.925234e+08 [B/s]
num_dma=2 burst=1: 1.566099e+00 [s], 4.894058e-02 [s], 3.428078e+08 [B/s]
num_dma=3 burst=1: 9.702759e-01 [s], 3.032112e-02 [s], 5.533178e+08 [B/s]
num_dma=4 burst=1: 7.804515e-01 [s], 2.438911e-02 [s], 6.878979e+08 [B/s]
num_dma=5 burst=1: 6.495056e-01 [s], 2.029705e-02 [s], 8.265839e+08 [B/s]
# DMA memset
num_dma=1 burst=1: 2.227883e+00 [s], 6.962134e-02 [s], 2.409781e+08 [B/s]
num_dma=1 burst=2: 1.462203e+00 [s], 4.569385e-02 [s], 3.671657e+08 [B/s]
num_dma=1 burst=3: 1.112389e+00 [s], 3.476217e-02 [s], 4.826286e+08 [B/s]
num_dma=2 burst=1: 1.379112e+00 [s], 4.309725e-02 [s], 3.892874e+08 [B/s]
num_dma=3 burst=1: 7.270880e-01 [s], 2.272150e-02 [s], 7.383851e+08 [B/s]
num_dma=4 burst=1: 1.353650e+00 [s], 4.230158e-02 [s], 3.966097e+08 [B/s]
num_dma=5 burst=1: 5.668983e-01 [s], 1.771557e-02 [s], 9.470321e+08 [B/s]
```
