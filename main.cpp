#include <bits/stdc++.h> 
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
typedef mp::number<mp::backends::cpp_int_backend<>, mp::et_off>       int_mp;
typedef mp::number<mp::cpp_rational_backend, mp::et_off>    rational_mp;
typedef mp::number<mp::cpp_dec_float<0>, mp::et_off>        float_mp;  // 0 means 'unlimited'

/*--------------------------------------------------------------------------------------

In-Tile load 

--------------------------------------------------------------------------------------*/

static double _sum_pow( int val, int nTerm, int coeff[], double denominator ){
	double ret = 0;
	for(int i=0; i<nTerm; i++)
		ret += coeff[i] * pow( val, i+1 );
	return ret/denominator;
}


double sum_pow1( int val ){
	int nTerm = 2;
	double denominator = 2;
	int coeff[nTerm] = { 1, 1 };
	return _sum_pow( val, nTerm, coeff, denominator );
}

double sum_pow2( int val ){
	int nTerm = 3;
	double denominator = 6;
	int coeff[nTerm] = { 1, 3, 2 };
	return _sum_pow( val, nTerm, coeff, denominator );
}

double sum_pow3( int val ){
	int nTerm = 4;
	double denominator = 4;
	int coeff[nTerm] = { 0, 1, 2, 1 };
	return _sum_pow( val, nTerm, coeff, denominator );
}

double sum_pow4( int val ){
	int nTerm = 5;
	double denominator = 30;
	int coeff[nTerm] = { 1, 0, 10, 15, 6 };
	return _sum_pow( val, nTerm, coeff, denominator );
}

double sum_pow5( int val ){
	int nTerm = 6;
	double denominator = 12;
	int coeff[nTerm] = { 0, -1, 0, 5, 6, 2 };
	return _sum_pow( val, nTerm, coeff, denominator );
}

double sum_pow( int val, int order ){
	#define CND_RET(val,ORD) if( ORD==order ) return sum_pow ## ORD (val);
	CND_RET(val,1);
	CND_RET(val,2);
	CND_RET(val,3);
	CND_RET(val,4);
	CND_RET(val,5);
	assert(false);
	return 0;
}


static double Gamma( int n ){
	return (double) (pow(n,3) - n)/6.0f;
}

static double Lambda( int n, int k ){
	return (double) Gamma(n-1) + (n-k) * sum_pow1(k);
}

static double Delta( int n, int k ){
	return (double) Lambda(n,n-k) - Lambda(n,k-1);
}


static double Dpart( int n, bool isLoad ){
	return 1/6.0f * sum_pow(n, 4+ isLoad ) 
		- (n+1) * sum_pow(n, 3+ isLoad) 
		+ (pow(n,2) +n   -1/6.0f) * sum_pow(n, 2+ isLoad ) 
		+ (pow(n,2) +2*n +1) * sum_pow(n, 1+ isLoad );
}

static double F_Upart( int n, int m, bool isLoad ){
	return 1/6.0f * 
		( -sum_pow(m, 4+ isLoad)
		+ 6*(n +1)* sum_pow(m, 3+ isLoad)
		- (12*pow(n,2) + 24*n +11) * sum_pow(m, 2+ isLoad)
		+ ( 8*pow(n,3) + 24 * pow(n,2) + 22 * n + 6) * sum_pow(m, 1+isLoad));
}

static double Upart( int n, bool isLoad ){
	return F_Upart(n, 2*n, isLoad) - F_Upart(n, n, isLoad);
}

static double nInstance(int n){
	return Dpart(n, false) + Upart(n, false);
}

static double nLoad(int n){
	return Dpart(n, true ) + Upart(n, true );
}

double intile_expval(int n){
	return nLoad(n)/nInstance(n);
}
double intile_ratio_expval2scale(int n){
	return (nLoad(n)/nInstance(n))/n;
}

/*--------------------------------------------------------------------------------------

END In-Tile load 

--------------------------------------------------------------------------------------*/



int_mp Factorial(int number) 
{ 
	int_mp num = 1; 
	for (int i = 1; i <= number; i++) 
		num = num * i; 
	return num;
}

int_mp Factorial(int number, int dst) 
{ 
	int_mp num = 1; 
	for (int i = dst; i <= number; i++) 
		num = num * i; 
	return num;
}

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cassert>


int_mp binomial_Coeff( int N, int K ){
	if( N==K )
		return 1;
	return Factorial( N , std::max(N-K+1,K+1) ) / Factorial( std::min(N-K,K) );
}

