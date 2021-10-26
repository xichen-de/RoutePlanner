// PROJECT LICENSE
//
// This project was submitted by Xi Chen as part of the Nanodegree At Udacity.
//
// As part of Udacity Honor code, your submissions must be your own work, hence
// submitting this project as yours will cause you to break the Udacity Honor Code
// and the suspension of your account.
//
// Me, the author of the project, allow you to check the code as a reference, but if
// you submit it, it's your own responsibility if you get expelled.
//
// Copyright (c) 2021 Xi Chen
//
// Besides the above notice, the following license applies and this license notice
// must be included in all works derived from this project.
//
// MIT License
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
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y) : m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}


float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for (auto *neighbor_node: current_node->neighbors) {
        neighbor_node->parent = current_node;
        neighbor_node->g_value = current_node->g_value + current_node->distance(*neighbor_node);
        neighbor_node->h_value = CalculateHValue(neighbor_node);
        neighbor_node->visited = true;
        open_list.emplace_back(neighbor_node);
    }

}


RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(),
              [](RouteModel::Node *a, RouteModel::Node *b) {
                  return a->g_value + a->h_value < b->g_value + b->h_value;
              });
    RouteModel::Node *nextNode = open_list.front();
    open_list.erase(open_list.begin());
    return nextNode;
}


std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    do {
        path_found.emplace_back(*current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    } while (current_node != start_node);
    path_found.emplace_back(*current_node);
    std::reverse(path_found.begin(), path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;
    start_node->visited = true;
    open_list.emplace_back(start_node);
    while (!open_list.empty()) {
        current_node = NextNode();
        if (current_node == end_node) {
            m_Model.path = ConstructFinalPath(current_node);
            break;
        }
        AddNeighbors(current_node);
    }
}

