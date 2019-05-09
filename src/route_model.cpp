#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int counter = 0;
    
    for(auto node : Nodes()){
        m_Nodes.push_back(Node(counter,this,node));
        counter++;
    }
    CreateNodeToRoadHashMap();

}

void RouteModel::CreateNodeToRoadHashMap(void){
    for(auto& road : Roads()){
        if(road.type == Model::Road::Type::Footway)
            continue;
        for(int node_idx : Ways()[road.way].nodes){
            if(node_to_road.find(node_idx)==node_to_road.end()){
                node_to_road[node_idx] = std::vector<const Model::Road*>();
            }
            node_to_road[node_idx].push_back(&road);
        }

    }
}

RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
        Node* closest_node = nullptr;
        Node node;

        for(int node_idx : node_indices){
            node = parent_model->SNodes()[node_idx];
            if(!node.visited && distance(node) != 0){
                if(closest_node == nullptr || distance(node)< distance(*closest_node))
                    closest_node = &parent_model->SNodes()[node_idx];
            }
        }
        return closest_node;
}

void RouteModel::Node::FindNeighbors(void){
        RouteModel::Node* new_neighbor = nullptr;
        for(auto& road : parent_model->node_to_road[this->index]){
            new_neighbor = FindNeighbor(parent_model->Ways()[road->way].nodes);
            if(new_neighbor)
                this->neighbors.push_back(new_neighbor);
        }
}

RouteModel::Node& RouteModel::FindClosestNode(float x, float y){
        Node input_node;
        input_node.x = x;
        input_node.y = y;
        float min_dist = std::numeric_limits<float>::max();
        float dist;
        int closest_idx = 0;
        for(auto& road : Roads()){
            if(road.type == Model::Road::Type::Footway)
                continue;
            for(auto node_idx : Ways()[road.way].nodes){
                dist = input_node.distance(SNodes()[node_idx]);
                if(dist < min_dist){
                closest_idx = node_idx;
                min_dist = dist; 
                }
            }
        }

        return SNodes()[closest_idx];
}