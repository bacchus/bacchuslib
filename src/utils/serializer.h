#pragma once

#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>

struct TestData {
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & BOOST_SERIALIZATION_NVP(someString)
           & BOOST_SERIALIZATION_NVP(someVector)
           & BOOST_SERIALIZATION_NVP(someComplx);
    }

    std::string someString;
    std::vector<int> someVector;
    std::map<int, std::vector<int> > someComplx;
};

class Serializer {
public:
    void saveBin(const std::string& fileName) {
        std::ofstream stream(fileName.c_str());
        boost::archive::binary_oarchive ar(stream);
        ar << BOOST_SERIALIZATION_NVP(testList);
    }

    void saveXml(const std::string& fileName) {
        std::ofstream stream(fileName.c_str());
        boost::archive::xml_oarchive ar(stream);
        ar << BOOST_SERIALIZATION_NVP(testList);
    }

    void loadBin(const std::string& fileName) {
        std::ifstream stream(fileName.c_str());
        assert(stream.is_open());
        boost::archive::binary_iarchive ar(stream);
        ar >> BOOST_SERIALIZATION_NVP(testList);
    }

    void loadXml(const std::string& fileName) {
        std::ifstream stream(fileName.c_str());
        assert(stream.is_open());
        boost::archive::xml_iarchive ar(stream);
        ar >> BOOST_SERIALIZATION_NVP(testList);
    }

    std::vector<TestData> testList;
};