int_mp num_linear_sol( int N, int K, int Th ){
	int_mp sum = 0;
	int_mp leading = binomial_Coeff(N+K-1,K-1);
	int fill = Th;
	int maxOvflBucket = N/fill;
	int remaindar = N-maxOvflBucket * fill;
	if( K * Th < N )
		return 0;
	//std::cout << " leading = " << leading <<" N= "<< N <<" K= "<< K<< " Th = "<< Th <<" ovflN = "<< Th+1 \
		<<" maxOvflBucket= "<< maxOvflBucket<<" , remaindar = "<< remaindar <<std::endl;
	for(int i=1; i<=K; i++){
		//std::cout << i*Th <<" , (N+K-1)=" << N+K-1 << " , N = " << N << " , fill = "<< fill <<std::endl;
		//std::cout << i*fill << " , "<< N+K-1 <<std::endl;
		if( i*fill > N )\
			break;
		if( i*fill > N+K-1 )\
			break;
		int_mp term = binomial_Coeff(K, i) * binomial_Coeff(N+K-1-i*fill, K-1);
		term = (i%2)? -term: term;
		sum += term;
		//std::cout << std::setw(3) << i << " : i*fill " \
			<< std::setw(12) << i*fill << " Coeff( "<< K <<" , "<< std::setw(2)<< i <<" ) = "\
			<< std::setw(12) << binomial_Coeff(K, i) << " Coeff( "<< std::setw(3)<< (N+K-1-i*fill) <<" , "<< std::setw(2)<< (K-1) <<" ) = "\
			<< std::setw(12) << binomial_Coeff(N+K-1-i*fill, K-1) <<" , term = "\
			<< std::setw(12) << term << " , sum = " << std::setw(12) << sum << std::endl;
	}

	//std::cout << "illegal = "<< - sum <<std::endl;
	return leading + sum;
}

double probability( int K, int Th, int N ){
	int_mp nLegal = num_linear_sol( N, K, Th );
	int_mp nAll   = binomial_Coeff(N+K-1,K-1);

	float_mp prob = nLegal.convert_to<float_mp>() / nAll.convert_to<float_mp>();
	double ret = prob.convert_to<double>();
	return 100 * ret;
}


using namespace std;
void plot_curve( int ScaleFactor, int Th ){
	double inTile = intile_ratio_expval2scale(ScaleFactor);
	cout << "In-Tile Factor = "<< inTile <<endl;
	char buff[1024];
	int NumTile = pow( ScaleFactor, 2 );
	sprintf( buff, "%5s %12s %12s %15s %15s %15s", "/", "NumTile", "Step", "Safe-Prob.", "AvgPrefill", "+In-Tile" );
	cout << buff << endl;
	for(int i=0; i<100; i++){
		int Load = NumTile * Th * 0.01 * i;
		double AvgPrefill = (double) Load / NumTile / Th;
		double InTileAdj  = AvgPrefill / inTile;
		double prob = probability( NumTile, Th, Load );
		sprintf( buff, "%5s %12d %12d %15.7f %15.7f %15.7f", "val", NumTile, i, prob, AvgPrefill, InTileAdj );
		cout << buff <<endl;
		if( prob < 50 )
			break;
	}
}

int main( int argc, char * argv[] ){
	if( argc < 3 ){
		cout<<"./prog [ScaleFactor] [Threshold]"<< endl;
		return 0;
	}
	plot_curve( atoi(argv[1]), atoi(argv[2]) );
	return 0;


	int K = 3; // num bucket
	int Th= 60;  // threshold
	
	double SafePrefill = 0;
//	for(int i=1; i<100; i++){
//		int Ni= K * Th * i * 0.01 ;
//		//if( 472 != Ni )\
//			continue;
//		double prob = probability(K, Th, Ni);\
//		if( prob < 0.01 )
//			break;
//		std::cout << "Given "<< std::setw(3) << Ni <<" candys "<< std::setw(3) << K << " buckets, the probability of all legal is ";\
//		std::cout << std::setw(17) << (prob<0.01? 0: prob) << " \%" << ". Avg Prefill = "<< (double)(Ni/K)/Th <<std::endl;\
//	}
//	for(int i=1; i<100; i++){
//		int Ni= K * Th * i * 0.01 ;
//		//if( 472 != Ni )\
//			continue;
//		double prob = probability(K, Th, Ni);\
//		if( prob < 0.01 )
//			break;
//		;
//		//std::cout << "Given "<< std::setw(3) << Ni <<" candys "<< std::setw(3) << K << " buckets, the probability of all legal is ";\
//		std::cout << std::setw(17) << (prob<0.01? 0: prob) << " \%" << ". Avg Prefill = "<< (double)(Ni/K)/Th <<std::endl;\
//	}
}
