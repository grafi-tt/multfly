# Multfly pseudo-random number generator
NOTE: it is still in very early stage of development.

This repository contains reference and optimized implementations of Multfly pseudo-random number generator. Their API is intended to be minimal and not much end-user friendly.

## Why do you develop it?
My primary motivation is to make a pseudo-random number generator (PRNG) suitable for deep neural network training. Though speed requirement is modest compared to Monte-Carlo simulation, there are some challenges.

* Massively parallel execution: the largest GPGPU supercomputers consist of millions of CUDA cores. Each CUDA core will execute logically different threads (arranged as grids), so the total number of threads can be in the order of trillion. Bookkeeping appropriate number of PRNG instances is not trivial task.
* Heterogeneous execution: in deep neural network training, it is not only GPUs who need random numbers. In typical case, CPUs also relies on them for dataset sampling and data augmentation. You might want to move data augmentation routines to GPUs for performance. But how to preserve determinism?
* Importance of determinism: whereas determinism given by seed benefits every randomized program, in machine learning domain it is especially beneficial. Performance of some algorithm on some data can heavily depend on random sampling results. Since we couldn't know in advance what algorithm and data have such property, we should be conservative.
* Non-deterministic scheduling: it is not fixed what processor is assigned to each data. How to generate the same number sequence regardless of scheduling? Shared PRNG states come at a cost.
* Memory heavy: except convolution and matrix product, current deep neural networks are bottlenecked by memory bandwidth. Well, there is a good news: we can reduce many memory accesses by eliminating temporary arrays (loop fusion.) PRNG should be friendly to such optimization.
* Development model: deep neural networks are rarely implemented by numerical computation experts. So API must be easy to use.

There are shortage in existing designs.

* Linear feedback shift register, multiple recursive generators, and linear congruential generator: those are not perfectly suitable for parallel execution. Even though they have "jumpahead" functionality, jump distance and computation parallelism must be carefully chosen for performance. There are libraries that achieves admirable performance, but relying on them often introduce temporal arrays. They also tend to have distinguishable patterns.
* Counter-based PRNG: actually it is based on block cipher. It accepts k-bit key and k-bit counter, and generates k-bit output. The reasonable choice would be k=128, as 128-bit counter is enough for decades... It's true if users wisely split the 128-bit counter space, e.g. 32 bits to count iterations, 16 bits to number each layer, 16 bits for rejection sampling and the other 64 bits for parallelism. But such splitting requires users' effort. I don't want to impose such a non-essential effort. Generating 128-bit keys by a hash function risks collision. Again, I don't want to force users not to generate 2^64 keys for decades. Setting k=256 slows down everything, despite users often need just 32 bits.

## Install
Just copy `include/*` to `/usr/local/include` or somewhere.

## Usage
Currently there is no documentation. See `tests/*`.

## Build
`make all` builds test commands. You need [TestU01 1.23](http://simul.iro.umontreal.ca/testu01/tu01.html) to build BigCrush. You need CUDA to build `test/test_cuda`.

## Performance
I'll add benchmark result. Expected to be fast on GPU and multi-core SIMD CPU. Sequential implementation should be not much slow.

## Design
* 256-bit key and 128-bit counter, generating 128-bit number
* Efficient in 4-way SIMD
* The counter is split into 64-bit thread index, 32-bit sequential counter and 32-bit auxiliary bits
* ARX-like network
* Combined with multiplication and butterfly-indexing like communication, which is origin of the name Multfly
* Not taking high 32-bit of multiplication to avoid performance drop on some architectures (e.g. NVidia GPU after Maxwell gen.)
* Keys are generated from 256-bit name and 128-bit user value
* I intend that a unique name is set to every neural net layer.
* The user value is split into 64-bit global counter, which can be e.g. iteration number, and 64-bit global seed
* Key generation is not slow on any platform, thanks to Chacha8 cipher
* Key splitting is also supported, because we wouldn't like to name everywhere we generate random numbers

## TODO
* DNN training example using Chainer, CuPy and Cython (planed to be on another repository)
* benchmark
* algorithm fix
* more documentation
* AVX2 implementation
* API fix
