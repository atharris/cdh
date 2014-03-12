#include <iostream>
#include <complex>
using namespace std;

int main() {

int mismatch=0;
int match=0;

float threshold=0.0001; //for performance reasons we check x^2 + y^2 < threshhold rather than sqrt(x^2+y^2) < max tolerable position error
//threshhold = (max tolerable position error)^2


complex<float>constelation_stars[4];
 constelation_stars[0] = complex<float> (281.5, 199.5);
 constelation_stars[1] = complex<float> (174.4, 145.23);
 constelation_stars[2] = complex<float> (221.708, 52.625);
 constelation_stars[3] = complex<float> (107.867, 44.5);

 complex<float>field_stars[18];
 field_stars[0] = complex<float>(209.5, 154.5);
 field_stars[1] = complex<float>(220.578, 122.578);
 field_stars[2] = complex<float>(114, 100);
 field_stars[3] = complex<float>(31.7778, 27);
 field_stars[4] = complex<float>(5, 18.5);
 field_stars[5] = complex<float>(133.5, 7.5);
 field_stars[6] = complex<float>(209.5, 154.5);
 field_stars[7] = complex<float>(120.578, 122.578);
 field_stars[8] = complex<float>(214, 100);
 field_stars[9] = complex<float>(131.7778, 27);
 field_stars[10] = complex<float>(15, 18.5);
 field_stars[11] = complex<float>(223.5, 7.5);
 field_stars[12] = complex<float>(109.5, 154.5);
 field_stars[13] = complex<float>(210.578, 122.578);
 field_stars[14] = complex<float>(124, 100);
 field_stars[15] = complex<float>(21.7778, 27);
 field_stars[16] = complex<float>(15, 18.5);
 field_stars[17] = complex<float>(113.5, 7.5);

 complex<float> constelation_divisor, constelation_normalized, field_divisor, field_normalized, constelation_position, scale_vector;
 constelation_divisor = constelation_stars[0] - constelation_stars[1]; //arbitrarily pick two stars (c_s[0] and c_s[1]) to use as a reference. In the new constelation coordinate system, c_s[0] and c_s[1] will correspond to (0,0) and (1,0) respectively
	for (int i=2;i<4;i++) { //scale the constelation. since we already know the scales c_s[0] and c_s[1] there is no need to waste time on them, so start at i=2
		constelation_stars[i] = (constelation_stars[i]-constelation_stars[0])/constelation_divisor;
	}
   for (int m=0;m<5000;m++){
	for (int j=0;j<18;j++) {
		for (int k=0;k<18;k++) {
			if (j==k) continue; // if j==k, we will get divide by zero
			field_divisor = field_stars[j] - field_stars[k]; //use every possible pairing of stars in the field as a reference (yes we have to do them backwards too)
     		for (int i=2;i<4;i++) {
				for (int l=0;l<18;l++) {
					if (l==j || l==k) continue; // no need to check our reference stars
					field_normalized = (field_stars[l]-field_stars[j])/field_divisor; //rotate, scale, and translate the remaining stars to the new coordinate system
					match=0;
					if ( norm(field_normalized - constelation_stars[i]) < threshold ) { //are they close?
						match=1;
						scale_vector=constelation_divisor/field_divisor; //this vector tells us both how much the constelation has been scaled, and rotated
						break;
					}
				}
				if (match == 0) break;
			}
			if (match == 1) break;
		}
		if (match == 1) {
			constelation_position=field_stars[j];//this tells us the position of the constelation in the photo we took
			break;
		}
	}
   }
	if (match == 0) {
		scale_vector=(0,0);
		constelation_position=(0,0);
	}
 
 cout <<"match="<<match<<endl<<"scale_vector="<<scale_vector<<endl<<"constelation_position="<<constelation_position<<endl;
 
 return 0;
}



