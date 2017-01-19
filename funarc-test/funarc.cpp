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

TVMC fun( TVMC x )
{
  int k, n = 5;
  TVMC t1 = x;
  double d1 = 1.0;

  for( k = 1; k <= n; k++ )
  {
    d1 = 2.0 * d1;
    t1 = t1 + sin (d1 * x) / d1;
  }

  return t1;
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
    TMMC model( 4, 2 );

    int i, n = 1000000;
    double dppi, ans = 5.795776322412856;
    TVMC h, t1, t2;
    TVMC s1;
    // double threshold = 1e-14L;

    dppi = acos(-1.0);
    s1 = TVMC( &model, 1, MC( I(0.0 + dppi/(2*n), dppi), dppi/2 ) );
    t1 = TVMC( &model, 2, MC( I(0.0 + dppi/(2*n), dppi), dppi/2 ) );
    h = TVMC( &model, 0, MC( I(0.0 + dppi/(2*n), dppi), dppi/2 ) );

    for( i = 1; i <= n; i++ )
    {
      t2 = fun (i * h);
      s1 = s1 + sqrt (h*h + (t2 - t1)*(t2 - t1));
      t1 = t2;
    }

    // cout << " => total error: %d %1.15Le %1.15Le\n" << (long double) fabs(ans-s1) > threshold << (long double) fabs(ans-s1) << threshold << endl;
    // cout << " => ans: %1.15Le\n =>  s1: %1.15Le\n" << (long double)ans <<  (long double)s1 << endl;
    cout << "Result: " << ans << endl;
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
