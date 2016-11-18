#define USE_PROFIL	// <-- specify to use PROFIL for interval arithmetic
#undef USE_FILIB	// <-- specify to use FILIB++ for interval arithmetic

////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iomanip>

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
  int k, n = 5;
  long double d1 = 1.0L;

  TV t1;

  t1 = x;

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

    int i, n = 1000000;
    TV t1, t2;
    long double acosarg, h, dppi, ans = 5.795776322412856L;
    long double s1, threshold = 1e-14L;

    acosarg = -1.0;
    dppi = acos(acosarg);
    s1 = 0.0;
    h = dppi / n;

    TM model( i, 4 );
    TV X( &model, 0, I(0.0,dppi) );

    for( i = 1; i < n; i++ )
    {
      long double left = (i-1)*h, center = i*h, right = (i+1)*h;
      TV tmh( &model, i, I( left, right ));
      t2 = fun (tmh);
      t1 = t2;
    }

    MC B = t1.B();
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
  }

#ifdef SAVE_RESULTS
  res.close();
#endif
  return 0;
}
