#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include <math.h>

#ifdef WIN32

# include <windows.h>
double get_time()
{
  LARGE_INTEGER t, f;

  QueryPerformanceCounter(&t);
  QueryPerformanceFrequency(&f);
  return (double)t.QuadPart / (double)f.QuadPart;
}

#else /* ifdef WIN32 */

# include <sys/time.h>
# include <sys/resource.h>

double get_time()
{
  struct timeval  t;
  struct timezone tzp;

  gettimeofday(&t, &tzp);
  return t.tv_sec + t.tv_usec * 1e-6;
}

#endif /* ifdef WIN32 */

#endif
