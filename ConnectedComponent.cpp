#include "ConnectedComponent.h"
#define CONNECTEDCOMPONENT MSPCAM001::ConnectedComponent

namespace MSPCAM001
{
	CONNECTEDCOMPONENT::ConnectedComponent() : pixelCount(0), identifier(0), coordinates(0)
	{}

	CONNECTEDCOMPONENT::~ConnectedComponent()
	{}

	CONNECTEDCOMPONENT::ConnectedComponent(const ConnectedComponent & rhs) : pixelCount(rhs.pixelCount), identifier(rhs.identifier), coordinates(0)
	{
		for( int i=0; i< rhs.coordinates.size(); ++i){
			coordinates.push_back(rhs.coordinates[i]);
		}
	}
	CONNECTEDCOMPONENT & CONNECTEDCOMPONENT::operator=(const CONNECTEDCOMPONENT & rhs){
		if (this !=&rhs){
			this->pixelCount = rhs.pixelCount;
			this->identifier = rhs.identifier;
			if((this->coordinates).size() != 0){
				coordinates.clear();
			}
			if (rhs.coordinates.size() !=0){
				for (int i=0; i<rhs.coordinates.size(); ++i){
					(this->coordinates).push_back(rhs.coordinates[i]);	
				}
			}
		}
		return *this;
	}
	CONNECTEDCOMPONENT::ConnectedComponent(CONNECTEDCOMPONENT && rhs) : pixelCount(rhs.pixelCount), identifier(rhs.identifier), coordinates(std::move(rhs.coordinates))
	{}
	CONNECTEDCOMPONENT & CONNECTEDCOMPONENT::operator=(CONNECTEDCOMPONENT && rhs)
	{
		if(this != &rhs)
		{
			this->pixelCount = rhs.pixelCount;
			this->identifier = rhs.identifier;
			if ((this->coordinates).size() != 0)
			{
				(this->coordinates).clear();
			}
			if (rhs.coordinates.size() != 0){
				this->coordinates= std::move(rhs.coordinates);
			}
		}
		return *this;
	}
	void CONNECTEDCOMPONENT::increment(){
		++pixelCount;
	}
	int CONNECTEDCOMPONENT::getPixelCount() const{
		return pixelCount;
	}
	int CONNECTEDCOMPONENT::getIdentifier() const{
		return identifier;
	}
	std::vector< std::pair<int,int> > CONNECTEDCOMPONENT::getCoordinates() const{
		return coordinates;
	}
	void CONNECTEDCOMPONENT::setIdentifier(const int identifier){
		this->identifier = identifier;
	}
	void CONNECTEDCOMPONENT::add( std::pair<int, int> a){
		coordinates.push_back(a);
	}
}
