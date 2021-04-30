#include "PGMimageProcessor.h"
#define PGMIMAGEPROCESSOR MSPCAM001::PGMimageProcessor

namespace MSPCAM001
{
	PGMIMAGEPROCESSOR::PGMimageProcessor() : pixels(nullptr), Ncols(0), Nrows(0), components(), name("")
	{}
	PGMIMAGEPROCESSOR::PGMimageProcessor(std::string filename) : pixels(nullptr), Ncols(0), Nrows(0), components(), name(filename)
	{}
	PGMIMAGEPROCESSOR::~PGMimageProcessor()
	{
		if (pixels != nullptr){
			delete [] pixels;
		}
	}
	PGMIMAGEPROCESSOR::PGMimageProcessor(const PGMIMAGEPROCESSOR & rhs): pixels(nullptr), Ncols(rhs.Ncols), Nrows(rhs.Nrows), components(), name(rhs.name)
	{
		this->pixels = new unsigned char[Nrows*Ncols];
		for (int i=0; i<Ncols*Nrows; ++i)
		{
			(this->pixels)[i] = (rhs.pixels)[i];
		}
		for (int j=0;j<rhs.components.size();++j)
		{
			(this->components).push_back( std::unique_ptr<MSPCAM001::ConnectedComponent>(new MSPCAM001::ConnectedComponent(*(rhs.components[j]))));
		}
	}
	PGMIMAGEPROCESSOR::PGMimageProcessor(PGMIMAGEPROCESSOR && rhs) : pixels(rhs.pixels), Ncols(rhs.Ncols), Nrows(rhs.Nrows), components(std::move(rhs.components)), name(std::move(rhs.name))
	{
		if (rhs.pixels != nullptr)
		{
			rhs.pixels = nullptr;
		}
	}
	PGMIMAGEPROCESSOR & PGMimageProcessor::operator=(const PGMIMAGEPROCESSOR & rhs)
	{
		(this->Ncols) = rhs.Ncols;
		(this->Nrows) = rhs.Nrows;
		(this->name) = rhs.name;
		(this->components).clear();
		if ((this->pixels) != nullptr)
		{
			delete [] pixels;
			pixels = nullptr;
		}
		if (rhs.pixels != nullptr){
			(this->pixels) = new unsigned char[Nrows*Ncols];
			for (int i=0; i<Nrows*Ncols; ++i)
			{
				(this->pixels)[i] = rhs.pixels[i];
			}
		}
		for (int j=0;j<rhs.components.size();++j)
		{
			(this->components).push_back( std::unique_ptr<MSPCAM001::ConnectedComponent>(new MSPCAM001::ConnectedComponent(*(rhs.components[j]))));
		}
		return *this;
	}
	PGMIMAGEPROCESSOR & PGMimageProcessor::operator=(PGMimageProcessor && rhs)
	{
		(this->Ncols) = rhs.Ncols;
		(this->Nrows) = rhs.Nrows;
		(this->name) = std::move(rhs.name);
		(this->components).clear();
		if ((this->pixels) != nullptr)
		{
			delete [] pixels;
			pixels = nullptr;
		}
		if (rhs.pixels != nullptr){
			(this->pixels) = new unsigned char[Nrows*Ncols];
			for (int i=0; i<Nrows*Ncols; ++i)
			{
				(this->pixels)[i] = rhs.pixels[i];
			}
			rhs.pixels = nullptr;
		}
		(this->components) = std::move(rhs.components);
		rhs.components.clear();
		return *this;
	}
	void PGMIMAGEPROCESSOR::readImage(void)
	{
		std::ifstream in(name, std::ios::binary | std::ios::in);
		std::string comments;
		getline(in,comments);
		getline(in,comments);
		while (comments.substr(0,1).compare("#")==0){
			getline(in,comments);
		}
		std::istringstream iss(comments);
		iss >> Ncols >> Nrows;
		getline(in,comments);
		pixels = new unsigned char[Nrows*Ncols];
		in.read(reinterpret_cast<char *>(pixels), Nrows*Ncols);
		in.close();
	}
	int PGMIMAGEPROCESSOR::extractComponents(unsigned char threshold, int minValidSize)
	{
		int componentcount =0;
		std::unique_ptr<MSPCAM001::ConnectedComponent> component(new MSPCAM001::ConnectedComponent);
		for (int i=0; i<Ncols*Nrows; ++i)
		{
			component->setIdentifier(componentcount);
			float pixelvalue = static_cast<float>(pixels[i]);
			pixels[i]=0;
			if (pixelvalue >= static_cast<float>(threshold)){
				std::cout << pixelvalue << " : starter" << std::endl;    //1
				std::queue<int> queue1;
				queue1.push(i);
				while( queue1.size() > 0)
				{
					int arr[4];
					arr[0] = (queue1.front()-Ncols > 0) ? (queue1.front()-Ncols) : 0;
					arr[1] = (queue1.front()+Ncols < Nrows*Ncols) ? (queue1.front()+Ncols) : 0;
					arr[2] = (queue1.front()%Ncols != 0) ? (queue1.front()-1) : 0;
					arr[3] = ((queue1.front()+1)%Ncols != 0) ? (queue1.front()+1) : 0;
					for (int j=0; j<4; ++j)
					{
						if (static_cast<float>(pixels[arr[j]]) >= static_cast<float>(threshold))
						{
							std::cout << static_cast<float>(pixels[arr[j]]) << ": part of component" << std::endl;  //2
							queue1.push(arr[j]);
							pixels[arr[j]] = 0;
						}
					}
					component->increment();
					component->add( std::pair<int,int>(queue1.front()%Ncols, queue1.front()/Ncols));
					queue1.pop();
				}
				if (component->getPixelCount() >= minValidSize)
				{
					components.push_back(std::move(component));
					++componentcount;
				}
				component.reset(new MSPCAM001::ConnectedComponent);
			}
		}
		delete [] pixels;
		pixels = nullptr;
		return componentcount;
	}
	int PGMIMAGEPROCESSOR::filterComponentsBySize(int minSize, int maxSize)
	{
		int counter = components.size();
		for(std::vector<std::unique_ptr<MSPCAM001::ConnectedComponent> >::iterator i = components.begin(); i!=components.end();)
		{
			if ((*i)->getPixelCount() < minSize || (*i)->getPixelCount() >maxSize )
			{
				--counter;
				i = components.erase(i);
			}
			else
			{
				++i;
			}	
		}
		return counter;
	}
	bool PGMIMAGEPROCESSOR::writeComponents(const std::string & outFileName)
	{
		std::ofstream outfile;
		outfile.open(outFileName, std::ios::binary | std::ofstream::trunc);
		std::string size = std::to_string(Ncols) + " " + std::to_string(Nrows);
		outfile << "P5" << std::endl << "#This image was generated by MSPCAM001::PGMimageProcessor" << std::endl << size << std::endl << "255" << std::endl;
		pixels = new unsigned char[Nrows*Ncols];
		memset(pixels,0,Nrows*Ncols);
		for (int j=0; j<components.size(); j++)
		{
			MSPCAM001::ConnectedComponent component((*(components[j])));
			const auto temp = component.getCoordinates();
			for (int i=0;i<temp.size();++i)
			{
				int multiple = ((temp[i].second)*Ncols)+temp[i].first;
				pixels[multiple] = 255;
			}
		}
		outfile.write(reinterpret_cast<const char*>(pixels), Nrows*Ncols);
		outfile.close();
		delete [] pixels;
		pixels = nullptr;
		return true;
	}
	int PGMIMAGEPROCESSOR::getLargestSize(void) const
	{
		int max = 0;
		for (int i=0; i<components.size(); ++i)
		{
			if (components[i]->getPixelCount() > max)
			{
				max = components[i]->getPixelCount();
			}
		}
		return max;
	}
	int PGMIMAGEPROCESSOR::getSmallestSize(void) const
	{
		int min = 0;
		for (int i=0; i<components.size(); ++i)
		{
			if (components[i]->getPixelCount() < min || min == 0)
			{
				min = components[i]->getPixelCount();
			}
		}
		return min;
	}
	void PGMIMAGEPROCESSOR::printComponentData( const MSPCAM001::ConnectedComponent & theComponent) const
	{
		int position =components.size();
		for (int i=0; i<components.size(); ++i)
		{
			if( (theComponent.getIdentifier() == components[i]->getIdentifier()) && (theComponent.getPixelCount() == components[i]->getPixelCount()) )
			{
				position = i;
			}
		}
		if (position != components.size())
		{
			std::cout << "ID: " << components[position]->getIdentifier() << " Pixel Count: " << components[position]->getPixelCount() << std::endl;
		}
		else
		{
			std::cout << "No matching component was found" << std::endl;
		}
	}
	void PGMIMAGEPROCESSOR::printAll() const
	{
		for(int i=0; i<components.size(); ++i)
		{
			MSPCAM001::ConnectedComponent temp((*(components[i])));
			printComponentData(temp);
		}
	}
			
	int PGMIMAGEPROCESSOR::getMaxSize() const
	{
		return Ncols*Nrows;
	}
}
	
