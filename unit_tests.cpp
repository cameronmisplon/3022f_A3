#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PGMimageProcessor.h"

using namespace MSPCAM001;

TEST_CASE("ConnectedComponent class unit tests")
{
	SECTION("Default Constructor")
	{
		ConnectedComponent cc;

		REQUIRE(cc.getPixelCount() == 0);
		REQUIRE(cc.getIdentifier() == 0);
		REQUIRE(cc.getCoordinates().getSize() ==0);
	}
	SECTION("Copy Constructor")
	{
		ConnectedComponent cc;
		cc.setIdentifier(15);
		cc.add( std::pair<int,int>(30,27));
		cc.increment();
		ConnectedComponent cc2(cc);
		REQUIRE(cc2.getPixelCount() == 1);
		REQUIRE(cc2.getIdentifer() == 15);
		REQUIRE(((cc2.getCoordinates())[0]).first ==30);
		REQUIRE(((cc1.getCoordinates())[0]).first ==30);
		REQUIRE(((cc2.getCoordinates())[0]).second ==27);
		REQUIRE(((cc1.getCoordinates())[0]).second ==27);
	}
	SECTION("Move Constructor")
	{
		ConnectedComponent cc;
		cc.setIdentifier(15);
		cc.add(std::pair<int,int>(30,27));
		cc.increment();
		ConnectedComponent cc2(std::move(cc));
		REQUIRE(cc.getCoordinates().size() == 0);
		REQUIRE(cc2.getPixelCount() ==1);
		REQUIRE(cc2.getIdentifier()==15);
		REQUIRE(((cc2.getCoordinates())[0]).first == 30);
		REQUIRE(((cc2.getCoordinates())[0]).second ==27);
	}
	SECTION("Copy Assignment Operator")
	{
		ConnectedComponent cc;
		cc.setIdentifier(15);
		cc.add(std::pair<int,int>(30,27));
		ConnectedComponent cc2;
		cc2 = cc;
		REQUIRE(cc2.getPixelCount() == 1);
		REQUIRE(cc2.getIdentifer() == 15);
		REQUIRE(((cc2.getCoordinates())[0]).first ==30);
		REQUIRE(((cc1.getCoordinates())[0]).first ==30);
		REQUIRE(((cc2.getCoordinates())[0]).second ==27);
		REQUIRE(((cc1.getCoordinates())[0]).second ==27);
	}
	SECTION("Move Assignment Operator")
	{
		ConnectedComponent cc;
		cc.setIdentifier(15);
		cc.add(std::pair<int,int>(30,27));
		cc.increment();
		ConnectedComponent cc2;
		cc2 = std::move(cc);
		REQUIRE(cc.getCoordinates().size() == 0);
		REQUIRE(cc2.getPixelCount() ==1);
		REQUIRE(cc2.getIdentifier()==15);
		REQUIRE(((cc2.getCoordinates())[0]).first == 30);
		REQUIRE(((cc2.getCoordinates())[0]).second ==27);
	}
}

TEST_CASE("PGMimageProcessor unit tests")
{
	SECTION("Testing getMaxSize, getSmallestSize, getLargestSize")
	// All other functions require access to PGMimageProcessors internals so it's impossible to unit test
	{
		PGMimageProcessor pgm(chess.pgm);
		pgm.readImage();
		int minValidSize = 3;
		int maxValidSize = image.getMaxSize();
		unsigned char threshold = 128;
		pgm.extractComponents(threshold, minValidSize);
		pgm.filterComponentsBySize(minValidSize, maxValidSize);
		REQUIRE(minValidSize <= image.getSmallestSize());
		REQUIRE(maxValidSize > image.getLargestSize());
	}
}

