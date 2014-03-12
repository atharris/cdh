#include <kalman/ekfilter.hpp>

class satelliteModel : public Kalman::EKFilter<double, 0> {
public:
   satelliteModel();
protected:
   
   void makeA();
   void makeH();
   void makeV();
   void makeR();
   void makeW();
   void makeQ();
   void makeProcess();
   void makeMeasure();

}
