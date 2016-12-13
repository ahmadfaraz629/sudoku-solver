#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"

#include <math.h>
#include <set>
using std::set;


class Sudoku : public Searchable {
  
private:
    int size;
    vector<vector<set<int>>> solution;
    
    const int removeFromRow(const int row, const int value) {
        int elements = 0;
        for(auto i = 0; i < size; ++i) {
            auto erased = solution[row][i].erase(value);
            if(erased == 1)
                ++elements;
        }
        return elements;
    }
    
    const int removeFromCol(const int col, const int value) {
        int elements = 0;
        for(auto i = 0; i < size; ++i) {
            auto erased = solution[i][col].erase(value);
            if(erased == 1)
                ++elements;
        }
        return elements;
    }
    
    const int removeFromBox(const int boxRow, const int boxCol, const int value) {
        int elements = 0;
        for(auto i = sqrt(size) * boxRow; i < sqrt(size) * (boxRow + 1); ++i) {
            for(auto j = sqrt(size) * boxCol; j < sqrt(size) * (boxCol + 1); ++j) {
                auto erased = solution[i][j].erase(value);
                if(erased == 1)
                    ++elements;
            }
        }
        return elements;
    }
    
    const int totalSearchSize() const {
        int totalSize = 0;
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                totalSize += solution[i][j].size();
            }
        }
        return totalSize;
    }
    
    const int removeDuplicatesInRows(const int i) {
        int elements = 0;
        for(auto j = 0; j < size - 1; ++j) {
            for(auto k = j + 1; k < size; ++k) {
                if(solution[i][j].size() == 2 && solution[i][k].size() == 2 && solution[i][j] == solution[i][k]) {
                    set<int> duplicates = solution[i][j];
                    for(auto element : duplicates) {
                        elements += removeFromRow(i, element);
                        solution[i][j].insert(element);
                        --elements;
                        solution[i][k].insert(element);
                        --elements;
                    }
                }
            }
        }
        return elements;
    }
    
    const int removeDuplicatesInCols() {
        int elements = 0;
        for(auto j = 0; j < size; ++j) {
            for(auto i = 0; i < size - 1; ++i) {
                for(auto k = i + 1; k < size; ++k) {
                    if(solution[i][j].size() == 2 && solution[k][j].size() == 2 && solution[i][j] == solution[k][j]) {
                        set<int> duplicates = solution[i][j];
                        for(auto element : duplicates) {
                            elements += removeFromCol(j, element);
                            solution[i][j].insert(element);
                            --elements;
                            solution[k][j].insert(element);
                            --elements;
                        }
                    }
                }
            }
        }
        return elements;
    }
    
    const int removeDuplicatesInBoxes() {
        int elements = 0;
        for(auto boxRow = 0; boxRow < sqrt(size); ++boxRow) {
            for(auto boxCol = 0; boxCol < sqrt(size); ++boxCol) {
                // for each box
                
                for(auto i = sqrt(size) * boxRow; i < sqrt(size) * (boxRow + 1) - 1; ++i) {
                    for(auto j = sqrt(size) * boxCol; j < sqrt(size) * (boxCol + 1) - 1; ++j) {
                        for(auto k = i; k < sqrt(size) * (boxRow + 1); ++k) {
                            for(auto l = j; l < sqrt(size) * (boxCol + 1); ++l) {
                                if(i != k && j != l) {
                                    if(solution[i][j] == solution[k][l] && solution[i][j].size() == 2 && solution[k][l].size() == 2) {
                                        set<int> duplicates = solution[i][j];
                                        for(auto element : duplicates) {
                                            elements += removeFromBox(boxRow, boxCol, element);
                                            solution[i][j].insert(element);
                                            --elements;
                                            solution[k][l].insert(element);
                                            --elements;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
            }
        }
        return elements;
    }

    
public:
    Sudoku(const int sizeIn) : size(sizeIn), solution(size, vector<set<int>>(size)) {
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
		for(auto k = 1; k <= size; ++k) {
		    solution[i][j].insert(k);
		}
            }
        }
    }
    
    const int getSquare(const int row, const int col) const {
        if(solution[row][col].size() == 1)
            return *solution[row][col].begin();
        return -1;
    }
    
    const bool setSquare(const int row, const int col, const int value) {
        solution[row][col].clear();
        solution[row][col].insert(value);
        
        while(true) {
            int totalSize = totalSearchSize();
            int newSize = totalSize;
            
            // Normal looking
            for(auto i = 0; i < size; ++i) {
                for(auto j = 0; j < size; ++j) {
                    if(solution[i][j].size() == 1) {
                        int val = *solution[i][j].begin();
                        newSize -= removeFromRow(i, val);
                        newSize -= removeFromCol(j, val);
                        newSize -= removeFromBox((int) (i / sqrt(size)), (int) (j / sqrt(size)), val);
                        solution[i][j].insert(val);
                        ++newSize;
                    }
                    if(solution[i][j].size() == 0)
                        return false;
                }
                newSize -= removeDuplicatesInRows(i);
            }
            
            // Duplicates looking
            newSize -= removeDuplicatesInCols();
            newSize -= removeDuplicatesInBoxes();            
            
            if(newSize == totalSize)
                break;
            totalSize = newSize;
        }
        return true;
    }
    
    bool isSolution() const {
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                if(solution[i][j].size() != 1)
                    return false;
            }
        }
        return true;
    }
    
    void write(ostream & o) const {
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                if(solution[i][j].size() == 1)
                    o << *solution[i][j].begin();
                else
                    o << " ";
            }
            o << "\n";
        }
    }
    
    vector<unique_ptr<Searchable>> successors() const {
        vector<unique_ptr<Searchable>> successors;
        int minX = -1;
        int minY = -1;
        int minSize = 10;
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                if(solution[i][j].size() < minSize && solution[i][j].size() != 1) {
                    minSize = solution[i][j].size();
                    minX = i;
                    minY = j;
                }
            }
        }
        if(minSize != 10) {
            for(auto element : solution[minX][minY]) {
                Sudoku * sudokuCopy = new Sudoku(*this);
                if(sudokuCopy->setSquare(minX, minY, element)) {
                    successors.push_back(unique_ptr<Searchable>(sudokuCopy));
                } else {
                    delete sudokuCopy;
                }
            }
            if(successors.size() == 1 && !successors.front()->isSolution()) {
                unique_ptr<Searchable> descendant(std::move(successors.front()));
                successors.pop_back();
                successors = descendant->successors();
            }
            return successors;
        }
    }

    int heuristicValue() const {
        int heuristic = 0;
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                if(solution[i][j].size() > 1)
                    ++heuristic;
            }
        }
        return heuristic;
    }
};

#endif
