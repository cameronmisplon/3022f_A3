#ifndef __PGMIMAGEPROCESSOR__
#define __PGMIMAGEPROCESSOR__
#include "ConnectedComponent.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

namespace MSPCAM001
{
	class PGMimageProcessor
	{
		private:
		unsigned char * pixels;
		int Ncols;
		int Nrows;
		std::vector< std::unique_ptr< MSPCAM001::ConnectedComponent> > components;
		std::string name;
		public:
		PGMimageProcessor();
		PGMimageProcessor(std::string filename);
		~PGMimageProcessor();
		PGMimageProcessor(const PGMimageProcessor & rhs);
		PGMimageProcessor(PGMimageProcessor && rhs);
		PGMimageProcessor & operator=(const PGMimageProcessor & rhs);
		PGMimageProcessor & operator=(PGMimageProcessor && rhs);
		void readImage(void);
		int extractComponents(unsigned char threshold, int minValidSize);
		int filterComponentsBySize(int minSize, int maxSize);
		bool writeComponents(const std::string & outFileName);
		int getLargestSize(void) const;
		int getSmallestSize(void) const;
		int getMaxSize(void) const;
		void printComponentData(const ConnectedComponent & theComponent) const;
		void printAll(void) const;
	};
}
#endif
