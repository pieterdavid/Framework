#pragma once

#include <boost/regex.hpp>
#include <map>

namespace tinyxml2 {
    class XMLElement;
}

template<typename T>
class Range {
    public:
        Range(T from, T to):
            m_from(from), m_to(to) {}

        T from() const {
            return m_from;
        }

        T to() const {
            return m_to;
        }

        bool in(T value) const {
            return value >= m_from && value <= m_to;
        }

        bool operator<(const Range<T>& other) const {
            return m_from < other.m_from;
        }

        template<typename U>
            friend std::ostream& operator<<(std::ostream& stream, const Range<U>& range);

    private:
        T m_from;
        T m_to;
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Range<T>& range)
{
    stream << "[" << range.from() << ", " << range.to() << "]";

    return stream;
}

class HLTService {
  public:

    using PathName = boost::regex;
    using PathVector =  std::vector<PathName>;

    HLTService(const std::string& filename):
      m_cachedRange(nullptr), m_cachedVector(nullptr) {
        parse(filename);
      }

    void print();
    const PathVector& getPaths(uint64_t run);

  private:
    std::map<Range<uint64_t>, PathVector> m_paths;

    const Range<uint64_t>* m_cachedRange;
    const PathVector* m_cachedVector;

    bool parse(const std::string& filename);
    bool parseRunsElement(const tinyxml2::XMLElement* runs);
};
