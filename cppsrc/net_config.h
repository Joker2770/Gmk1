/*
    Extended from code:
    Copyright (C) 2017 Gian-Carlo Pascutto

    Leela Zero is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leela Zero is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela Zero.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

/*
 * Features
 *
 * USE_BLAS: Use a basic linear algebra library.
 * We currently require this, as not all operations are performed on
 * the GPU - some operations won't get any speedup from it.
 * Also used for OpenCL self-checks.
 */
#define USE_BLAS
/*
 * We use OpenBLAS by default, except on macOS, which has a fast BLAS
 * built-in. (Accelerate)
 */
#if !defined(__APPLE__) && !defined(__MACOSX)
#define USE_OPENBLAS
#endif
/*
 * USE_MKL: Optionally allows using Intel Math Kernel library as
 * BLAS implementation. Note that MKL's license is not compatible with the GPL,
 * so do not redistribute the resulting binaries. It is fine to use it on your
 * own system.
 */
//# define USE_MKL
/*
 * USE_OPENCL: Use OpenCL acceleration for GPUs. This makes the program a lot
 * faster if you have a recent GPU. Don't use it on CPUs even if they have
 * OpenCL drivers - the BLAS version is much faster for those.
 * If you don't have a GPU, don't define USE_OPENCL, then the network will use cpu implement.
 */
#define USE_OPENCL

/*
 * OpenBLAS limitation: the default configuration on some Linuxes
 * is limited to 64 cores.
 */
#if defined(USE_BLAS) && defined(USE_OPENBLAS)
#define MAX_CPUS 64
#else
#define MAX_CPUS 128
#endif

using net_t = float;

#if defined(USE_BLAS) && defined(USE_OPENCL) && !defined(USE_HALF)
// If both BLAS and OpenCL are fully usable, then check the OpenCL
// results against BLAS with some probability.
#define USE_OPENCL_SELFCHECK
#define SELFCHECK_PROBABILITY 2000
#endif

#if (_MSC_VER >= 1400) /* VC8+ Disable all deprecation warnings */
    #pragma warning(disable : 4996)
#endif /* VC8+ */

#endif
