#define USE_PROFIL	// <-- specify to use PROFIL for interval arithmetic
#undef USE_FILIB	// <-- specify to use FILIB++ for interval arithmetic

////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iomanip>
#include <exception>

#ifdef USE_PROFIL
  #include "mcprofil.hpp"
  typedef INTERVAL I;
#else
  #ifdef USE_FILIB
    #include "mcfilib.hpp"
    typedef filib::interval<double> I;
  #else
    #include "interval.hpp"
    typedef mc::Interval I;
  #endif
#endif

#include "tmodel.hpp"
#include "mccormick.hpp"
typedef mc::McCormick<I> MC;

typedef mc::TModel<I> TM;
typedef mc::TVar<I> TV;

typedef mc::TModel<MC> TMMC;
typedef mc::TVar<MC> TVMC;

using namespace std;
using namespace mc;

void gravity( TVMC& x1, TVMC& x1v, long double m1, TVMC x2, long double m2, long double time)
// returns new position of x1 computed by Euler integration
// positions in meters, masses in kg, time in seconds
{
  long double G = 6.6740831e-11;
  // force on x1
  TVMC r = x1 - x2;
  TVMC a1 = G*m2/((r)*(r));
  x1v = x1v + a1 * time;
  x1 = x1 + x1v * time;
}

int main()
{
  // <-- set options here -->
  MC::options.ENVEL_USE=true;
  MC::options.ENVEL_MAXIT=100;
  MC::options.ENVEL_TOL=1e-12;
  MC::options.MVCOMP_USE=true;

#ifdef SAVE_RESULTS
  ofstream res( "funarc.out", ios_base::out );
  res << scientific << setprecision(5) << right;
#endif

  try{
    double earth_radius = 6.371e6;
    double initial_condition[2] = { 0.1, earth_radius + 1.0 };
    // We construct a model with 2 variables, one for the Earth and another for a dropped ball
    TMMC model( 3, 2 );
    TVMC earth_position_0( &model, 0, MC( I(-1.0, 1.0), 0.0 ) );
    TVMC ball_position_0 = TVMC( &model, 1, MC( I(earth_radius - 1.0, earth_radius + 10.0), earth_radius ) );
    TVMC ball_velocity_0 = TVMC( &model, 2, MC( I(-1, 1), 0.0 ) );
    TVMC ball_position = ball_position_0;
    TVMC ball_velocity = ball_velocity_0;

    long double earth_mass = 5.972e24;
    long double ball_mass = 0.1;

    for (int i = 0; i < 10; i++) {
        cout << "Approximation of ball p/v at second " << i << ": " << ball_position << endl << ball_velocity << endl;
        cout << "Evaluated ball p/v at second " << i << ": " << endl << ball_position.P(initial_condition) << endl << ball_velocity.P(initial_condition) << endl;
        gravity(ball_position, ball_velocity, ball_mass, earth_position_0, earth_mass, 1.0);
    }
    // evaluate position for initial conditions of ball at earth radius + 1 and earth at zero
    long double result = ball_position.P(initial_condition);
    cout << "Result: " << result << endl;
  }

#ifndef USE_PROFIL
#ifndef USE_FILIB
  catch( I::Exceptions &eObj ){
    cerr << "Error " << eObj.ierr()
         << " in natural interval extension:" << endl
	 << eObj.what() << endl
         << "Aborts." << endl;
    return eObj.ierr();
  }
#endif
#endif
  catch( MC::Exceptions &eObj ){
    cerr << "Error " << eObj.ierr()
         << " in McCormick relaxation:" << endl
	 << eObj.what() << endl
         << "Aborts." << endl;
    return eObj.ierr();
  } catch (exception& e) {
    cerr << "Error:" << endl
         << e.what() << endl
         << "Aborts." << endl;
  }

#ifdef SAVE_RESULTS
  res.close();
#endif
  return 0;
}
