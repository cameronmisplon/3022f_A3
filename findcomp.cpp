#include "PGMimageProcessor.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

int main(int argc, char* argv[])
{
	MSPCAM001::PGMimageProcessor image(argv[argc-1]);
	image.readImage();
	unsigned char threshold = 128;
	int minValidSize = 3;
	int maxValidSize = image.getMaxSize();
	int componentCount = 0;
	bool p = false;
	std::string outFileName;
	for(int i=1; i<argc-1;)
	{
		std::string option = std::string(argv[i]);
		if (option.compare("-s"))
		{
			minValidSize = std::stoi(std::string(argv[i+1]));
			maxValidSize = std::stoi(std::string(argv[i+2]));
			i = i+3;
		}
		else if(option.compare("-t"))
		{
			threshold = static_cast<unsigned char>((*(argv[i+1])));
			i=i+2;
		}
		else if(option.compare("-p"))
		{
			p=true;
			++i;
		}
		else if(option.compare("-w"))
		{
			outFileName = std::string(argv[i+1]);
			i=i+2;
		}
		else
		{
			std::cout << "Invalid option chosen: " << option << std::endl;
			++i;
		}
	}
	if ( minValidSize < image.getMaxSize() && static_cast<float>(threshold) > 0 && static_cast<float>(threshold) <255 && minValidSize>0 && maxValidSize>=minValidSize && maxValidSize <image.getMaxSize())
	{
		image.extractComponents(threshold, minValidSize);
		componentCount = image.filterComponentsBySize(minValidSize, maxValidSize);
		image.writeComponents(outFileName);
	}
	else
	{
		std::cout << "Either threshold, minValidSize or maxValidSize is out of bounds. Default values will be used." << std::endl;
		unsigned char threshold = 128;
		int minValidSize = 3;
		int maxValidSize = image.getMaxSize();
		image.extractComponents(threshold, minValidSize);
		componentCount = image.filterComponentsBySize(minValidSize, maxValidSize);
		image.writeComponents(outFileName);
	}
	if(p)
	{
		std::cout << "Number of components: " << componentCount << std::endl;
		image.printAll();
		std::cout << "Size of smallest component: " << image.getSmallestSize() << std::endl;
		std::cout << "Size of largest component: " << image.getLargestSize() << std::endl;
	}
}
		
		
