/*
Benedict Hofmockel
4/20/2022

If I had to operate blindly without Monty Hall (like this code): I picked the strateg of switching, because the statisical chance that you will pick a goat is 2/3, and you won't know what you picked.

If I was on "Let's make a deal": Considering my first choice doesn't really matter as I will be shown a goat by Monty anyway. So the second choice matters, and then it's a 50/50 (and how good you are at behavoral analysis of Monty Hall)... but since I can tolerate my mistakes, I would stay. 

*/


#include <iostream>//For I/O (mostly during testing)
#include <fstream>//For writting the binary image
#include <string>//For easy file names
#include <thread>//Add Threads
#include <complex>

//Write Header of a Bitmap Image
void writeHeader(std::ofstream &image, int height, int width);
//Write DIB header of a Bitmap Image
void writeDIB(std::ofstream &image, int height, int width, int dpi);
//Write Pixels from a collection of arrays
void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels);

//Break an integer into individual bytes
char getByte(int value, int byte);
//Dots per inch to pixels per meter
int dpiToPPM(int dpi);

//Color Thread //TODO: delete this if not used in final submission
void colorThread(
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int startHeight,
	int endHeight,
	int startWidth,
	int endWidth,
	long double xStep,
	long double yStep
	);

//mandelbot z function
int z(long double x, long double y, int threshold);

using namespace std;

int main(int argc, char** argv){

    if (argc != 6){ 
        cout << "Usage: mandelbrot x1 x2 y1 y2 file_name.bmp" << endl;
        return(0);
    }

    long double x1 = strtold(argv[1],NULL);
    long double x2 = strtold(argv[2],NULL);
    long double y1 = strtold(argv[3],NULL);
    long double y2 = strtold(argv[4],NULL);
    
    //TODO: generate the height scaling based off of the width
    /*The width of all images should be 1500 pixels. The height should be determined by the number of
points on the y axis. The image needs to scale if the x and y are not of equal length. Base the scaling on
the fact that the width is always 1500 pixels.*/
    int width = 1500; 
    int height = (abs(y2 - y1) / abs(x2 - x1)) * width; //TODO: change this to above mentioned generation
	cout << "Height: " << height <<endl;
	
	int dpi = 150;//pixel per inch
    std::string filename=argv[5];
    
	//Open File
	std::ofstream image(filename,std::ios::binary);
    //Make the Bitmap Header
	writeHeader(image, height, width);
	writeDIB(image,height,width,dpi);

    //Arrays to Store the Three Colors of a Pixel
	int pixels = height*width;
	unsigned char* red = new unsigned char[pixels];
	unsigned char* green = new unsigned char[pixels];
	unsigned char* blue = new unsigned char[pixels];

	//TODO: get rid of this code block 

	//Color the Image with 4 threads
	int stepSize = pixels/4;
	cout << "stepSize: " << stepSize <<endl;
	std::thread* myThreads = new std::thread[4];

	/*
	void colorThread(
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int startHeight,
	int endHeight,
	int startWidth,
	int endWidth,
	long double xStep,
	long double yStep
	)
	*/

	long double xStep = abs(x2 - x1) / width;
	long double yStep = abs(y2 - y1) / height;

	myThreads[0] = std::thread(colorThread, red, green, blue, 0, (height/4)*1, 0, width, xStep, yStep);
	//Light Grey
	myThreads[1] = std::thread(colorThread, red, green, blue, (height/4)*1+1, (height/4)*2, 0, width, xStep, yStep);
	//Dark Grey
	myThreads[2] = std::thread(colorThread, red, green, blue, (height/4)*2+1, (height/4)*3, 0, width, xStep, yStep);
	//White
	myThreads[3] = std::thread(colorThread, red, green, blue, (height/4)*3+1, height, 0, width, xStep, yStep);

	for(int i=0; i < 4; i++){
		myThreads[i].join();
	}
	
	//Write the pixels to file
	writePixels(image, red, green, blue, pixels);
	//Close the File
	image.close();

	//Exit the Program
	std::cout <<"Made Image " << filename << std::endl;

    return(0);
}

//TODO: delete this function is not used oin the fnal submission
void colorThread(
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int startHeight,
	int endHeight,
	int startWidth,
	int endWidth,
	long double xStep,
	long double yStep
	)
{
	long double xVal = xStep;
	long double yVal = yStep;
	int value = 0;

	for (int i=startHeight; i < endHeight; i++){
		for (int j=startWidth; j < endWidth; j++){
			value = z(xVal, yVal, 100);
			// cout << "xVal:" << xVal << " yVal:" << yVal << " value: " << value <<  endl;
			xVal = xVal + xStep;			
			if (value < 100){
				red[i*j] = 0;
				green[i*j] = 0;
				blue[i*j] = 0;
			} else {
				red[i*j] = 255;
				green[i*j] = 255;
				blue[i*j] = 255;
			}
		}
		cout << "xVal:" << xVal << " yVal:" << yVal << " value: " << value <<  endl;
		xVal = xStep;
		yVal = yVal + yStep;
	}
}

