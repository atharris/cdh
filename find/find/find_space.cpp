/*
* Author       : Christopher McKiernan
*
* Date         : 2/13/2014
*
* Title        : Findstars/Findstreaks Suite
*
* Findstars    : Find the centroid of stars from an image, and 
*                data relaying to those stars.
*		
* Findstreaks  : Output a Nx5 matrix where N is the number of streak,
*                containing midpoint X, midpoint Y, angle, length, brightness
*                of streaks.
*/

#include "find_space.h"

int main(int argc, char* argv[]) {
	string filepath;
	int thresh;

	// Check # arguments
	if(argc != 4 && argc != 7) {
		cout << "Error: Invalid # arguments, expected 4 or 7" <<
			" got " << argc << endl;
		exit(EXIT_FAILURE);
	}

	// Decide which should be getting ran findstars, or findstreaks
	int prog = (argc == 4) ? FSTARS : FSTREAKS;  

	// Check if first argument is valid param
	string param = argv[1];
	if(param != "-s" && param != "-t") {
		cout << "Error: Expected \"-s\" or \"-t\"" << endl;
		exit(EXIT_FAILURE);
	}

	Commandline command = Commandline();

	// Check if param passed in matches the type above
	if((prog == FSTARS && param != "-s") || 
		(prog == FSTREAKS && param != "-t"))
		command.displayError(param, prog);

	// The filepath (string) is the second arg
	if(!command.validFileCheck(argv[2])) 
		command.displayError(param, prog);
	filepath = argv[2];

	// The threshold (int) is the third arg
	thresh = command.convertPosInt(argv[3]);

	if(thresh == -1 || thresh > 255)
		command.displayError(param, prog);

	// Check the extra arguments findstreaks has
	if(prog == FSTREAKS) {
		int lineThresh, minLineLen, maxLineGap;
		// The line threshold (int) is the fourth arg
		lineThresh = command.convertPosInt(argv[4]);
		if(lineThresh == -1 || lineThresh > 255)
			command.displayError(param, prog);

		// The min length for a line is the fifth arg
		minLineLen = command.convertPosInt(argv[5]);
		if(minLineLen == -1)
			command.displayError(param, prog);

		// The max gap inbetween lines is the sixth arg
		maxLineGap = command.convertPosInt(argv[6]);
		if(maxLineGap == -1)
			command.displayError(param, prog);


		// Create findstreaks class
		Findstreaks fs = Findstreaks(filepath, thresh, lineThresh,
			minLineLen, maxLineGap);
		fs.outputStreakData();

		// FOR TESTING, PRINTS IMAGE
		//####################
		//fs.outputImage();

	}
	// Create findstars class 
	else {

		// FINDSTARS FROM IMAGE
		Findstars fs = Findstars(filepath, thresh);
		fs.outputStarLocations();
		Constellations cs = Constellations(fs.getStarVec());
		// FOR TESTING, PRINTS IMAGE
		//####################
		fs.outputImage();
	}
}


// COMMANDLINE
// Errors with parsing arguments
void Commandline::displayError(string param, int prog) {
	if(prog == FSTARS) {
		cout << "Usage is \"findspace " << param <<
			" path_image int_thresh\"\n";
		exit(EXIT_FAILURE);
	}
	else if(prog == FSTREAKS) {
		cout << "Usage is \"findspace "<< param <<
			" path_image int_thresh int_lineThresh "
			<< "int_minLineLen int_maxLineGap\"\n";
		exit(EXIT_FAILURE);
	}
}

// Check if valid file, return true if file exist, else false
bool Commandline::validFileCheck(char* filepath) {
	// Check to see if the file exist
	ifstream file(filepath);
	if(!file)
		return false;
	// Close file
	file.close();
	return true;
}

// Return integer value of argument else return -1
int Commandline::convertPosInt(char* str) {
	if(strlen(str) == 0)
		return -1;
	for(int i = 0; i < strlen(str); i++) {
		if(!isdigit(str[i]))
			return -1;
	}
	int ret = atoi(str);
	if(ret < 0)
		return -1;
	return ret;
}


