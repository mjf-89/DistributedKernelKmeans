# Distributed Kernel K-Means

## Installation

### Source release

Installation from source files requires:
- cmake > 2.8
- an MPI library implementation e.g. openmpi or MPICH
- [only for GPU support] CUDA-toolkit > 5.0
- [only for GPU support] OpenMP ready compiler

```
mkdir build_directory
cd build_directory
cmake source_directory
ccmake .
make && make install
```

ccmake is used to configure the build process. Important variables to be set are:
- DKK_PLG_CUDA, set it ON to enable GPU support
- DKK_PLG_FERMI, set it ON to enable distributed support
- DKK_REAL, set the real type: float, double, etc ...
- DKK_INTEGER, set the integer type: int, long int, etc ...

### Binary release
Pre-complied on Ubuntu with GCC-4.3.2, CUDA-5.5 and MPICH-3.2, it only requires the installation of:
- MPICH/HYDRA 3.2

```
tar -xvf dkk_bin_release.tar.gz
export LD_LIBRARY_PATH=absolute_path_to/dkk_release/lib:$LD_LIBRARY_PATH
export PATH=absolute_path_to/dkk_release:$PATH
```

## Usage

Multi process CPU version:
```
mpirun -n np ./dkk configuration_file.cfg
```

GPU-enabled version:
```
export OMP_NESTED=TRUE
export OMP_NUM_THREADS=nt
mpirun -n 1 ./dkk configuration_file.cfg
```

## Configuration file KEYWORDS
* PLUGIN_FILES: space separated list of plugin files to be loaded.

* SEED: integer number used to seed the random number generator.

* NC: integer number of clusters to be identified

* READER: string specifying the reader to be used [DCDReader | XTCReader]
  * CRD_FILES: space separated list of coordinates files
  * IDX_FILES: space separated list of atom indices files
  
* KERNEL: string specifying the kernel to be used [GaussianKernel]
  * SIGMA: float specifying gaussian kernel width
  * WORKER: string specifying the low level implementation of the kernel [CUDAWorker | CPURMSDWorker]
  
* INITIALIZER: string specifying the initialization method [RandomInitializer | KPPInitializer]

* ITERATOR: string specifying the kernel k-means iteratoions implementation [SimpleIterator]

* DRIVER: string specifying the mini-batch driver [SubsampleDriver | GPUSubsampleDriver]
  * NB: integer specifying the number of mini-batches
  * SPARSITY: float in [0.0,1.0] specifying the sparsity of the representation

* OUTPUT_PREFIX: path prefix for the output files (in case of path all mid-folders should be created in advance)
* OUTPUT_MEDOIDS: string specifying the output method for the medoids, either as pair of coordinate file / frame index or as explicit atoms coordinates [INDICES | COORDINATES].
* OUTPUT_LABELS: [ON | OFF] 
* OUTPUT_COST: [ON | OFF]

## Configuration file examples

```
#load plugins
PLUGIN_FILES={dkk_plg_default dkk_plg_fermi dkk_plg_cuda} 

#set seed for the random number generator
SEED=3251

#set the number of clusters
NC=20

#set reader
READER=DCDReader CRD_FILES={test_0.dcd test_1.dcd} IDX_FILES={test0.idx test_1.idx}

#set kernel
KERNEL=GaussianKernel SIGMA=120.0 WORKER=CUDAWorker
KERNEL=GaussianKernel SIGMA=120.0 WORKER=CPURMSDWorker

#set initialization method
INITIALIZER=KPPInitializer
INITIALIZER=RandomInitializer

#set iterator method
ITERATOR=SimpleIterator

#set mini-batch driver
DRIVER=GPUSubsampleDriver NB=4 SPARSITY=1.0
DRIVER=SubsampleDriver NB=4 SPARSITY=1.0

#control output behaviour
OUTPUT_PREFIX=output_file_prefix

OUTPUT_MEDOIDS=INDICES
OUTPUT_MEDOIDS=COORDINATES

OUTPUT_LABELS=OFF
OUTPUT_COST=OFF

```
