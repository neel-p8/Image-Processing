#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <cmath>
using namespace std;

class Image {
private:
	string name;
public:
	struct Header {
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;

		bool operator==(Header& rhs) const;
		bool operator!=(Header& rhs) const;

		Header() {
			idLength = 0;
			colorMapType = 0;
			dataTypeCode = 0;
			colorMapOrigin = 0;
			colorMapLength = 0;
			colorMapDepth = 0;
			xOrigin = 0;
			yOrigin = 0;
			width = 0;
			height = 0;
			bitsPerPixel = 0;
			imageDescriptor = 0;
		}

		Header(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin,
			short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin, short width,
			short height, char bitsPerPixel, char imageDescriptor) {
			this->idLength = idLength;
			this->colorMapType = colorMapType;
			this->dataTypeCode = dataTypeCode;
			this->colorMapOrigin = colorMapOrigin;
			this->colorMapLength = colorMapLength;
			this->colorMapDepth = colorMapDepth;
			this->xOrigin = xOrigin;
			this->yOrigin = yOrigin;
			this->width = width;
			this->height = height;
			this->bitsPerPixel = bitsPerPixel;
			this->imageDescriptor = imageDescriptor;
		}
	};
	struct Pixel {
		unsigned char blue, green, red;

		Pixel() {
			blue = 0;
			green = 0;
			red = 0;
		}

		Pixel(unsigned char  blue, unsigned char green, unsigned char red) {
			this->blue = blue;
			this->green = green;
			this->red = red;
		}

		bool operator==(Pixel& rhs) const;
		bool operator!=(Pixel& rhs) const;
	};
	vector<Pixel> pixels;
	Header header;
	Image();
	Image(const string& name);
	void writePath(const string& outName);

	bool operator==(Image& rhs) const;
	bool operator!=(Image& rhs) const;
};

Image Multiply(const Image& topLayer, const Image& bottomLayer);
Image Subtraction(const Image& topLayer, const Image& bottomLayer);
Image Screen(const Image& topLayer, const Image& bottomLayer);
Image Overlay(const Image& topLayer, const Image& bottomLayer);
Image AddGreen200(const Image& topLayer);
Image AddColor(const Image& topLayer, int value, string color);
Image ScaleRedBlue(const Image& topLayer);
Image ScaleColor(const Image& topLayer, int value, string color);
Image SplitImage(string color, const Image& topLayer);
Image CombineFiles(const Image& blueLayer, const Image& greenLayer, const Image& redLayer);
Image Rotate(const Image& topLayer);

Image::Image()
{
	name = "Image";
}

