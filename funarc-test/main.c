// This is a C version of the core test algorithm in main.cpp
// It is meant to be submitted to Precimonious to see what Precimonious does with the problem
// It integrates the dynamics of a ball falling from a few meters above the surface of the Earth

#include <stdio.h>

void gravity( long double* x1, long double* x1v, long double m1, long double x2, long double m2, long double time)
// returns new position of x1 computed by Euler integration
// positions in meters, masses in kg, time in seconds
{
  long double G = 6.6740831e-11;
  // force on x1
  long double r = *x1 - x2;
  long double a1 = G*m2/((r)*(r));
  *x1v = *x1v + a1 * time;
  *x1 = *x1 + *x1v * time;
}

int main(int argc, char* argv) {
  long double earth_radius = 6.371e6;
  long double initial_condition[2] = { 0.1, earth_radius + 1.0 };
  // We construct a model with 2 variables, one for the Earth and another for a dropped ball

  long double earth_position_0 = 0.0;
  long double ball_position_0 = earth_radius;
  long double ball_velocity_0 =  0.0;
  long double ball_position = ball_position_0;
  long double ball_velocity = ball_velocity_0;

  long double earth_mass = 5.972e24;
  long double ball_mass = 0.1;

  for (int i = 0; i < 10; i++) {
    printf("Approximation of ball p/v at second %d is %Lf, %Lf\n", i, ball_position, ball_velocity);
    gravity(&ball_position, &ball_velocity, ball_mass, earth_position_0, earth_mass, 1.0);
  }

  return 0;
}
