#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int lover_bound(int *, int, int);
int lover_bound_branchless(int *, int, int);
int lover_bound_prefetch(int *, int, int);
double search_median(double *, int);
double search_avg(double *, int, int);
void choose_alg(int, int *, int, int);

int lover_bound_branchless(int *t, int n, int x) {
  int *base = t, len = n;
  while (len > 1) {
    int half = len / 2;
    base += (base[half - 1] < x) * half;
    len -= half;
  }
  return *base;
}

int lover_bound_prefetch(int *t, int n, int x) {
  int *base = t, len = n;
  while (len > 1) {
    int half = len / 2;
    len -= half;
    __builtin_prefetch(&base[len / 2 - 1]);
    __builtin_prefetch(&base[half + len / 2 - 1]);
    base += (base[half - 1] < x) * half;
  }
  return *base;
}
int compare(const void *a, const void *b) {
  double diff = *(double *)a - *(double *)b;
  return (diff > 0) - (diff < 0);
}

double wtime() {
  struct timespec tsp;
  clock_gettime(CLOCK_MONOTONIC, &tsp);
  return tsp.tv_sec + tsp.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
  int N = (argc > 1) ? atoi(argv[1]) : 1 << 24;
  int choice = (argc > 2) ? atoi(argv[2]) : 1;
  int x = N;
  int nruns = 100;
  double *times = malloc(sizeof(*times) * nruns);
  int *v = malloc(sizeof(*v) * N);

  for (int i = 0; i < N; i++) {
    v[i] = i + 1;
  }

  for (int i = 0; i < 5; i++) {
    choose_alg(choice, v, N, x);
  }

  double total_time = 0.0;
  for (int i = 0; i < nruns; i++) {
    double start = wtime();
    choose_alg(choice, v, N, x);
    double end = wtime();
    times[i] = end - start;
    total_time += times[i];
  }

  double tusec = search_median(times, nruns);
  double tavg = search_avg(times, nruns, 3);
  double res = total_time / nruns;

  printf("# N         t_med [usec]   t_avg [usec]   res[usec]\n");
  printf("%-8d    %.3f  \t %.3f\t%.3f\n", N, tusec * 1e6, tavg * 1e6,
         res * 1e6);

  free(v);
  free(times);
  return 0;
}

int lover_bound(int *t, int n, int x) {
  int l = 0, r = n - 1;
  while (l < r) {
    int m = (l + r) / 2;
    if (t[m] >= x)
      r = m;
    else
      l = m + 1;
  }
  return t[l];
}

double search_median(double *times, int nruns) {
  qsort(times, nruns, sizeof(times[0]), compare);
  return times[nruns / 2];
}

double search_avg(double *times, int nruns, int k) {
  double t = 0;
  int n = 0;
  for (int i = k - 1; i < nruns - k; i++) {
    t += times[i];
    n++;
  }
  return t / n;
}

void choose_alg(int choice, int *t, int n, int x) {
  switch (choice) {
  case 2:
    lover_bound_branchless(t, n, x);
    break;
  case 3:
    lover_bound_prefetch(t, n, x);
    break;
  default:
    lover_bound(t, n, x);
    break;
  }
}
