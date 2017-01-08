#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void direct_sampling(int samples) {
	srand(time(0));
	int cnt = samples;
	int traffic = 0;
	for (int x = 0; x < cnt; x++) {
		bool snow = false;
		if ((rand() % 10) <= 1) {
			snow = true;
		}
		if (snow && (rand() % 10 != 3)) {
			traffic++;
		}
		if (!snow && (rand() % 2 != 0)) {
			traffic++;
		}
	}
	printf("Direct Sampling: %d of %d trials have bad traffic (%1.3f)\n", traffic, cnt, (float)traffic / cnt);
}

void likelihood_weighting_sampling(int samples) {
	srand(time(0));
	int cnt = samples;
	double rainCnt = 0;
	double rainNotCnt = 0;
	double weight = 1;
	for (int x = 0; x < cnt; x++) {
		bool cloudy = false;
		if (rand() % 2 == 1) {
			cloudy = true;
		}
		bool sprinkler = true;
		if (cloudy) weight = 0.1;
		if (!cloudy) weight = 0.5;
		bool rain = false;
		if (cloudy && (rand() % 10 >= 2)) {
			rain = true;
		}
		if (cloudy && (rand() % 10 < 2)) {
			rain = true;
		}
		bool wet = false;
		if (cloudy && rain && (rand() % 100 < 99)) {
			wet = true;
		}
		if (cloudy && !rain && (rand() % 100 < 90)) {
			wet = true;
		}
		if (!cloudy && rain && (rand() % 100 < 90)) {
			wet = true;
		}
		if (rain) rainCnt += weight;
		if (!rain) rainNotCnt += weight;
	}

	printf("Likelihood Weighting: %1.3f weight on rain; %1.3f on no rain. (%1.3f normalized chance of rain |sprinkler = true)\n", 
		rainCnt, rainNotCnt, rainCnt / (rainCnt+ rainNotCnt)); 
}

int gibbs_sampling(int samples) {

	// calculated probabilities
	
	// cloudy
	// P(c = t | r = t, s = t) = .04 / (.04 + .05) = .444444
	// P(c = t | r = f, s = t) = .01 / (.01 + .2 ) = .047619
	// P(c = f | r = t, s = t) = .05 / (.05 + .04) = .555555
	// P(c = f | r = f, s = t) = .2  / (.2  + .01) = .952380
	
	// wet grass (from table)
	// P(w = t | r = t, s = t) = .99
	// P(w = t | r = f, s = t) = .9
	// P(w = f | r = t, s = t) = .01
	// P(w = f | r = f, s = t) = .1

	// rain
	// P(r = t | c = t, w = t) = .814
	// P(r = t | c = f, w = t) = .215
	// P(r = t | c = t, w = f) = .285
	// P(r = t | c = f, w = f) = .024

	// N, a vector of counts for each value of X, initially 0
	int N = samples;

	// seed random number generator
	// srand and rand are not best, but they will do for this purpose
	srand(time(0));

	// sprinkler = evidence variable = true
	// cloudy, rain, wet are non evidence variables
	// sprinkler is alway true, so set to .1
	// cloudy is 50/50 so its .5 no matter what
	bool cloudy, sprinkler = true, rain, wet;
	int rainCnt = 0, rainNotCnt = 0;
	
	// initialize random values for the variables in the set of non evidence variables
	cloudy = false;
	if (rand() % 2 == 1) {
		cloudy = true;
	}
	rain = false;
	if (rand() % 2 == 1) {
		rain = true;
	}
	wet = false;
	if (rand() % 2 == 1) {
		wet = true;
	}

	// loop through count of samples
	for (int x = 0; x < N; x++) {
		if (x % 3 == 0) {
			//change cloudy
			if (rain && (rand() % 1000) < 444) {
				cloudy = true;
			}else if(!rain && (rand() % 1000) < 47){
				cloudy = true;
			}else {
				cloudy = false;
			}
		}else if (x % 3 == 1) {
			//change rain
			if (cloudy && wet && (rand() % 1000) < 814) {
				rain = true;
			}else if(!cloudy && wet && (rand() % 1000) < 215){
				rain = true;
			}else if (cloudy && !wet && (rand() % 1000) < 285) {
				rain = true;
			}else if (!cloudy && !wet && (rand() % 1000) < 24) {
				rain = true;
			}else {
				rain = false;
			}
		}else if (x % 3 == 2) {
			//change wet grass
			if (rain && (rand() % 1000) < 990) {
				wet = true;
			}else if(!rain && (rand() % 1000) < 900){
				wet = true;
			}else {
				wet = false;
			}
		}

		// increment counts
		if (rain) {
			rainCnt++;
		}else {
			rainNotCnt++;
		}
	}
	
	std::cout << "Rain count: " << rainCnt << std::endl;
	std::cout << "Rain not count: " << rainNotCnt << std::endl;
	std::cout << ((float) rainCnt / N) << std::endl;

	//return normalized n
	return -1;
}

int main(int argc, char* argv[]) {
	std::cout << "Starting..." << std::endl;
	int samples = 10000000;
	//direct_sampling(samples);
	//likelihood_weighting_sampling(samples);
	gibbs_sampling(samples);
	
	//hold output open
	getchar();
}