#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.
    void AStarSearch(void);
    float GetDistance() const {return distance;}

  private:
    // Add private variables or methods declarations here.
    void AddNeighbors(RouteModel::Node* current_node);
    float CalculateHValue(const RouteModel::Node* current_node){
        return current_node->distance(*end_node);
    }
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node* current_node); 
    RouteModel::Node* NextNode();
    std::vector<RouteModel::Node*> open_list;  
    RouteModel::Node* start_node;
    RouteModel::Node* end_node;
    float distance;
    RouteModel &m_Model;
};
