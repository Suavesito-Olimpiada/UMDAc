#include "random.h"

double random_uniform()
{
    return (double) rand() / RAND_MAX; // By no mean the best, but the simplest.
}

double random_uniform_ab(double a, double b)
{
    return a + random_uniform() * (b - a);
}

double random_normal()
{
    static int call;
    static double x, y;
    double u, v, s;

    if (call == 1) {
        call = 0;
        return y;
    }

    do {
        u = random_uniform_ab(-1.0, 1.0);
        v = random_uniform_ab(-1.0, 1.0);
        s = u*u + v*v;
    } while (s >= 1 || s == 0);
    x = u * sqrt((-2 * log(s)) / s);
    y = v * sqrt((-2 * log(s)) / s);

    call = 1;

    return x;
}

double random_normal_mu_si(double mu, double sigma)
{
    return mu + sigma * random_normal();
}
