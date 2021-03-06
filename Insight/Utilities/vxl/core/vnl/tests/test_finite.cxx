//:
// \file
// Tests for vnl_finite.h, written by Peter Vanroose, 6 May 2002.

#include <vcl_iostream.h>
#include <vcl_cstdlib.h> // for atoi()
#include <testlib/testlib_test.h>
#include <vnl/vnl_finite.h>

template <int N>
void test_finite_int(vnl_finite_int<N>)
{
  vcl_cout << "\n --- Testing vnl_finite_int<" << N << "> ---\n";

  vcl_cout << "Phi(" << N << ") = " << vnl_finite_int<N>::totient() << '\n';
  vnl_finite_int<N> g = vnl_finite_int<N>::smallest_generator();
  if (g != 1) {
    vcl_cout << "Smallest multiplicative generator of Z_" << N << " is " << g << '\n';
    TEST("g^(Phi(N)/2) == -1", g.pow(vnl_finite_int<N>::totient()/2), -1);
  }
  if (N <= 1000) { // calculating multiplicative order is time consuming
    unsigned int m = vnl_finite_int<N>(2).multiplicative_order();
    vcl_cout << "Order of 2 in Z_" << N << " is ";
    if (m==(unsigned int)(-1)) vcl_cout << "invalid\n"; else vcl_cout << m << '\n';
  }

  vcl_cout << "\nStarting constructor tests:\n";

  {vnl_finite_int<N> b(0); TEST("vnl_finite_int<N> b(0);", b, 0);}
  {vnl_finite_int<N> b(1); TEST("vnl_finite_int<N> b(1);", b, 1);}
  {vnl_finite_int<N> b(-1); TEST("vnl_finite_int<N> b(-1);", b, -1);}
  {vnl_finite_int<N> b(-1); TEST("b == N-1;", b, N-1);}
  {vnl_finite_int<N> b(111); TEST("vnl_finite_int<N> b(111);", b, 111);}
  {vnl_finite_int<N> b(-99); TEST("vnl_finite_int<N> b(-99);", b, -99);}

  vcl_cout << "\nStarting assignment tests:\n";
  vnl_finite_int<N> b1; b1 = 2;
  TEST("vnl_finite_int<N> b1; b1 = 2;", b1, 2);
  b1 = 10;
  TEST("vnl_finite_int<N> b1; b1 = 10;", b1, 10-N);
  b1 = -77;
  TEST("vnl_finite_int<N> b1; b1 = -77;", b1, 11*N-77);

  vnl_finite_int<N> b5 = 4321; b1 = b5;
  TEST("b1 = b5", b1, b5);

  vcl_cout << "b1 = " << b1 << vcl_endl
           << "b5 = " << b5 << vcl_endl;
  TEST("<<", 1, 1);

  TEST("unary plus", +b5, b5);

  vcl_cout << "\nStarting logical comparison tests:\n";
  vnl_finite_int<N> b0=0; vcl_cout << "b0=" << b0 << '\n';
  b1=1; vcl_cout << "b1=" << b1 << '\n';
  vnl_finite_int<N> b2=2; vcl_cout << "b2=" << b2 << '\n';
  vnl_finite_int<N> b3=-2; vcl_cout << "b3=" << b3 << '\n';

  TEST("b0 == b0", b0 == b0, true);
  TEST("b0 == b1", b0 == b1, false);
  TEST("b0 == b2", b0 == b2, N==2);
  TEST("b0 == b3", b0 == b3, N==2);
  TEST("b1 == b2", b1 == b2, false);
  TEST("b1 == b3", b1 == b3, N==3);
  TEST("b2 == b3", b2 == b3, N==2 || N==4);

  TEST("b0 != b0", b0 != b0, false);
  TEST("b0 != b1", b0 != b1, true);
  TEST("b0 != b2", b0 != b2, N!=2);
  TEST("b0 != b3", b0 != b3, N!=2);
  TEST("b1 != b2", b1 != b2, true);
  TEST("b1 != b3", b1 != b3, N!=3);
  TEST("b2 != b3", b2 != b3, N!=2 && N!=4);

  TEST("b3 != b2", b3 != b2, N!=2 && N!=4);
  TEST("b3 != b3", b3 != b3, false);

  vcl_cout << "\nStarting addition, subtraction tests:\n";

  vnl_finite_int<N> bi,bj,bij;
  vcl_cout << " for (i = 1; i < 1000; i *= 3)\n"
           << "   for (j = 1; j < 1000; j *= 3)\n      ";

  for (int i = 1; i < 1000;  i *= 3) {
    for (int j = 1; j < 1000; j *= 3) {
      bi = i; bj = j;
      bij = vnl_finite_int<N>(i+j);
      if (bi + bj != bij) {
        TEST("bi + bj == vnl_finite_int<N>(i + j)", false, true);
        vcl_cout << "i = "<<i<<", j = "<<j<<'\n';
      }
      bij = vnl_finite_int<N>(i-j);
      if (bi - bj != bij) {
        TEST("bi - bj == vnl_finite_int<N>(i - j)", false, true);
        vcl_cout << "i = "<<i<<", j = "<<j<<'\n';
      }
    }
  }
  vcl_cout << vcl_endl;

  vnl_finite_int<N> b1000(1000);

  TEST("-b0 == b0", -b0, b0);
  TEST("-b1000 == (-1)*b1000", -b1000, (-1)*b1000);
  TEST("-(-b1000) == b1000", -(-b1000), b1000);
  TEST("b0 + b1000 == b1000", b0 + b1000, b1000);
  TEST("b1000 + b0 == b1000", b1000 + b0, b1000);
  TEST("b0 + (-b1000) == -b1000", b0 + (-b1000), -b1000);
  TEST("-b1000 + b0 == -b1000", -b1000 + b0, -b1000);
  TEST("-b1000 + (-b1000) == 2*(-b1000))", -b1000 + (-b1000), 2*(-b1000));
  TEST("b1000 + (-b1000) == b0", b1000 + (-b1000), b0);

  TEST("b0 - b1000 == -b1000", b0 - b1000, -b1000);
  TEST("b1000 - b0 == b1000", b1000 - b0, b1000);
  TEST("b0 - (-b1000) == b1000", b0 - (-b1000), b1000);
  TEST("-b1000 - b0 == -b1000", -b1000 - b0, -b1000);
  TEST("-b1000 - (-b1000) == b0", -b1000 - (-b1000), b0);
  TEST("-b1000 - b1000 == -2*b1000", -b1000 - b1000, -2*b1000);
  TEST("b1000 - (-b1000) == 2*b1000", b1000 - (-b1000), 2*b1000);

  b2 = 1237; // should not be divisible by 2, 3, 5, 7, 11
  b3 = 4321;
  TEST("(b2+b3) - b2 = b3", (b2+b3) - b2, b3);
  TEST("(b2+b3) - b3 = b2", (b2+b3) - b3, b2);

  vcl_cout << "\nStarting multiplication tests:\n";

  TEST("b0*b0 == b0", b0*b0, b0);
  TEST("b0*b1000 == b0", b0*b1000, b0);
  TEST("b1000*b0 == b0", b1000*b0, b0);
  vnl_finite_int<N> b1000000(1000000);
  TEST("b1000*b1000 == b1000000", b1000*b1000, b1000000);
  TEST("b1000*b1000000 == b1000000*b1000", b1000*b1000000, b1000000*b1000);

  if (b1000.is_unit()) {
    vnl_finite_int<N> bb = 1;
    for (int phi = vnl_finite_int<N>::totient(); phi > 0; --phi) bb *= b1000;
    TEST("b1000^Phi(N) == 1", bb, 1);
  }

  vcl_cout << "\nStarting division tests:\n";

  TEST("b0/b1", b0/b1, 0);
  TEST("(-b1)/b1", (-b1)/b1, -1);

  TEST("(b2*b3) / b3 = b2", (b2*b3) / b3, b2);
  TEST("(b2*b3) / b2 = b3", (b2*b3) / b2, b3);

  // Do not continue when N is too large, since that would take too long
  if (N > 1000) return;

  vcl_cout << " for (i = 10000; i > 0; i /= 3)\n"
           << "   for (j = 10000; j > 0; j /= 3)\n"
           << "     for (k = 1; k < 17; ++k)\n"
           << "       for (l = 1; l < 17; ++l)\n         ";
  for (int i = 10000; i > 0; i /= 3) {
    for (int j = 10000; j > 0; j /= 3) {
      for (int k = 1; k < 17; ++k) {
        for (int l = 1; l < 17; ++l) {
          b1 = vnl_finite_int<N>(i+k);
          b2 = vnl_finite_int<N>(j+l);
          if (b2.is_zero_divisor()) continue; // division by zero divisors is undefined
          b3 = b1/b2;
          int r = int(b3); r = r*(j+l)-(i+k);
          if (r%N) {
            TEST("(vnl_finite_int<N>(i+k)/vnl_finite_int<N>(j+l))", false, true);
            vcl_cout<< "i="<<i<<", j="<<j<<", k="<<k<<", l="<<l<<'\n'
                    << "b1="<<int(b1)<<", b2="<<int(b2)<<", b3="<<int(b3)<<'\n';
          }
        }
      }
    }
  }
  vcl_cout << vcl_endl;
}

