/**
 * Calculating the probability of a sum of two uniform random number occuring.
 *
 * Matthew J. Urffer, NE 582, Problem 2-6.  Febuary 10, 2012
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;
#define NSAMPLES 1000000								// Number of samples


int main(){

	// Decarling Variables
	double sum[NSAMPLES];

	// Only Need the Histograms, but keep the NTuple for Kicks
    int nBins = 100;
	TH1D *h = new TH1D("sum","Sum of Two Uniform Distributions",nBins,-0.1,2.1);
	TCanvas *c = new TCanvas("c","Canvas",400,400);
	assert(h != NULL && c!= NULL);

	double val = 1.4;		// Value that sum should be greater than
	int score = 0;			// Number of sums greater than val
	
	// Filling Values
	int i;
	double x;
	double y;
	for (i=0; i<NSAMPLES; i++){
	   y = (double) rand()/(double) RAND_MAX;
	   x = (double) rand()/(double) RAND_MAX;
	
		// Computing the Sum, Binning it
		sum[i] = x + y;
		h->Fill(sum[i]);

		// Counting a score if sum is greater than val
		if (sum[i] > val)
			score++;
	}
	// Plotting with ROOT
	h->Draw();
	c->SaveAs("Problem2-6.gif");

    // Creating A CDF
    TH1D *hC = new TH1D("CDF Sum","CDF of Sum of Two Unifrom Distrubtions",nBins,-0.1,2.1);
    int total =0;
    for(int i=0; i < nBins; i++){
        total += h->GetBinContent(i);
        for(int k=0; k < total; k++)
            hC->AddBinContent(i);
    }
    hC->Scale(1/h->Integral());
    hC->Draw();
    c->SaveAs("Problem2-6CDF.gif");
	fprintf(stdout,"Out of %d trials, %d where greater than %f  (%f) %%\n",NSAMPLES,score,val,(double) score / (double) NSAMPLES);

	return EXIT_SUCCESS;
}
