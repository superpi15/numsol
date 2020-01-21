#include <bits/stdc++.h> 
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
typedef mp::number<mp::backends::cpp_int_backend<>, mp::et_off>       int_mp;
typedef mp::number<mp::cpp_rational_backend, mp::et_off>    rational_mp;
typedef mp::number<mp::cpp_dec_float<0>, mp::et_off>        float_mp;  // 0 means 'unlimited'

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

int main(){
	int K = 128; // num bucket
	int Th= 60;  // threshold
	

	for(int i=2; i<10; i++){
		int Ni= K * Th * i * 0.1 ;
		//if( 472 != Ni )\
			continue;
		double prob = probability(K, Th, Ni);\
		std::cout << "Given "<< std::setw(3) << Ni <<" candys "<< std::setw(3) << K << " buckets, the probability of all legal is ";\
		std::cout << std::setw(17) << (prob<0.01? 0: prob) << " \%" << ". Avg Prefill = "<< (double)(Ni/K)/Th <<std::endl;\
	}
}