template <int N, int M>
void test_finite_poly(vnl_finite_int_poly<N,M>, vcl_string s)
{
  vcl_cout << "\n --- Testing vnl_finite_int_poly<" << N << ',' << M << "> ---\n";

  vcl_cout << "\nStarting constructor tests:\n";

  vnl_finite_int_poly<N,M> b0; vcl_cout << b0 << '\n';
  TEST("vnl_finite_int_poly<N,M> b0;", b0, 0);
  vcl_vector<vnl_finite_int<N> > p(1); p[0]=1;
  vnl_finite_int_poly<N,M> b(p); vcl_cout << b << '\n';
  TEST("vnl_finite_int_poly<N,M> b(p);", b, 1);

  vcl_cout << "\nStarting assignment tests:\n";
  vnl_finite_int<N> b1 = 2; b = b1;
  TEST("vnl_finite_int<N> b1; b = b1;", b, 2);

  TEST("unary plus", +b, b);
  TEST("unary minus", -b, N-2);

  vcl_cout << "\nStarting addition, subtraction tests:\n";

  vcl_cout << " for (i = 1; i < 1000; i *= 3)\n"
           << "   for (j = 1; j < 1000; j *= 3)\n      ";

  for (int i = 1000; i > 0;  i /= 3) {
    for (int j = 1000; j > 0; j /= 3) {
      vcl_vector<vnl_finite_int<N> > v1(M), v2(M), v3(M);
      for (int m = 0; m < M;  ++m) {
        v1[m] = i+m; v2[m] = j+m, v3[m] = i+j+2*m;
      }
      vnl_finite_int_poly<N,M> p1(v1);
      vnl_finite_int_poly<N,M> p2(v2);
      vnl_finite_int_poly<N,M> p3(v3);
#ifdef DEBUG
      vcl_cout << p3 << "\n      ";
#endif
      if (p1 + p2 != p3) {
        TEST("p1 + p2", false, true);
        vcl_cout << "i = "<<i<<", j = "<<j<<'\n';
      }
      if (p3 - p2 != p1) {
        TEST("p3 - p2", false, true);
        vcl_cout << "i = "<<i<<", j = "<<j<<'\n';
      }
    }
  }

  vcl_cout << "\nStarting multiplication tests:\n";

  vcl_vector<vnl_finite_int<N> > mod_p(M+1);
  for (int m=0; m<=M; ++m)
    mod_p[m] = vcl_atoi(s.c_str()+2*m);

  mod_p = vnl_finite_int_poly<N,M>::modulo_polynomial(mod_p);
  TEST("Setting modulo polynomial", 1, 1);

  mod_p.pop_back();
  vnl_finite_int_poly<N,M> irred = mod_p;
  vcl_cout << "X^" << M << " = " << irred << '\n';

  vcl_vector<vnl_finite_int<N> > v(M);
  for (int m = 0; m < M;  ++m) v[m] = m+1+m*m;
  vnl_finite_int_poly<N,M> x(v);
  vcl_cout << "x = " << x << '\n';
  for (int m = 0; m < M;  ++m) v[m] = m+1-m*m;
  vnl_finite_int_poly<N,M> y(v);
  vcl_cout << "y = " << y << '\n';
  y *= x;
  TEST("multiplying y with x", 1, 1);
  vcl_cout << "y * x = " << y << '\n';

  // Is irred indeed a polynomial of maximal multiplicative order?
  vnl_finite_int_poly<N,M> t = vnl_finite_int<N>(1);
  unsigned int order = 1;
  do t *= irred; while (t != vnl_finite_int<N>(1) && ++order < t.cardinality());
  TEST("multiplicative order", order != 0 && order < t.cardinality(), true);
  vcl_cout << "Multiplicative order of " << irred << " is " << order << '\n';
  TEST("multiplicative order", irred.multiplicative_order(), order);
  if (order+1 == t.cardinality())
    vcl_cout << "This is a Galois field of order " << order+1 << '\n';
  TEST("Field?", t.is_field(), order+1 == t.cardinality());

  vcl_cout << vcl_endl;
}