//Mandelbrot z function
//Returns the number of iterations needed
//to reach 4 or threshold if bounded
//We use long double for the maximum accuracy that is easily accesible
int z(long double x, long double y, int threshold){ //this function was adopted from mandelbrot lecture of CS361 - Mark Boady - 202135
	std::complex<long double> c(x,y);
	std::complex<long double> current(0,0);
	
	for(int i=1; i < threshold; i++){
		current = current * current + c;
		if(std::abs(current) > 4){ //Rule of thumb: if |Z_n| > 4 we can call a sequence divergent
			return i;
		}
	}
	//x,y pair never converged; the pair is divergent
	return threshold;
}

char getByte(int value, int byte)
{
	int newValue = value;
	
	unsigned char rem;
	for(int i=0; i <= byte; i++)
	{
		rem = static_cast<unsigned char>( newValue%256 );
		newValue = newValue/256;
	}
	return rem;
}

//Convesion
//x pixels/inches * C inches/meter = y pixels/meter
int dpiToPPM(int dpi)
{
	float inchesPerMeter = 39.3701/1;
	float convert = dpi*inchesPerMeter;
	return static_cast<int>(convert);
}

void writeHeader(std::ofstream &image, int height, int width)
{
	//How many pixel does the image have
	int pixels = height*width;
	//Make the header. It is always 14 bytes
	int headerSize = 14;
	//Array to store the header
	char* header = new char[headerSize];
	//The header is 14 Bytes
	//The DIB is 40 bytes
	int offset = headerSize + 40;
	//Each Pixel is another 3 bytes
	int totalBits = pixels*3+offset;
	//Make the Header
	//First 2 Bytes are BM for bitmap
	header[0] = 'B';
	header[1] = 'M';
	//Next 4 bytes are the total size of the file
	header[2] = getByte(totalBits,0);
	header[3] = getByte(totalBits,1);
	header[4] = getByte(totalBits,2);
	header[5] = getByte(totalBits,3);
	//Next for bits are 0 (reserved for other uses)
	header[6] = 0;
	header[7] = 0;
	header[8] = 0;
	header[9] = 0;
	//Last 4 bytes are offset
	//Where do the pixels start
	header[10] = getByte(offset,0);
	header[11] = getByte(offset,2);
	header[12] = getByte(offset,2);
	header[13] = getByte(offset,3);
	//Write the Header to the file in binary
	image.write(header, headerSize);
	//Exit the Function
	return;
}

//Using the BITMAPINFOHEADER standard
void writeDIB(std::ofstream &image, int height, int width, int dpi)
{
	//Convert DPI to Pixels Per Meter
	int resolution = dpiToPPM(dpi);
	//Fixed Size of 40 Bytes
	int sizeDIB = 40;
	//Make array of bytes
	char* DIB = new char[sizeDIB];
	//Set Values
	//First 4 bytes are header size of this header (40)
	DIB[0] = getByte(40,0);
	DIB[1] = getByte(40,1);
	DIB[2] = getByte(40,2);
	DIB[3] = getByte(40,3);
	//Bitmap Width (4 bytes)
	DIB[4] = getByte(width,0);
	DIB[5] = getByte(width,1);
	DIB[6] = getByte(width,2);
	DIB[7] = getByte(width,3);
	//Height (4 bytes)
	DIB[8] = getByte(height,0);
	DIB[9] = getByte(height,1);
	DIB[10] = getByte(height,2);
	DIB[11] = getByte(height,3);
	//Color Plane (2 bytes) is always 1
	DIB[12] = 1;
	DIB[13] = 0;
	//Color Depth (2 bytes) we are using 24 (three 8 bit colors)
	DIB[14] = getByte(24,0);
	DIB[15] = getByte(24,1);
	//Compression (4 bytes) 0 means none
	DIB[16] = 0;
	DIB[17] = 0;
	DIB[18] = 0;
	DIB[19] = 0;
	//Uncompressed Size (4 bytes)
	//0 because we aren't using compression
	DIB[20] = 0;
	DIB[21] = 0;
	DIB[22] = 0;
	DIB[23] = 0;
	//Horizontal Resolution (4 bytes)
	//Pixel per meter
	DIB[24] = getByte(resolution, 0);
	DIB[25] = getByte(resolution, 1);
	DIB[26] = getByte(resolution, 2);
	DIB[27] = getByte(resolution, 3);
	//Vertical Resolution (4 bytes)
	//Pixel per meter
	DIB[28] = getByte(resolution, 0);
	DIB[29] = getByte(resolution, 1);
	DIB[30] = getByte(resolution, 2);
	DIB[31] = getByte(resolution, 3);
	//Color Pallet (4 bytes)
	//0 means all
	DIB[32] = 0;
	DIB[33] = 0;
	DIB[34] = 0;
	DIB[35] = 0;
	//Number of important colors
	//0 mean all equal
	DIB[36] = 0;
	DIB[37] = 0;
	DIB[38] = 0;
	DIB[39] = 0;
	//Write the Header to the file in binary
	image.write(DIB, sizeDIB);
	//Exit the Function
	return;
}

void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels)
{
	char* pixel = new char[3];
	for(int i=0; i < pixels; i++)
	{
		pixel[2] = red[i];
		pixel[1] = green[i];
		pixel[0] = blue[i];
		image.write(pixel, 3);
	}
	return;
}


