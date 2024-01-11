#include <stdio.h>
#include <time.h>

float trapezoidal_rule(float x)
{
      return x*x;
}
 
int main()
{
      int count, interval;
      float lower_limit, upper_limit, result = 0, sum = 0, length;
      clock_t start,end;

      printf("\nEnter the Interval:\t");
      scanf("%d", &interval);
      printf("\nEnter Lower Limit:\t");
      scanf("%f", &lower_limit);
      printf("\nEnter Upper Limit:\t");
      scanf("%f", &upper_limit);

      start=clock();
      length = (upper_limit - lower_limit) / interval;

      for(count = 1; count <= interval - 1; count++)
      {
            sum = sum + trapezoidal_rule(lower_limit + count*length);
      }

      result = (trapezoidal_rule(lower_limit) + trapezoidal_rule(upper_limit) + 2*sum) * (length/2);

      end=clock();

      printf("\nResult:\t%f\n", result);

      
      printf("\nElapsed time is:\t%lf\n",(double)(end-start)/ CLOCKS_PER_SEC);

      return 0;
}
