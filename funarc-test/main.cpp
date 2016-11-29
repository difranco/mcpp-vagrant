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

TV fun( TV x )
{

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
    // ASSUME a finite-dimensional input space so as to be able to set the number of independent variables in the model
    TMMC model( 1, 4 );
    TVMC X( &model, 0, MC( I(0.0, 6.3), 3.14 ) );
    TVMC Y = X;
    for (int i = 0; i < 10; i++) {
        Y = Y + 1;
        cout << "Approximation info at iteration " << i << ": " << Y << endl;
    }
    double x[1] = { 1.0 };
    long double result = Y.P(x);
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
