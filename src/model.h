// MIT License
//
// Copyright (c) 2021 Xi Chen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>

class Model {
public:
    struct Node {
        double x = 0.f;
        double y = 0.f;
    };

    struct Way {
        std::vector<int> nodes;
    };

    struct Road {
        enum Type {
            Invalid, Unclassified, Service, Residential,
            Tertiary, Secondary, Primary, Trunk, Motorway, Footway
        };
        int way;
        Type type;
    };

    struct Railway {
        int way;
    };

    struct Multipolygon {
        std::vector<int> outer;
        std::vector<int> inner;
    };

    struct Building : Multipolygon {
    };

    struct Leisure : Multipolygon {
    };

    struct Water : Multipolygon {
    };

    struct Landuse : Multipolygon {
        enum Type {
            Invalid, Commercial, Construction, Grass, Forest, Industrial, Railway, Residential
        };
        Type type;
    };

    Model(const std::vector <std::byte> &xml);

    auto MetricScale() const

    noexcept { return m_MetricScale; }

    auto &Nodes() const

    noexcept { return m_Nodes; }

    auto &Ways() const

    noexcept { return m_Ways; }

    auto &Roads() const

    noexcept { return m_Roads; }

    auto &Buildings() const

    noexcept { return m_Buildings; }

    auto &Leisures() const

    noexcept { return m_Leisures; }

    auto &Waters() const

    noexcept { return m_Waters; }

    auto &Landuses() const

    noexcept { return m_Landuses; }

    auto &Railways() const

    noexcept { return m_Railways; }

private:
    void AdjustCoordinates();

    void BuildRings(Multipolygon &mp);

    void LoadData(const std::vector <std::byte> &xml);

    std::vector <Node> m_Nodes;
    std::vector <Way> m_Ways;
    std::vector <Road> m_Roads;
    std::vector <Railway> m_Railways;
    std::vector <Building> m_Buildings;
    std::vector <Leisure> m_Leisures;
    std::vector <Water> m_Waters;
    std::vector <Landuse> m_Landuses;

    double m_MinLat = 0.;
    double m_MaxLat = 0.;
    double m_MinLon = 0.;
    double m_MaxLon = 0.;
    double m_MetricScale = 1.f;
};