// read binary file
Image::Image(const string& name)
{
	ifstream inFile(name, ios_base::binary);
	if (inFile) {
		inFile.read(&header.idLength, sizeof(header.idLength));
		inFile.read(&header.colorMapType, sizeof(header.colorMapType));
		inFile.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
		inFile.read((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		inFile.read((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		inFile.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
		inFile.read((char*)&header.xOrigin, sizeof(header.xOrigin));
		inFile.read((char*)&header.yOrigin, sizeof(header.yOrigin));
		inFile.read((char*)&header.width, sizeof(header.width));
		inFile.read((char*)&header.height, sizeof(header.height));
		inFile.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		inFile.read(&header.imageDescriptor, sizeof(header.imageDescriptor));

		int size = header.height * header.width;
		pixels.resize(size);

		for (int i = 0; i < pixels.size(); i++) {
			inFile.read((char*)&pixels[i].blue, sizeof(pixels[i].blue));
			inFile.read((char*)&pixels[i].green, sizeof(pixels[i].green));
			inFile.read((char*)&pixels[i].red, sizeof(pixels[i].red));
		};
	}
	else {
		throw runtime_error("Failed to open");
	}
}

// write file based on function
void Image::writePath(const string& outName)
{
	ofstream outFile(outName, ios_base::binary);
	if (outFile) {
		outFile.write(&header.idLength, sizeof(header.idLength));
		outFile.write(&header.colorMapType, sizeof(header.colorMapType));
		outFile.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
		outFile.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		outFile.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		outFile.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
		outFile.write((char*)&header.xOrigin, sizeof(header.xOrigin));
		outFile.write((char*)&header.yOrigin, sizeof(header.yOrigin));
		outFile.write((char*)&header.width, sizeof(header.width));
		outFile.write((char*)&header.height, sizeof(header.height));
		outFile.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		outFile.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

		int size = header.height * header.width;

		for (int i = 0; i < size; i++) {
			outFile.write((char*)&pixels[i].blue, sizeof(pixels[i].blue));
			outFile.write((char*)&pixels[i].green, sizeof(pixels[i].green));
			outFile.write((char*)&pixels[i].red, sizeof(pixels[i].red));
		}
		outFile.close();
	}
	else {
		throw runtime_error("Failed to open");
	}
}

// check if image output and example are same
bool Image::operator==(Image& rhs) const
{
	if (header != rhs.header) {
		cout << "headers dont match" << endl;
		return false;
	}
	if (pixels.size() != rhs.pixels.size()) {
		cout << "pixel size dont match" << endl;
		return false;
	}
	for (int i = 0; i < pixels.size(); i++) {
		if (pixels[i] != rhs.pixels[i]) {
			cout << "Pixel " << i << " dont match";
			cout << "ours: " << "red: " << (int)pixels[i].red << " green: " << (int)pixels[i].green << " blue: " << (int)pixels[i].blue << endl;
			cout << "expected: " << "red: " << (int)rhs.pixels[i].red << " green: " << (int)rhs.pixels[i].green << " blue: " << (int)rhs.pixels[i].blue << endl;

			return false;
		}
	}
	return true;
}

bool Image::operator!=(Image& rhs) const
{
	return !(operator == (rhs));
}

// check if image output header and example are same
bool Image::Header::operator==(Header& rhs) const
{
	if (idLength != rhs.idLength) {
		return false;
	}
	if (colorMapType != rhs.colorMapType) {
		return false;
	}
	if (dataTypeCode != rhs.dataTypeCode) {
		return false;
	}
	if (colorMapOrigin != rhs.colorMapOrigin) {
		return false;
	}
	if (colorMapLength != rhs.colorMapLength) {
		return false;
	}
	if (colorMapDepth != rhs.colorMapDepth) {
		return false;
	}
	if (xOrigin != rhs.xOrigin) {
		return false;
	}
	if (yOrigin != rhs.yOrigin) {
		return false;
	}
	if (width != rhs.width) {
		return false;
	}
	if (height != rhs.height) {
		return false;
	}
	if (bitsPerPixel != rhs.bitsPerPixel) {
		return false;
	}
	if (imageDescriptor != rhs.imageDescriptor) {
		return false;
	}
	return true;
}

bool Image::Header::operator!=(Header& rhs) const
{
	return !(operator==(rhs));
}

// check if image output pixel data and example are same
bool Image::Pixel::operator==(Pixel& rhs) const
{
	if (red != rhs.red) {
		return false;
	}
	if (green != rhs.green) {
		return false;
	}
	if (blue != rhs.blue) {
		return false;
	}
	return true;
}

bool Image::Pixel::operator!=(Pixel& rhs) const
{
	return !(operator==(rhs));
}

// multiply images with layers
Image Multiply(const Image& topLayer, const Image& bottomLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		unsigned char blue = round(((float)topLayer.pixels[i].blue * bottomLayer.pixels[i].blue) / 255.0f);
		unsigned char green = round(((float)topLayer.pixels[i].green * bottomLayer.pixels[i].green) / 255.0f);
		unsigned char red = round(((float)topLayer.pixels[i].red * bottomLayer.pixels[i].red) / 255.0f);

		Image::Pixel pixelValues = { blue, green, red };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// subtract images with layers
Image Subtraction(const Image& topLayer, const Image& bottomLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		int blue = static_cast<int>((topLayer.pixels[i].blue - bottomLayer.pixels[i].blue));
		int green = static_cast<int>((topLayer.pixels[i].green - bottomLayer.pixels[i].green));
		int red = static_cast<int>((topLayer.pixels[i].red - bottomLayer.pixels[i].red));

		blue = max(0, min(255, blue));
		green = max(0, min(255, green));
		red = max(0, min(255, red));

		Image::Pixel pixel_values = { static_cast<unsigned char>(blue), static_cast<unsigned char>(green), static_cast<unsigned char>(red) };
		result.pixels.push_back(pixel_values);
	}
	return result;
}

// screen images
Image Screen(const Image& topLayer, const Image& bottomLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		unsigned char blue = round((1 - ((1 - (float)topLayer.pixels[i].blue / 255) * (1 - (float)bottomLayer.pixels[i].blue / 255))) * 255);
		unsigned char green = round((1 - ((1 - (float)topLayer.pixels[i].green / 255) * (1 - (float)bottomLayer.pixels[i].green / 255))) * 255);
		unsigned char red = round((1 - ((1 - (float)topLayer.pixels[i].red / 255) * (1 - (float)bottomLayer.pixels[i].red / 255))) * 255);

		Image::Pixel pixelValues = { blue, green, red };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// image overlay
Image Overlay(const Image& topLayer, const Image& bottomLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {

		unsigned char blue = 0, green = 0, red = 0;
		Image::Pixel pixelValues = { blue, green, red };

		if ((float)bottomLayer.pixels[i].blue / 255 <= 0.5) {
			unsigned char blue = round(2 * ((float)topLayer.pixels[i].blue * bottomLayer.pixels[i].blue) / 255.0f);
			pixelValues.blue = blue;
		}
		else {
			unsigned char blue = round((1 - 2 * ((1 - (float)topLayer.pixels[i].blue / 255) * (1 - (float)bottomLayer.pixels[i].blue / 255))) * 255);
			pixelValues.blue = blue;
		}
		if ((float)bottomLayer.pixels[i].green / 255 <= 0.5) {
			unsigned char green = round(2 * ((float)topLayer.pixels[i].green * bottomLayer.pixels[i].green) / 255.0f);
			pixelValues.green = green;
		}
		else {
			unsigned char green = round((1 - 2 * ((1 - (float)topLayer.pixels[i].green / 255) * (1 - (float)bottomLayer.pixels[i].green / 255))) * 255);
			pixelValues.green = green;
		}
		if ((float)bottomLayer.pixels[i].red / 255 <= 0.5) {
			unsigned char red = round(2 * ((float)topLayer.pixels[i].red * bottomLayer.pixels[i].red) / 255.0f);
			pixelValues.red = red;
		}
		else {
			unsigned char red = round((1 - 2 * ((1 - (float)topLayer.pixels[i].red / 255) * (1 - (float)bottomLayer.pixels[i].red / 255))) * 255);
			pixelValues.red = red;
		}
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// add 200 to green channel
Image AddGreen200(const Image& topLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		int green = static_cast<int>(topLayer.pixels[i].green + 200);
		green = max(0, min(255, green));

		Image::Pixel pixelValues = { topLayer.pixels[i].blue, static_cast<unsigned char> (green), topLayer.pixels[i].red };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// add value to user selected color
Image AddColor(const Image& topLayer, int value, string color)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		if (color == "blue") {
			int blue = static_cast<int>(topLayer.pixels[i].blue + value);
			blue = max(0, min(255, blue));
			Image::Pixel pixelValues = { static_cast<unsigned char> (blue), topLayer.pixels[i].green, topLayer.pixels[i].red };
			result.pixels.push_back(pixelValues);
		}
		if (color == "green") {
			int green = static_cast<int>(topLayer.pixels[i].green + value);
			green = max(0, min(255, green));
			Image::Pixel pixelValues = { topLayer.pixels[i].blue, static_cast<unsigned char> (green), topLayer.pixels[i].red };
			result.pixels.push_back(pixelValues);
		}
		if (color == "red") {
			int red = static_cast<int>(topLayer.pixels[i].red + value);
			red = max(0, min(255, red));
			Image::Pixel pixelValues = { topLayer.pixels[i].blue, topLayer.pixels[i].green, static_cast<unsigned char> (red) };
			result.pixels.push_back(pixelValues);
		}
	}
	return result;
}

// increase intensity of red while negating blue
Image ScaleRedBlue(const Image& topLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		int red = static_cast<int>(topLayer.pixels[i].red * 4);
		red = max(0, min(255, red));

		int blue = static_cast<int>(topLayer.pixels[i].blue * 0);
		blue = max(0, min(255, blue));

		Image::Pixel pixelValues = { static_cast<unsigned char> (blue), topLayer.pixels[i].green, static_cast<unsigned char> (red) };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// scale value to user selected color
Image ScaleColor(const Image& topLayer, int value, string color)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {
		if (color == "blue") {
			int blue = static_cast<int>(topLayer.pixels[i].blue * value);
			blue = max(0, min(255, blue));
			Image::Pixel pixelValues = { static_cast<unsigned char> (blue), topLayer.pixels[i].green, topLayer.pixels[i].red };
			result.pixels.push_back(pixelValues);
		}
		if (color == "green") {
			int green = static_cast<int>(topLayer.pixels[i].green * value);
			green = max(0, min(255, green));
			Image::Pixel pixelValues = { topLayer.pixels[i].blue, static_cast<unsigned char> (green), topLayer.pixels[i].red };
			result.pixels.push_back(pixelValues);
		}
		if (color == "red") {
			int red = static_cast<int>(topLayer.pixels[i].red * value);
			red = max(0, min(255, red));
			Image::Pixel pixelValues = { topLayer.pixels[i].blue, topLayer.pixels[i].green, static_cast<unsigned char> (red) };
			result.pixels.push_back(pixelValues);
		}
	}
	return result;
}

// split image into 3 channels based on rgb
Image SplitImage(string color, const Image& topLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = 0; i < topLayer.header.width * topLayer.header.height; i++) {

		if (color == "blue") {
			unsigned char blue = topLayer.pixels[i].blue;

			Image::Pixel pixelValues = { blue, blue, blue };
			result.pixels.push_back(pixelValues);
		}
		if (color == "green") {
			unsigned char green = topLayer.pixels[i].green;

			Image::Pixel pixelValues = { green, green, green };
			result.pixels.push_back(pixelValues);
		}
		if (color == "red") {
			unsigned char red = topLayer.pixels[i].red;

			Image::Pixel pixelValues = { red, red, red };
			result.pixels.push_back(pixelValues);
		}
	}
	return result;
}

// combine 3 color files into one
Image CombineFiles(const Image& blueLayer, const Image& greenLayer, const Image& redLayer)
{
	Image result;
	result.header = blueLayer.header;

	for (int i = 0; i < blueLayer.header.width * blueLayer.header.height; i++) {
		unsigned char blue = blueLayer.pixels[i].blue;
		unsigned char green = greenLayer.pixels[i].green;
		unsigned char red = redLayer.pixels[i].red;

		Image::Pixel pixelValues = { blue, green, red };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

// flip image upside down 180 degrees
Image Rotate(const Image& topLayer)
{
	Image result;
	result.header = topLayer.header;

	for (int i = (topLayer.header.width * topLayer.header.height) - 1; i >= 0; i--) {
		unsigned char blue = topLayer.pixels[i].blue;
		unsigned char green = topLayer.pixels[i].green;
		unsigned char red = topLayer.pixels[i].red;

		Image::Pixel pixelValues = { blue, green, red };
		result.pixels.push_back(pixelValues);
	}
	return result;
}

int main(int argc, const char* argv[]) {

	// help method if no argument or 1
	if (argc == 1 || (argc == 2 && string(argv[1]) == "--help")) {
		cout << "Project 2: Image Processing, Fall 2023" << endl << endl;
		cout << "Usage:" << endl;
		cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
	}
	else {
		// check validity of output file name and source file
		string outputFile = argv[1];
		if (argc == 2 || outputFile.substr(outputFile.size() - 3) != "tga") {
			cout << "Invalid file name." << endl;
			return 1;
		}
		string sourceFile = argv[2];
		if (sourceFile.substr(sourceFile.size() - 3) != "tga") {
			cout << "Invalid file name." << endl;
			return 1;
		}
		ifstream sourceFileCheck(sourceFile, ios::binary);
		if (!sourceFileCheck) {
			cout << "File does not exist." << endl;
			return 1;
		}

		// initalize tracking image as sourcefile
		Image trackingImage(sourceFile);

		for (int i = 3; i < argc; i++) {
			string method = argv[i];

			//check for first 4 methods, check if enough args
			if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
				if (i + 1 >= argc) {
					cout << "Missing argument." << endl;
					return 1;
				}
				//check validity
				string secondFile = argv[i + 1];
				if (secondFile.substr(secondFile.size() - 3) != "tga") {
					cout << "Invalid argument, invalid file name." << endl;
					return 1;
				}
				ifstream secondFileCheck(secondFile, ios::binary);
				if (!secondFileCheck) {
					cout << "Invalid argument, file does not exist." << endl;
					return 1;
				}
				// output image
				Image secondImage(secondFile);
				if (method == "multiply") {
					trackingImage = Multiply(trackingImage, secondImage);
				}
				if (method == "subtract") {
					trackingImage = Subtraction(trackingImage, secondImage);
				}
				if (method == "overlay") {
					trackingImage = Overlay(trackingImage, secondImage);
				}
				if (method == "screen") {
					trackingImage = Screen(secondImage, trackingImage);
				}
				i++;
			}

			// check 5th method and ensure implementation of 2 new layers
			else if (method == "combine") {
				if (i + 2 >= argc) {
					cout << "Missing argument." << endl;
					return 1;
				}
				//check validity
				string greenFile = argv[i + 1];
				string blueFile = argv[i + 2];
				if (greenFile.substr(greenFile.size() - 3) != "tga" || blueFile.substr(blueFile.size() - 3) != "tga") {
					cout << "Invalid argument, invalid file name." << endl;
					return 1;
				}
				ifstream greenFileCheck(greenFile, ios::binary);
				ifstream blueFileCheck(blueFile, ios::binary);
				if (!greenFileCheck) {
					cout << "Invalid argument, file does not exist." << endl;
					return 1;
				}
				if (!blueFileCheck) {
					cout << "Invalid argument, file does not exist." << endl;
					return 1;
				}
				Image greenImage(greenFile);
				Image blueImage(blueFile);

				trackingImage = CombineFiles(blueImage, greenImage, trackingImage);
				i += 2;
			}
			// implement methods without arguments
			else if (method == "flip") {
				trackingImage = Rotate(trackingImage);
			}
			else if (method == "onlyred") {
				trackingImage = SplitImage("red", trackingImage);
			}
			else if (method == "onlygreen") {
				trackingImage = SplitImage("green", trackingImage);
			}
			else if (method == "onlyblue") {
				trackingImage = SplitImage("blue", trackingImage);
			}
			// implement scale and add. no image arguments
			else if (method == "addred" || method == "addgreen" || method == "addblue" || method == "scalered" || method == "scalegreen" || method == "scaleblue") {
				if (i + 1 >= argc) {
					cout << "Missing argument." << endl;
					return 1;
				}
				// use try catch to ensure stoi works as intended
				try {
					int value = stoi(argv[i + 1]);
					if (method == "addred") {
						trackingImage = AddColor(trackingImage, value, "red");
					}
					else if (method == "addgreen") {
						trackingImage = AddColor(trackingImage, value, "green");
					}
					else if (method == "addblue") {
						trackingImage = AddColor(trackingImage, value, "blue");
					}
					else if (method == "scalered") {
						trackingImage = ScaleColor(trackingImage, value, "red");
					}
					else if (method == "scalegreen") {
						trackingImage = ScaleColor(trackingImage, value, "green");
					}
					else if (method == "scaleblue") {
						trackingImage = ScaleColor(trackingImage, value, "blue");
					}
					i++;
				}
				catch (const invalid_argument& e) {
					cout << "Invalid argument, expected number." << endl;
					return 1;
				}
			}
			else {
				cout << "Invalid method name." << endl;
				return 1;
			}
			trackingImage.writePath(outputFile);
		}
	}
	return 0;
}