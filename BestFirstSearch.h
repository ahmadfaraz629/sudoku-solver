#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
using std::unique_ptr;

#include <queue>
using std::priority_queue;

#include <vector>
using std::vector;


struct comparison {
    bool operator()(const unique_ptr<Searchable> & l, const unique_ptr<Searchable> & r) {  
       return l->heuristicValue() > r->heuristicValue();  
    }  
};

class BestFirstSearch {

protected:
    
    int nodes = 0;
    priority_queue<unique_ptr<Searchable>, vector<unique_ptr<Searchable>>, comparison> Q;
    
    
public:

    BestFirstSearch(unique_ptr<Searchable> && startFrom) {
        Q.push(std::move(startFrom));
    }
    
    int getNodesExpanded() const {
        return nodes;
    }
    
    Searchable * solve() {
            
        while (!Q.empty()) {
            if (Q.top()->isSolution()) {
                return Q.top().get(); 
            }
            ++nodes;

            unique_ptr<Searchable> current(std::move(const_cast<unique_ptr<Searchable> &> (Q.top()) ));

            Q.pop();
            
            vector<unique_ptr<Searchable>> successors = current->successors();
            
            for (auto & successor : successors) {
                Q.push(std::move(successor));
            }
        }
    
        return nullptr;
    }
};

// Do not edit below this line

#endif