// SPACEIMAGE
SpaceImage::SpaceImage(Mat img, int thresh) {
	this->img = img;
	this->thresh = thresh;
}

void SpaceImage::convertImageToBinary() {
	Mat img_g;
	cvtColor(img, img_g, CV_BGR2GRAY);
	GaussianBlur(img_g, img_g, Size(9, 9), 2, 2 );
	img_b = img_g > thresh;
}

Mat SpaceImage::getBinaryImage() {
	return img_b;
}


// FINDSTARS

// Findstars constructor for image
Findstars::Findstars(string filepath, int thresh) {
	img = imread(filepath);
	if(img.data) {
		SpaceImage si = SpaceImage(img, thresh);
		si.convertImageToBinary();
		img_b = si.getBinaryImage();
		gatherStarDataFromImage();
	} 
	else {
		cout << "Error: Image can't be read" << endl;
		exit(EXIT_FAILURE);
	}
}

bool Findstars::gatherStarDataFromImage() {
	if(img_b.data) {
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// Find contours
		findContours(img_b, contours, hierarchy,
			CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
			Point(0, 0));

		vector<Moments>mu(contours.size());
		for(int i = 0; i < contours.size(); i++) {
			mu[i] = moments( contours[i], false );
		}

		//  Get the mass centers:
		vector<Point2f>mc(contours.size());
		for(int i = 0; i < contours.size(); i++) {
			mc[i] = Point2f(mu[i].m10/mu[i].m00 ,
				mu[i].m01/mu[i].m00);
		}
		this->mc = mc;
		return true;
	}
	return false;

}
// Output star data to text file
bool Findstars::outputStarLocations() {
	if(!mc.empty()) {
		string out = "";
		// Draw plus signs on the stars
		for(int i = 0; i < mc.size(); i++) 
			out += numToStr(mc[i].x) +
			" " + numToStr(mc[i].y) + "\n";

		ofstream output("findstars.txt");
		output << out;
		output.close();
		return true;
	}
	return false;
}

// Convert float to string
string Findstars::numToStr(float number) {
	stringstream ss;
	ss << number;
	return ss.str();
}

// Return a complex<float> of the x,y locations of the star centroids
vector<complex <float> > Findstars::getStarVec() {
	vector<complex <float> > starVec;
	for(int i = 0; i < mc.size(); i++) {
		complex<float> loc(mc[i].x, mc[i].y);
		starVec.push_back(loc);
	}
	return starVec;
}

void Findstars::draw(Mat& out, Point start, Point end) {
	// Draw red line on img, from start to end
	int thickness = 1;
	int lineType = 8;
	line(out,
		start,
		end,
		Scalar(0, 0, 255),
		thickness,
		lineType);
}

// Draw plus on centroids, save image
void Findstars::outputImage() {
	Mat out;
	img.copyTo(out);
	for(int i = 0; i < mc.size(); i++) {
		Point start = Point(mc[i].x-1, mc[i].y);
		Point end = Point(mc[i].x+1, mc[i].y);
		draw(out, start, end);
		start = Point(mc[i].x, mc[i].y-1);
		end = Point(mc[i].x, mc[i].y+1);
		draw(out, start, end);
	}
	imwrite("star_test.png", out);
}



