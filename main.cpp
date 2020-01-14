#include <iostream>
#include <cmath>
#include <iomanip>
double stirling( double N ){
	return N>1? N * log(N) - N + 0.5 * log(2*3.14159265359*N): 0;
}

double binomial_Coeff( double N, double K ){
	double nominator_ln = stirling(N);
	double denominator_ln1 = stirling(K);
	double denominator_ln2 = N>K? stirling(N-K): 0;
	return pow( exp(1), nominator_ln - denominator_ln1 - denominator_ln2);
}

double num_linear_sol( double N, double K, double Th ){
	double sum = 0;
	double leading = binomial_Coeff(N+K-1,K-1);
	for(int i=1; i<=K; i++){
		if( i*Th >= N+K-1 )
			break;
		double term = binomial_Coeff(K,i) * binomial_Coeff(N+K-1-i*Th,K-1);
		sum += (i%2)? -term: term;
	}
	return leading + sum;
}

int main(){
	int K = 16; // num bucket
	int N = 4*7;  // num candy
	int Th= 7;  // threshold
	double nLegal = num_linear_sol(N,K,Th);
	double nAll   = binomial_Coeff(N+K-1,K-1);
	std::cout << "legal = "<< nLegal <<std::endl;
	std::cout << "all = "<< nAll <<std::endl;
	std::cout << "prob= " <<nLegal/nAll <<std::endl;
}
