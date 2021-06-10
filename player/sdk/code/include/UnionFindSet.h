#pragma once

#include <vector>

class UnionFindSet {
public:
    explicit UnionFindSet(size_t num) 
        : parents_(std::vector<size_t>(num)) 
        , ranks_(std::vector<size_t>(num, 0))
    {
        for(size_t i = 0; i < num; ++i) {
            parents_[i] = i;
        }
    }

    UnionFindSet(const UnionFindSet&) = delete;
    UnionFindSet(UnionFindSet&&) = delete;
    
    size_t find(size_t x) 
    {
        return x == parents_[x] ? x : (parents_[x] = find(parents_[x]));
    }

    void merge(size_t i, size_t j)
    {
        size_t x = find(i), y = find(j);
        if (ranks_[x] <= ranks_[y])
            parents_[x] = y;
        else
            parents_[y] = x;
        if (ranks_[x] == ranks_[y] && x != y)
            ranks_[y]++;
    }

private:
    std::vector<size_t> parents_;
    std::vector<size_t> ranks_;
};