// FINDSTREAKS
Findstreaks::Findstreaks(string filepath, int thresh, 
						 int lineThresh, int minLineLength, int maxLineGap) {

	// Convert img to binary
	img = imread(filepath);
	if(img.data) {
		SpaceImage si = SpaceImage(img, thresh);
		si.convertImageToBinary();
		img_b = si.getBinaryImage();
		this->lineThresh = lineThresh;
		this->minLineLength = minLineLength;
		this->maxLineGap = maxLineGap;
		this->img = img;
		if(img_b.data) {
			setupImage();
			gatherStreakData();
		}
		else {
			cout <<"Error: Converting image to binary failed" << endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		cout << "Error: Image can't be read" << endl;
		exit(EXIT_FAILURE);
	}
}

// Get skeletal image of line
void Findstreaks::setupImage() {
	// Morphological Skeleton by Félix Abecassis
	Mat img_edge(img_b.size(), CV_8UC1, Scalar(0));
	Mat temp(img_b.size(), CV_8UC1);
	Mat element = getStructuringElement(MORPH_CROSS,
		Size(3, 3));

	bool done = false;
	while(!done) {
		morphologyEx(img_b, temp, MORPH_OPEN, element);
		bitwise_not(temp, temp);
		bitwise_and(img_b, temp, temp);
		bitwise_or(img_edge, temp, img_edge);
		erode(img_b, img_b, element);

		double max;
		cv::minMaxLoc(img_b, 0, &max);
		done = (max == 0);
	}
	// Image with edges
	img_edge.copyTo(this->img_edge);
}

// Hough Line Transform and data gathering
bool Findstreaks::gatherStreakData() {
	if(img_edge.data) {
		HoughLinesP(img_edge, lines, 1, CV_PI/180, lineThresh,
			minLineLength, maxLineGap);


		vector<float> data;
		for (size_t i = 0; i < lines.size(); i++ )
		{
			// Compute relevant data on streak
			Vec4i l = lines[i];

			Point start = Point(l[0], l[1]);
			Point end = Point(l[2], l[3]);
			float x1 = start.x, x2 = end.x, 
				y1 = start.y, y2 = end.y;

			// Midpoint calculation
			Point mid = Point((x1+x2)/2, (y1+y2)/2);

			// Angle calculation
			float angle = atan2(y2-y1,x2-x1)*180.0/CV_PI;
			if(angle < 0) angle+= 360.0;

			// Length calculation
			float length = sqrt((x2-x1)*(x2-x1) +
				(y2-y1)*(y2-y1));

			// Brightness needs more work, right now it
			// only involes looking at the center pixel
			float brightness = brightnessCalc(mid);

			// Store data on streak in vector
			streakVec.push_back(data);
			streakVec[i].push_back(mid.x);
			streakVec[i].push_back(mid.y);
			streakVec[i].push_back(length); 
			streakVec[i].push_back(angle);
			streakVec[i].push_back(brightness); 		
		}
		return true;
	}
	return false;
}

// Brightness calculation, needs improvement
float Findstreaks::brightnessCalc(Point mid) {
	Vec3b bgrPixel = img.at<Vec3b>(mid.y, mid.x);
	uchar blue = bgrPixel.val[0];
	uchar green = bgrPixel.val[1];
	uchar red = bgrPixel.val[2];
	return(0.2126 * red 
		+ 0.7152 * green 
		+ 0.0722 * blue);
}

// output streak data to textfile
bool Findstreaks::outputStreakData() {
	if(!streakVec.empty()) {
		string out = "";
		stringstream ss;
		// Display image        
		for(int i = 0; i < streakVec.size(); i++) {
			for(int j = 0; j < streakVec[i].size(); j++) {
				ss << streakVec[i][j];
				out += ss.str() + " ";
				ss.str("");
			}
			out += "\n";
		}
		ofstream output("findstreaks.txt");
		output << out;
		output.close();
		return true;
	}
	return false;
}

// Draw streak and mid/endpoints, and save image
void Findstreaks::outputImage() {
	Mat out;
	img.copyTo(out);
	for(int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		Point start = Point(l[0], l[1]);
		Point end = Point(l[2], l[3]);
		float x1 = start.x, x2 = end.x, 
			y1 = start.y, y2 = end.y;

		Point mid = Point((x1+x2)/2, (y1+y2)/2);
		draw(out, start, end, mid);
	}
	imwrite("streak_test.png", out);
}

// Draw on img the line, and midpoint and endpoints
void Findstreaks::draw(Mat& img, Point start, Point end, Point mid) {
	int thickness = 3;
	int lineType = 8;
	line(img, start, end, Scalar(0, 0, 255), thickness, lineType);
	circle(img, start, 2, Scalar(255,255,0), thickness,lineType ,0);
	circle(img, end, 2, Scalar(255,255,0), thickness,lineType ,0);
	circle(img, mid, 2, Scalar(255,0,0), thickness,lineType ,0);
}


// Constellation, check for matches
Constellations::Constellations(	vector<complex <float> > starVec) {
	this->starVec = starVec;
	if(initFileVec()) {
		for(int i = 0; i < fileVec.size();i++) {
			if(updateConstellationVec(fileVec[i])) {
				constellationCheck();
			}
		}
	}
}

// Update Constellation Vector to hold the complex values
// stored in the file at filepath
bool Constellations::updateConstellationVec(string filepath){
	constellationVec.clear();
	ifstream ifs;
	ifs.open(filepath.c_str());
	if(!ifs.good()) 
		return false;
	string token;
	while(ifs >> token) {
		float x = convertToken(token);
		ifs >> token;
		float y = convertToken(token);
		if(x == -1 || y == -1) return false;
		complex<float> loc(x,y);
		constellationVec.push_back(loc);
	}
	return true;
}

// Check to see if token is valid, if so return that float, else -1
float Constellations::convertToken(string token) {
	int decimalPt = 0;
	for(int i = 0; i < token.length(); i++) {
		if(token[i] == '.') {
			decimalPt++;
			if(decimalPt > 1) {
				return -1;
			}
		}
		else if(!isdigit(token[i]))
			return -1;
	}
	return atof(token.c_str());
}

// Initialize fileVec to hold the filepaths of the constellation text files
bool Constellations::initFileVec() {
	ifstream ifs;
	ifs.open("text/fl.txt");
	if(!ifs.good())
		return false;
	string token;
	while(ifs >> token) {
		fileVec.push_back(token);
	}
	return true;
}

// Check for match
void Constellations:: constellationCheck() {
	int mismatch=0;
	int match=0;

	//for performance reasons we check x^2 + y^2 < threshhold 
	// rather than sqrt(x^2+y^2) < max tolerable position error
	float threshold=0.00001; 
	//threshhold = (max tolerable position error)^2

	complex<float> constellation_divisor, constellation_normalized,
		field_divisor, field_normalized, constellation_position, scale_vector;
	// Arbitrarily pick two stars (c_s[0] and c_s[1]) to use as a reference. 
	// In the new constellation coordinate system, c_s[0] and c_s[1] will
	// correspond to (0,0) and (1,0) respectively
	constellation_divisor = constellationVec[0] - constellationVec[1]; 
	// scale the constellation. since we already know the scales c_s[0] 
	// and c_s[1] there is no need to waste time on them, so start at i=2
	for (int i=2;i<constellationVec.size();i++) { 
		constellationVec[i] = 
			(constellationVec[i]-constellationVec[0])/constellation_divisor;
	}
	for (int j=0;j<starVec.size();j++) {
		for (int k=0;k<starVec.size();k++) {
			if (j==k) continue; // if j==k, we will get divide by zero
			// use every possible pairing of stars in the field as a reference
			// (yes we have to do them backwards too)
			field_divisor = starVec[j] - starVec[k]; 
			for (int i=2;i<constellationVec.size();i++) {
				for (int l=0;l<starVec.size();l++) {
					// no need to check our reference stars
					if (l==j || l==k) continue; 
					// rotate, scale, and translate the remaining stars 
					// to the new coordinate system
					field_normalized = 
						(starVec[l]-starVec[j])/field_divisor; 
					match=0;
					// are they close?
					if ( norm(field_normalized - 
						constellationVec[i]) < threshold ) { 
							match=1;
							// this vector tells us both how much
							// the constellation has been scaled, and rotated
							scale_vector=constellation_divisor/field_divisor; 
							break;
					}
				}
				if (match == 0) break;
			}
			if (match == 1) break;
		}
		if (match == 1) {
			// this tells us the position of
			// the constellation in the photo we took
			constellation_position=starVec[j];
			break;
		}
	}
	if (match == 0) {
		scale_vector=(0,0);
		constellation_position=(0,0);
	}

	cout <<"match="<<match<<endl<<"scale_vector="<<scale_vector<<
		endl<<"constellation_position="<<constellation_position<<endl;
}


