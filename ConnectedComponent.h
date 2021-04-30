#ifndef __CONNECTEDCOMPONENT__
#define __CONNECETEDCOMPONENT__
#include <vector>
#include <utility>

namespace MSPCAM001{
	class ConnectedComponent
	{
		private:
		int pixelCount;
		int identifier;
		std::vector< std::pair<int,int> > coordinates;
		public:
		ConnectedComponent();
		~ConnectedComponent();
		ConnectedComponent(const ConnectedComponent & component);
		ConnectedComponent(ConnectedComponent && component);
		ConnectedComponent & operator=(const ConnectedComponent & rhs);
		ConnectedComponent & operator=(ConnectedComponent && rhs);
		void increment();
		int getPixelCount();
		int getIdentifier();
		std::vector< std::pair<int,int> > getCoordinates() const;
		void setIdentifier(const int identifier);
		void add( std::pair<int, int> a);
	};
}
#endif