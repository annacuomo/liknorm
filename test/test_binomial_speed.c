#include "liknorm.h"
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

double logaddexp(double x, double y)
{
  double tmp = x - y;

  if (x == y) return x + M_LN2;

  if (tmp > 0) return x + log1p(exp(-tmp));
  else if (tmp <= 0) return y + log1p(exp(tmp));

  return tmp;
}

// A(X) = N log(1 + e^x)
// A'(X) = N e^x / (1 + e^x)
// A''(X) = N e^x / (1 + e^x)^2
void binomial_log_partition(double  theta,
                            double *b0,
                            double *logb1,
                            double *logb2,
                            double *sign)
{
  *b0 = logaddexp(0, theta);

  if (logb1 != 0) *logb1 = theta - *b0;

  if (logb2 != 0) *logb2 = theta - 2 * (*b0);

  if (sign != 0) *sign = +1;
}

int main()
{
  Normal normal = { 0, 1 };
  double A0, logA1, logA2, sign;


  double N  = 10;
  double K  = 5;
  ExpFam ef = { K, 1 / N, &binomial_log_partition };

  double mean, variance;
  double mu;
  double var;

  LikNormMachine *machine = create_liknorm_machine(250, 1e-7);

  N          = 3;
  K          = 1;
  mu         = 0;
  var        = 1;
  ef.y       = K;
  ef.lp      = &binomial_log_partition;
  ef.aphi    = 1 / N;
  normal.tau = 1 / var;
  normal.eta = mu / var;

  int nrep     = 10000;
  double start = get_time();

  for (int i = 0; i < nrep; i++) integrate(machine, ef, normal, &mean, &variance);



  printf("Elapsed time: %.30f\n", (get_time() - start) / nrep);

  destroy_liknorm_machine(machine);

  return 0;
}