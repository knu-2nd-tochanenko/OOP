//
// Created by Vlad on 25.05.2019.
//

#ifndef INC_2_LAB_GRAPHLIST_H
#define INC_2_LAB_GRAPHLIST_H

#include "graph.h"

template <typename T>
class GraphList : public Graph<T> {
private:
    vector<vector<short int>> list;

    bool bfs(int ind, int fathers[], int length[], bool used[]) override {
        std::queue<int> nodes;
        int listSize = Graph<T>::elementsToIndex.size();
        for (int i = 0; i < listSize; i++) {
            fathers[i] = 0;
            length[i] = INT_MAX;
            used[i] = !Graph<T>::isUsed[i];
        }

        used[ind] = true;
        fathers[ind] = -1;
        nodes.push(ind);
        length[ind] = 0;

        while (!nodes.empty()) {
            int curVertex = nodes.front();
            nodes.pop();

            //	Scan all connected nodes
            int numberOfNodes = list[curVertex].size();
            for (int vectorIterator = 0; vectorIterator < numberOfNodes; vectorIterator++) {
                int connectedNumber = list[curVertex][vectorIterator];
                //	If vertex is not visited
                if (!used[connectedNumber]) {
                    used[connectedNumber] = true;
                    nodes.push(connectedNumber);
                    length[connectedNumber] = length[curVertex] + 1;
                    fathers[connectedNumber] = curVertex;
                }
            }
        }
        return true;
    }

    int distanceByIndex(int first, int second) override {
        int listSize = Graph<T>::elementsToIndex.size();
        int *fathers = new int[listSize];
        int *length = new int[listSize];
        bool *used = new bool[listSize];

        if (first * second < 0)
            return -1; //	There is no nodes with firstNode or secondNode values

        bfs(first, fathers, length, used);

        if (length[second] == INT_MAX)
            return -1;
        else
            return length[second];
    }

    bool removeByIndex(short int ind) override {
        // If index is invalid
        if (ind > list.size())
            return false;

        // Remove connections with ind node
        for (vector<short int> vec : list) {
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&ind](short int found){ return found == ind; }));
        }

        // Disconnect node from other nodes
        list[ind].clear();
        Graph<T>::isUsed[ind] = false;
        return true;
    }

    bool connectByIndex(short int from, short int to) override {
        // If indexes are invalid
        if ((from >= Graph<T>::elementsToIndex.size()) || (to >= Graph<T>::elementsToIndex.size()))
            return false;
        // Connect if doesn't connected
        Graph<T>::pushIfDoesntContain(list[from], to);
        Graph<T>::pushIfDoesntContain(list[to], from);
        return true;
    }

public:

    bool add(T value) override {
        // Add new value to elementToIndex vector if it doesn't exist yet
        if (!Graph<T>::has(value)) {
            Graph<T>::elementsToIndex.push_back(value);
            Graph<T>::isUsed.push_back(true);
            list.push_back(vector<short int>());
            return true;
        }
        Graph<T>::isUsed[Graph<T>::index(value)] = true;
        return false;
    }
};

#endif //INC_2_LAB_GRAPHLIST_H
