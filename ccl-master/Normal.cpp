
#include "Normal.h"

/*float exponential ( float x ) {

  return (float)(exp ( (double)x ));

}*/

float cdf1(float x)
{

    float a1 =  0.254829592;
    float a2 = -0.284496736;
    float a3 =  1.421413741;
    float a4 = -1.453152027;
    float a5 =  1.061405429;
    float p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    float t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);

}

float cdf2(float x)
{
  if(x<-10.)return 0.;
  if(x>10.)return 1.;
  // number of steps
  int N=1000;
  // range of integration
  float a=0,b=x;
  // local variables
  float s,h,sum=0.;
  // inialise the variables
  h=(b-a)/N;
  // add in the first few terms
  sum = sum + exp(-a*a/2.) + 4.*exp(-(a+h)*(a+h)/2.);
  // and the last one
  sum = sum + exp(-b*b/2.);
  // loop over terms 2 up to N-1
  for(int i=1;i<N/2;i++)
  {
    s = a + 2*i*h;
    sum = sum + 2.*exp(-s*s/2.);
    s = s + h;
    sum = sum + 4.*exp(-s*s/2.);
  }
  // complete the integral
  sum = 0.5 + h*sum/3./sqrt(8.*atan(1.));
  // return result
  return sum;
}


float cdf3(float x, float mu, float sigma)
{

    //After 3 sigmas, off the charts
    if(x < (mu - 3*sigma))
    {
        return 0.;
    }
    if(x > (mu + 3*sigma))
    {
        return 1.;
    }
    //Number of steps
    int N = 1000;
    //Integration range
    float a=mu, b=x;
    //Local variables: s -> each of the terms from 2 to N-1, h -> integration range deltas, sum -> final value of the integral
    float s,h,sum=0.;
    //Initialization of h
    h=(b-a)/N;
    //Start by adding the simplest terms (0 and 1 (Remember, this is the 1st instance of the 4 term))
    sum = sum + exp(-(pow((a-mu),2.)/((pow(sigma,2.))*2.))) + 4.*exp(-(pow((a+h-mu),2.)/((pow(sigma,2.))*2.)));
    //Also add the last term
    sum = sum + exp(-(pow((b-mu),2.)/((pow(sigma,2.))*2.)));
    //Now loop over all intermediate terms
    for(int i=1;i<N/2;i++)
    {
        s = a + 2*i*h - mu;
        sum = sum + 2.*exp(-(pow(s,2.)/((pow(sigma,2.))*2.)));
        s = s + h;
        sum = sum + 4.*exp(-(pow(s,2.)/((pow(sigma,2.))*2.)));
    }
    //Complete with the missing pieces: 0.5 is from -infinity to mu (a) and we need to multiply by the distribution factor and by Simpson's factor (h/3)
    //sum = 0.5 + (h*sum/3.)*(1/(sigma*sqrt(2*PI)));
    sum = 0.5 + (h*sum/3.)*(1/(sigma*sqrt(8.*atan(1.))));
    return sum;

}

float pdf(float x, float mu, float sigma)
{

    //After 3 sigmas, off the charts
    if(x < (mu - 3*sigma) || x > (mu + 3*sigma))
    {
        return 0.;
    }
    return exp(-(pow((x-mu),2.)/((pow(sigma,2.))*2.)))*(1/(sigma*sqrt(8.*atan(1.))));
}

float gaussian_random ( float mu, float sigma )
{

    static float V1, V2, S;
    static int phase = 0;
    float X;

    if(phase == 0) {
        do {
            float U1 = (float)rand() / RAND_MAX;
            float U2 = (float)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
            } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return ((X*sigma)+mu);

}

float uneven_gaussian ( float mu, float sigma_left, float sigma_right )
{
    float where = (float)rand() / RAND_MAX;
    float value = mu;
    // Let's say this is left
    if(where <= 0.5)
    {
        value = gaussian_random(mu, sigma_left);
        if(value > mu)
        {
            value = mu - (value - mu);
        }
    }
    //Then, this is right
    else
    {
        value = gaussian_random(mu, sigma_right);
        if(value < mu)
        {
            value = mu + (mu - value);
        }
    }
    return(value);
}

/*int main()
{

    float x[] =
    {
        -3,
        -1,
        0.0,
        0.5,
        2.1
    };

    // Output computed by Mathematica
    // y = Phi[x]
    float y[] =
    {
        0.00134989803163,
        0.158655253931,
        0.5,
        0.691462461274,
        0.982135579437
    };


    float z[] =
    {
        17,
        19,
        20.0,
        20.5,
        22.1
    };

    int numTests = sizeof(x)/sizeof(float);

    float maxError = 0.0;
    for (int i = 0; i < numTests; ++i)
    {
        float error = fabs(y[i] - cdf1(x[i]));
        if (error > maxError)
            maxError = error;
    }

    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%% CDF %%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n\n";

    std::cout << "Maximum error 1: " << maxError << "\n";

    maxError = 0.0;
    for (int i = 0; i < numTests; ++i)
    {
        float error = fabs(y[i] - cdf2(x[i]));
        if (error > maxError)
            maxError = error;
    }

    std::cout << "Maximum error 2: " << maxError << "\n";

    maxError = 0.0;
    for (int i = 0; i < numTests; ++i)
    {
        float error = fabs(y[i] - cdf3(z[i],20,1));
        if (error > maxError)
            maxError = error;
    }

    std::cout << "Maximum error 3: " << maxError << "\n";
    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%% PDF %%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n\n";


    float prob = 0.0, t = 0.0, mu = 20.0, sigma = 3.0;
    do
    {
        std::cout << "Give an x: ";
        std::cin >> t;
        std::cout << "The probability is: " << pdf(t,mu,sigma) << "\n";
    }
    while(t > -50);

    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%% Random numbers %%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n\n";
    for(int i=1; i<= 100000; i++)
    {
        std::cout << gaussian_random(mu, sigma) << "\t";
        if(i%6 == 0)
        {
            std::cout << "\n";
        }
    }
    std::cout << "\n";

    return 0;
}*/
