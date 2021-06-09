#pragma once

#include <vector>

class UnionFindSet {
public:
    using num_t = uint32_t;
    using id_t  = uint32_t;
    
    explicit UnionFindSet(num_t num) 
        : parents_(std::vector<id_t>(num)) 
        , ranks_(std::vector<num_t>(num, 0))
    {
        for(id_t i = 0; i < num; ++i) {
            parents_[i] = i;
        }
    }

    UnionFindSet(const UnionFindSet&) = delete;
    UnionFindSet(UnionFindSet&&) = delete;
    
    id_t find(id_t x) 
    {
        return x == parents_[x] ? x : (parents_[x] = find(parents_[x]));
    }

    void merge(id_t i, id_t j)
    {
        id_t x = find(i), y = find(j);
        if (ranks_[x] <= ranks_[y])
            parents_[x] = y;
        else
            parents_[y] = x;
        if (ranks_[x] == ranks_[y] && x != y)
            ranks_[y]++;
    }

private:
    std::vector<id_t> parents_;
    std::vector<num_t> ranks_;
};