void test_finite()
{
  test_finite_int(vnl_finite_int<2>(0));
  test_finite_int(vnl_finite_int<3>(0));
  test_finite_int(vnl_finite_int<4>(0)); // not a field
  vnl_finite_int<4> b4 = 2; TEST("2*2=0 mod 4", b4*b4, 0); // zero divisor
  test_finite_int(vnl_finite_int<5>(0)); // Fermat prime
  test_finite_int(vnl_finite_int<6>(0));
  vnl_finite_int<6> b6 = 2; TEST("2*3=0 mod 6", b6*3, 0); // zero divisor
  test_finite_int(vnl_finite_int<7>(0)); // Mersenne prime
  test_finite_int(vnl_finite_int<8>(0));
  test_finite_int(vnl_finite_int<9>(0));
  test_finite_int(vnl_finite_int<10>(0));
  test_finite_int(vnl_finite_int<17>(0)); // Fermat prime
  test_finite_int(vnl_finite_int<31>(0)); // Mersenne prime
  test_finite_int(vnl_finite_int<100>(0)); // non-prime square
  vnl_finite_int<100> b100 = 20; TEST("25*20=0 mod 100", 25*b100, 0); // zero divisor
  test_finite_int(vnl_finite_int<243>(0)); // high prime power
  test_finite_int(vnl_finite_int<256>(0)); // high power of 2
  test_finite_int(vnl_finite_int<432>(0)); // high combined power of 2 and 3
  vnl_finite_int<432> b432 = 180; TEST("180*12=0 mod 432", b432*12, 0); // zero divisor
  test_finite_int(vnl_finite_int<257>(0)); // Fermat prime
  test_finite_int(vnl_finite_int<0x10001>(0)); // Fermat prime

  test_finite_poly(vnl_finite_int_poly<2,5>(0), "1 0 1 0 0 1"); // this is GF(32)
  test_finite_poly(vnl_finite_int_poly<3,3>(0), "1 2 0 1"); // this is GF(27)
  test_finite_poly(vnl_finite_int_poly<3,3>(0), "1 1 0 1"); // not a field
  test_finite_poly(vnl_finite_int_poly<6,2>(0), "1 0 1");
}

TESTMAIN(test_finite);
